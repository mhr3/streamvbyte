#include <stddef.h>
#include <x86intrin.h>

#include "svb_tables_decode.h"

static inline __m128i svb_decode_quad(uint32_t key,
                                      const uint8_t *__restrict__ *dataPtrPtr)
{
    uint8_t len;
    __m128i Data = _mm_loadu_si128((const __m128i *)*dataPtrPtr);
    uint8_t *pshuf = (uint8_t *)&shuffleTable_1234[key];
    __m128i Shuf = *(__m128i *)pshuf;
#ifdef AVOIDLENGTHLOOKUP
    // this avoids the dependency on lengthTable,
    // see https://github.com/lemire/streamvbyte/issues/12
    len = pshuf[12 + (key >> 6)] + 1;
#else
    len = lengthTable_1234[key];
#endif
    Data = _mm_shuffle_epi8(Data, Shuf);
    *dataPtrPtr += len;
    return Data;
}

static inline __m128i svb_decode_quad_alt(uint32_t key,
                                          const uint8_t *__restrict__ *dataPtrPtr)
{
    uint8_t len = lengthTable_0124[key];
    __m128i Data = _mm_loadu_si128((const __m128i *)*dataPtrPtr);
    uint8_t *pshuf = (uint8_t *)&shuffleTable_0124[key];
    __m128i Shuf = *(__m128i *)pshuf;
    Data = _mm_shuffle_epi8(Data, Shuf);
    *dataPtrPtr += len;
    return Data;
}

static inline void svb_write_sse41(uint32_t *out, __m128i Vec)
{
    _mm_storeu_si128((__m128i *)out, Vec);
}

static inline const uint8_t *svb_decode_quad_quad(uint32_t *out,
                                                  const uint8_t *__restrict__ keyPtr,
                                                  const uint8_t *__restrict__ dataPtr,
                                                  uint64_t count)
{
    uint64_t keybytes = count / 4; // number of key bytes
    __m128i data;

    if (keybytes >= 8)
    {
        int64_t offset = -(int64_t)keybytes / 8 + 1;

        const uint64_t *keyPtr64 = (const uint64_t *)keyPtr - offset;
        uint64_t nextkeys;
        __builtin_memcpy(&nextkeys, keyPtr64 + offset, sizeof(nextkeys));
        for (; offset != 0; ++offset)
        {
            uint64_t keys = nextkeys;
            __builtin_memcpy(&nextkeys, keyPtr64 + offset + 1, sizeof(nextkeys));

            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 4, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 8, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 12, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 16, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 20, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 24, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 28, data);

            out += 32;
        }
        {
            uint64_t keys = nextkeys;

            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 4, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 8, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 12, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 16, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 20, data);

            keys >>= 16;
            data = svb_decode_quad((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 24, data);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 28, data);

            out += 32;
        }
    }
    return dataPtr;
}

static inline const uint8_t *svb_decode_quad_quad_alt(uint32_t *out,
                                                      const uint8_t *__restrict__ keyPtr,
                                                      const uint8_t *__restrict__ dataPtr,
                                                      uint64_t count)
{
    uint64_t keybytes = count / 4; // number of key bytes
    __m128i data;
    if (keybytes >= 8)
    {
        int64_t offset = -(int64_t)keybytes / 8 + 1;

        const uint64_t *keyPtr64 = (const uint64_t *)keyPtr - offset;
        uint64_t nextkeys;
        __builtin_memcpy(&nextkeys, keyPtr64 + offset, sizeof(nextkeys));
        for (; offset != 0; ++offset)
        {
            uint64_t keys = nextkeys;
            __builtin_memcpy(&nextkeys, keyPtr64 + offset + 1, sizeof(nextkeys));

            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 4, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 8, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 12, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 16, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 20, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 24, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 28, data);

            out += 32;
        }
        {
            uint64_t keys = nextkeys;

            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 4, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 8, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 12, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 16, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 20, data);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0xFF), &dataPtr);
            svb_write_sse41(out + 24, data);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            svb_write_sse41(out + 28, data);

            out += 32;
        }
    }

    return dataPtr;
}

#define BroadcastLastXMM 0xFF // bits 0-7 all set to choose highest element

static inline __m128i svb_write_16bit_sse41_d1(uint32_t *out, __m128i Vec,
                                               __m128i Prev)
{
    __m128i High16To32 = _mm_set_epi64x(0xFFFF0F0EFFFF0D0CLL,
                                        0xFFFF0B0AFFFF0908LL);
    // vec == [A B C D E F G H] (16 bit values)
    __m128i Add = _mm_slli_si128(Vec, 2);             // [- A B C D E F G]
    Prev = _mm_shuffle_epi32(Prev, BroadcastLastXMM); // [P P P P] (32-bit)
    Vec = _mm_add_epi32(Vec, Add);                    // [A AB BC CD DE FG GH]
    Add = _mm_slli_si128(Vec, 4);                     // [- - A AB BC CD DE EF]
    Vec = _mm_add_epi32(Vec, Add);                    // [A AB ABC ABCD BCDE CDEF DEFG EFGH]
    __m128i V1 = _mm_cvtepu16_epi32(Vec);             // [A AB ABC ABCD] (32-bit)
    V1 = _mm_add_epi32(V1, Prev);                     // [PA PAB PABC PABCD] (32-bit)
    __m128i V2 = _mm_shuffle_epi8(Vec, High16To32);   // [BCDE CDEF DEFG EFGH] (32-bit)
    V2 = _mm_add_epi32(V1, V2);                       // [PABCDE PABCDEF PABCDEFG PABCDEFGH] (32-bit)
    svb_write_sse41(out, V1);
    svb_write_sse41(out + 4, V2);
    return V2;
}

static __m128i svb_write_sse41_d1(uint32_t *out, __m128i Vec, __m128i Prev)
{
    __m128i Add = _mm_slli_si128(Vec, 4);             // Cycle 1: [- A B C] (already done)
    Prev = _mm_shuffle_epi32(Prev, BroadcastLastXMM); // Cycle 2: [P P P P]
    Vec = _mm_add_epi32(Vec, Add);                    // Cycle 2: [A AB BC CD]
    Add = _mm_slli_si128(Vec, 8);                     // Cycle 3: [- - A AB]
    Vec = _mm_add_epi32(Vec, Prev);                   // Cycle 3: [PA PAB PBC PCD]
    Vec = _mm_add_epi32(Vec, Add);                    // Cycle 4: [PA PAB PABC PABCD]

    svb_write_sse41(out, Vec);
    return Vec;
}

static const uint8_t *svb_delta_decode_quad_quad(uint32_t *out,
                                                 const uint8_t *__restrict__ keyPtr,
                                                 const uint8_t *__restrict__ dataPtr,
                                                 uint64_t count, uint32_t prev)
{
    uint64_t keybytes = count / 4; // number of key bytes
    if (keybytes >= 8)
    {
        __m128i Prev = _mm_set1_epi32((int32_t)prev);
        __m128i data;
        int64_t offset = -(int64_t)keybytes / 8 + 1;

        const uint64_t *keyPtr64 = (const uint64_t *)keyPtr - offset;
        uint64_t nextkeys;
        __builtin_memcpy(&nextkeys, keyPtr64 + offset, sizeof(nextkeys));
        for (; offset != 0; ++offset)
        {
            uint64_t keys = nextkeys;
            __builtin_memcpy(&nextkeys, keyPtr64 + offset + 1, sizeof(nextkeys));
            // faster 16-bit delta since we only have 8-bit values
            if (!keys)
            { // 32 1-byte ints in a row
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
                Prev = svb_write_16bit_sse41_d1(out, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
                Prev = svb_write_16bit_sse41_d1(out + 8, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
                Prev = svb_write_16bit_sse41_d1(out + 16, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 24)));
                Prev = svb_write_16bit_sse41_d1(out + 24, data, Prev);
                out += 32;
                dataPtr += 32;
                continue;
            }

            data = svb_decode_quad(keys & 0x00FF, &dataPtr);
            Prev = svb_write_sse41_d1(out, data, Prev);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 4, data, Prev);

            keys >>= 16;
            data = svb_decode_quad((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 8, data, Prev);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 12, data, Prev);

            keys >>= 16;
            data = svb_decode_quad((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 16, data, Prev);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 20, data, Prev);

            keys >>= 16;
            data = svb_decode_quad((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 24, data, Prev);
            data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 28, data, Prev);

            out += 32;
        }
        {
            uint64_t keys = nextkeys;
            // faster 16-bit delta since we only have 8-bit values
            if (!keys)
            { // 32 1-byte ints in a row
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
                Prev = svb_write_16bit_sse41_d1(out, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
                Prev = svb_write_16bit_sse41_d1(out + 8, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
                Prev = svb_write_16bit_sse41_d1(out + 16, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i *)(dataPtr + 24)));
                Prev = svb_write_16bit_sse41_d1(out + 24, data, Prev);
                out += 32;
                dataPtr += 32;
            }
            else
            {
                data = svb_decode_quad(keys & 0x00FF, &dataPtr);
                Prev = svb_write_sse41_d1(out, data, Prev);
                data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 4, data, Prev);

                keys >>= 16;
                data = svb_decode_quad((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 8, data, Prev);
                data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 12, data, Prev);

                keys >>= 16;
                data = svb_decode_quad((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 16, data, Prev);
                data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 20, data, Prev);

                keys >>= 16;
                data = svb_decode_quad((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 24, data, Prev);
                data = svb_decode_quad((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 28, data, Prev);

                out += 32;
            }
        }
    }

    return dataPtr;
}

static const uint8_t *svb_delta_decode_quad_quad_alt(uint32_t *out,
                                                     const uint8_t *__restrict__ keyPtr,
                                                     const uint8_t *__restrict__ dataPtr,
                                                     uint64_t count, uint32_t prev)
{
    uint64_t keybytes = count / 4; // number of key bytes
    if (keybytes >= 8)
    {
        __m128i Prev = _mm_set1_epi32((int32_t)prev);
        __m128i data;
        int64_t offset = -(int64_t)keybytes / 8 + 1;

        const uint64_t *keyPtr64 = (const uint64_t *)keyPtr - offset;
        uint64_t nextkeys;
        __builtin_memcpy(&nextkeys, keyPtr64 + offset, sizeof(nextkeys));
        for (; offset != 0; ++offset)
        {
            uint64_t keys = nextkeys;
            __builtin_memcpy(&nextkeys, keyPtr64 + offset + 1, sizeof(nextkeys));
            // faster 16-bit delta since we only have 8-bit values
            // byte: 0b01010101 = 0x55
            if (keys == 0x5555555555555555ull)
            { // 32 1-byte ints in a row
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
                Prev = svb_write_16bit_sse41_d1(out, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
                Prev = svb_write_16bit_sse41_d1(out + 8, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
                Prev = svb_write_16bit_sse41_d1(out + 16, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 24)));
                Prev = svb_write_16bit_sse41_d1(out + 24, data, Prev);
                out += 32;
                dataPtr += 32;
                continue;
            }

            data = svb_decode_quad_alt(keys & 0x00FF, &dataPtr);
            Prev = svb_write_sse41_d1(out, data, Prev);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 4, data, Prev);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 8, data, Prev);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 12, data, Prev);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 16, data, Prev);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 20, data, Prev);

            keys >>= 16;
            data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
            Prev = svb_write_sse41_d1(out + 24, data, Prev);
            data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
            Prev = svb_write_sse41_d1(out + 28, data, Prev);

            out += 32;
        }
        {
            uint64_t keys = nextkeys;
            // faster 16-bit delta since we only have 8-bit values
            if (keys == 0x5555555555555555ull)
            { // 32 1-byte ints in a row
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr)));
                Prev = svb_write_16bit_sse41_d1(out, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 8)));
                Prev = svb_write_16bit_sse41_d1(out + 8, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_lddqu_si128((const __m128i *)(dataPtr + 16)));
                Prev = svb_write_16bit_sse41_d1(out + 16, data, Prev);
                data = _mm_cvtepu8_epi16(_mm_loadl_epi64((const __m128i *)(dataPtr + 24)));
                Prev = svb_write_16bit_sse41_d1(out + 24, data, Prev);
                out += 32;
                dataPtr += 32;
            }
            else
            {
                data = svb_decode_quad_alt(keys & 0x00FF, &dataPtr);
                Prev = svb_write_sse41_d1(out, data, Prev);
                data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 4, data, Prev);

                keys >>= 16;
                data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 8, data, Prev);
                data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 12, data, Prev);

                keys >>= 16;
                data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 16, data, Prev);
                data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 20, data, Prev);

                keys >>= 16;
                data = svb_decode_quad_alt((keys & 0x00FF), &dataPtr);
                Prev = svb_write_sse41_d1(out + 24, data, Prev);
                data = svb_decode_quad_alt((keys & 0xFF00) >> 8, &dataPtr);
                Prev = svb_write_sse41_d1(out + 28, data, Prev);

                out += 32;
            }
        }
    }

    return dataPtr;
}