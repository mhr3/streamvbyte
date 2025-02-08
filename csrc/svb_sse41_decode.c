#include <stddef.h>
#include <x86intrin.h>

#include "svb_tables_decode.h"

static inline __m128i svb_decode_quad(uint8_t key, const uint8_t *__restrict__ *dataPtrPtr)
{
#ifdef AVOIDLENGTHLOOKUP
    // this avoids the dependency on lengthTable,
    // see https://github.com/lemire/streamvbyte/issues/12
    uint8_t len = pshuf[12 + (key >> 6)] + 1;
#else
    uint8_t len = lengthTable_1234[key];
#endif
    __m128i data = _mm_loadu_si128((const __m128i *)*dataPtrPtr);
    uint8_t *pshuf = (uint8_t *)&shuffleTable_1234[key];
    __m128i shuf = *(__m128i *)pshuf;
    data = _mm_shuffle_epi8(data, shuf);
    *dataPtrPtr += len;
    return data;
}

static inline __m128i svb_decode_quad_alt(uint8_t key, const uint8_t *__restrict__ *dataPtrPtr)
{
    uint8_t len = lengthTable_0124[key];
    __m128i data = _mm_loadu_si128((const __m128i *)*dataPtrPtr);
    uint8_t *pshuf = (uint8_t *)&shuffleTable_0124[key];
    __m128i shuf = *(__m128i *)pshuf;
    data = _mm_shuffle_epi8(data, shuf);
    *dataPtrPtr += len;
    return data;
}

static inline void svb_write_sse41(uint32_t *out, __m128i vec)
{
    _mm_storeu_si128((__m128i *)out, vec);
}

#define BroadcastLastXMM 0xFF // bits 0-7 all set to choose highest element

static inline __m128i svb_write_16bit_sse41_delta(uint32_t *out, __m128i vec,
                                                  __m128i prev)
{
    __m128i High16To32 = _mm_set_epi64x(0xFFFF0F0EFFFF0D0CLL,
                                        0xFFFF0B0AFFFF0908LL);
    // vec == [A B C D E F G H] (16 bit values)
    __m128i add = _mm_slli_si128(vec, 2);             // [- A B C D E F G]
    prev = _mm_shuffle_epi32(prev, BroadcastLastXMM); // [P P P P] (32-bit)
    vec = _mm_add_epi32(vec, add);                    // [A AB BC CD DE FG GH]
    add = _mm_slli_si128(vec, 4);                     // [- - A AB BC CD DE EF]
    vec = _mm_add_epi32(vec, add);                    // [A AB ABC ABCD BCDE CDEF DEFG EFGH]
    __m128i V1 = _mm_cvtepu16_epi32(vec);             // [A AB ABC ABCD] (32-bit)
    V1 = _mm_add_epi32(V1, prev);                     // [PA PAB PABC PABCD] (32-bit)
    __m128i V2 = _mm_shuffle_epi8(vec, High16To32);   // [BCDE CDEF DEFG EFGH] (32-bit)
    V2 = _mm_add_epi32(V1, V2);                       // [PABCDE PABCDEF PABCDEFG PABCDEFGH] (32-bit)
    svb_write_sse41(out, V1);
    svb_write_sse41(out + 4, V2);
    return V2;
}

static __m128i svb_write_sse41_delta(uint32_t *out, __m128i vec, __m128i prev)
{
    __m128i add = _mm_slli_si128(vec, 4);             // Cycle 1: [- A B C] (already done)
    prev = _mm_shuffle_epi32(prev, BroadcastLastXMM); // Cycle 2: [P P P P]
    vec = _mm_add_epi32(vec, add);                    // Cycle 2: [A AB BC CD]
    add = _mm_slli_si128(vec, 8);                     // Cycle 3: [- - A AB]
    vec = _mm_add_epi32(vec, prev);                   // Cycle 3: [PA PAB PBC PCD]
    vec = _mm_add_epi32(vec, add);                    // Cycle 4: [PA PAB PABC PABCD]

    svb_write_sse41(out, vec);
    return vec;
}

static const uint8_t *svb_delta_decode_sse4_u32_std(uint32_t *out,
                                                    const uint8_t *__restrict__ keyPtr,
                                                    const uint8_t *__restrict__ dataPtr,
                                                    uint64_t count, uint32_t previous)
{
    const uint64_t keybytes = count / 4; // number of key bytes
    __m128i prev = _mm_set1_epi32((int32_t)previous);
    __m128i data;

    const uint64_t *keyPtr64 = (const uint64_t *)keyPtr;

    for (const uint64_t *keyBoundPtr64 = (const uint64_t *)(keyPtr + keybytes - 7);
         keyPtr64 < keyBoundPtr64;
         keyPtr64++)
    {
        uint64_t keys = *keyPtr64;

        // faster 16-bit delta since we only have 8-bit values
        if (!keys)
        { // 32 1-byte ints in a row
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
            prev = svb_write_16bit_sse41_delta(out, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
            prev = svb_write_16bit_sse41_delta(out + 8, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
            prev = svb_write_16bit_sse41_delta(out + 16, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 24)));
            prev = svb_write_16bit_sse41_delta(out + 24, data, prev);
            out += 32;
            dataPtr += 32;
            continue;
        }

        data = svb_decode_quad(keys & 0x00FF, &dataPtr);
        prev = svb_write_sse41_delta(out, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 4, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 8, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 12, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 16, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 20, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 24, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 28, data, prev);

        out += 32;
    }

    return dataPtr;
}

static const uint8_t *svb_delta_decode_sse4_u32_alt(uint32_t *out,
                                                    const uint8_t *__restrict__ keyPtr,
                                                    const uint8_t *__restrict__ dataPtr,
                                                    uint64_t count, uint32_t previous)
{
    const uint64_t keybytes = count / 4; // number of key bytes
    __m128i prev = _mm_set1_epi32((int32_t)previous);
    __m128i data;

    const uint64_t *keyPtr64 = (const uint64_t *)keyPtr;

    for (const uint64_t *keyBoundPtr64 = (const uint64_t *)(keyPtr + keybytes - 7);
         keyPtr64 < keyBoundPtr64;
         keyPtr64++)
    {
        uint64_t keys = *keyPtr64;

        // faster 16-bit delta since we only have 8-bit values
        // byte: 0b01010101 = 0x55
        if (keys == 0x5555555555555555ull)
        { // 32 1-byte ints in a row
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
            prev = svb_write_16bit_sse41_delta(out, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
            prev = svb_write_16bit_sse41_delta(out + 8, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
            prev = svb_write_16bit_sse41_delta(out + 16, data, prev);
            data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 24)));
            prev = svb_write_16bit_sse41_delta(out + 24, data, prev);
            out += 32;
            dataPtr += 32;
            continue;
        }

        data = svb_decode_quad_alt(keys & 0x00FF, &dataPtr);
        prev = svb_write_sse41_delta(out, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 4, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 8, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 12, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 16, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 20, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        prev = svb_write_sse41_delta(out + 24, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        prev = svb_write_sse41_delta(out + 28, data, prev);

        out += 32;
    }

    return dataPtr;
}

static inline __m128i svb_zigzag_decode_sse4(__m128i val)
{
    // SSE4 for: (val >> 1) ^ (0 - (val & 1));
    __m128i shifted = _mm_srli_epi32(val, 1); // val >> 1
    __m128i ones = _mm_set1_epi32(1);
    __m128i masked = _mm_and_si128(val, ones);
    __m128i negated = _mm_sub_epi32(_mm_setzero_si128(), masked);

    return _mm_xor_si128(shifted, negated);
}

static const uint8_t *svb_delta_decode_sse4_s32_std(uint32_t *out,
                                                    const uint8_t *__restrict__ keyPtr,
                                                    const uint8_t *__restrict__ dataPtr,
                                                    uint64_t count, int32_t previous)
{
    const uint64_t keybytes = count / 4; // number of key bytes
    __m128i prev = _mm_set1_epi32(previous);
    __m128i data;

    const uint64_t *keyPtr64 = (const uint64_t *)keyPtr;

    for (const uint64_t *keyBoundPtr64 = (const uint64_t *)(keyPtr + keybytes - 7);
         keyPtr64 < keyBoundPtr64;
         keyPtr64++)
    {
        uint64_t keys = *keyPtr64;

        data = svb_decode_quad(keys & 0x00FF, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 0, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 4, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 8, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 12, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 16, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 20, data, prev);

        keys >>= 16;
        data = svb_decode_quad((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 24, data, prev);
        data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 28, data, prev);

        out += 32;
    }

    return dataPtr;
}

static const uint8_t *svb_delta_decode_sse4_s32_alt(uint32_t *out,
                                                    const uint8_t *__restrict__ keyPtr,
                                                    const uint8_t *__restrict__ dataPtr,
                                                    uint64_t count, int32_t previous)
{
    const uint64_t keybytes = count / 4; // number of key bytes
    __m128i prev = _mm_set1_epi32(previous);
    __m128i data;

    const uint64_t *keyPtr64 = (const uint64_t *)keyPtr;

    for (const uint64_t *keyBoundPtr64 = (const uint64_t *)(keyPtr + keybytes - 7);
         keyPtr64 < keyBoundPtr64;
         keyPtr64++)
    {
        uint64_t keys = *keyPtr64;

        data = svb_decode_quad_alt(keys & 0x00FF, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 0, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 4, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 8, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 12, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 16, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 20, data, prev);

        keys >>= 16;
        data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 24, data, prev);
        data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
        data = svb_zigzag_decode_sse4(data);
        prev = svb_write_sse41_delta(out + 28, data, prev);

        out += 32;
    }

    return dataPtr;
}
