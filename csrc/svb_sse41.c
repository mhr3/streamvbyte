#include <stddef.h>
#include <x86intrin.h>

#include "svb_type.h"

#include "svb_scalar_decode.c"
#include "svb_scalar_encode.c"
#include "svb_sse41_decode.c"
#include "svb_sse41_encode.c"

// gocc: svb_encode(in []uint32, out *byte) uint64
uint64_t svb_encode(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint8_t *out)
{
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4; // 2-bits rounded to full byte
    uint8_t *restrict keyPtr = out;
    uint8_t *restrict dataPtr = keyPtr + keyLen; // variable byte data after all keys

    for (const uint32_t *end = &in [(count & ~7U)]; in != end; in += 8)
    {
        __m128i r0, r1;
        uint16_t keys;

        r0 = _mm_loadu_si128((const __m128i *)&in[0]);
        r1 = _mm_loadu_si128((const __m128i *)&in[4]);

        dataPtr += svb_encode_two_quads(r0, r1, dataPtr, &keys);

        *((uint16_t *)keyPtr) = keys;
        keyPtr += 2;
    }
    // do remaining
    count %= 8;

    return (uint64_t)(svb_scalar_encode(in, keyPtr, dataPtr, count, stdEncode) - out);
}

// gocc: svb_encode_alt(in []uint32, out *byte) uint64
uint64_t svb_encode_alt(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint8_t *out)
{
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const __m128i data = _mm_loadu_si128((const __m128i *)in);
        dataPtr += svb_encode_quad_alt(data, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    return (uint64_t)(svb_scalar_encode(in, keyPtr, dataPtr, count, altEncode) - out);
}

// gocc: svb_decode(in []byte, count int, out *uint32) uint64
uint64_t svb_decode(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                    int64_t count, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *keyPtr = in;               // full list of keys is next
    uint32_t keyLen = ((count + 3) / 4);      // 2-bits per key (rounded up)
    const uint8_t *dataPtr = keyPtr + keyLen; // data starts at end of keys
    const uint32_t *outStartPtr = out;

    // FIXME: we're not checking whether there's enough "in" bytes left
    dataPtr = svb_decode_quad_quad(out, keyPtr, dataPtr, count);
    out += count & ~31U;
    keyPtr += (count / 4) & ~7U;
    count &= 31;

    dataPtr = svb_scalar_decode(&out, keyPtr, dataPtr, count, stdEncode);
    if (dataPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_decode_alt(in []byte, count int, out *uint32) uint64
uint64_t svb_decode_alt(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                        int64_t count, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *keyPtr = in;               // full list of keys is next
    uint32_t keyLen = ((count + 3) / 4);      // 2-bits per key (rounded up)
    const uint8_t *dataPtr = keyPtr + keyLen; // data starts at end of keys
    const uint32_t *outStartPtr = out;

    // FIXME: we're not checking whether there's enough "in" bytes left
    dataPtr = svb_decode_quad_quad_alt(out, keyPtr, dataPtr, count);
    out += count & ~31U;
    keyPtr += (count / 4) & ~7U;
    count &= 31;

    dataPtr = svb_scalar_decode(&out, keyPtr, dataPtr, count, altEncode);
    if (dataPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_encode(in []uint32, prev uint32, out *byte) uint64
uint64_t svb_delta_encode(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint32_t prev, uint8_t *out)
{
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4; // 2-bits rounded to full byte
    uint8_t *restrict keyPtr = out;
    uint8_t *restrict dataPtr = keyPtr + keyLen; // variable byte data after all keys

    __m128i Prev = _mm_set1_epi32((int32_t)prev);

    for (const uint32_t *end = &in [(count & ~7U)]; in != end; in += 8)
    {
        __m128i data0, deltas0, data1, deltas1;
        uint16_t keys;

        data0 = _mm_loadu_si128((const __m128i *)&in[0]);
        deltas0 = svb_differences(data0, Prev);
        Prev = data0;
        data1 = _mm_loadu_si128((const __m128i *)&in[4]);
        deltas1 = svb_differences(data1, Prev);
        Prev = data1;

        dataPtr += svb_encode_two_quads(deltas0, deltas1, dataPtr, &keys);

        *((uint16_t *)keyPtr) = keys;
        keyPtr += 2;
    }

    prev = (uint32_t)_mm_extract_epi32(Prev, 3);

    // do remaining
    count %= 8;

    return (uint64_t)(svb_scalar_delta_encode(in, keyPtr, dataPtr, count, stdEncode, prev) - out);
}

// gocc: svb_delta_encode_alt(in []uint32, prev uint32, out *byte) uint64
uint64_t svb_delta_encode_alt(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint32_t prev, uint8_t *out)
{
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;
    __m128i Prev = _mm_set1_epi32((int32_t)prev);

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const __m128i data = _mm_loadu_si128((const __m128i *)in);
        const __m128i deltas = svb_differences(data, Prev);
        Prev = data;

        dataPtr += svb_encode_quad_alt(deltas, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    prev = (uint32_t)_mm_extract_epi32(Prev, 3);

    count -= 4 * count_quads;

    return (uint64_t)(svb_scalar_delta_encode(in, keyPtr, dataPtr, count, altEncode, prev) - out);
}

// gocc: svb_delta_decode(in []byte, count int, prev uint32, out *uint32) uint64
uint64_t svb_delta_decode(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                          int64_t count, uint32_t prev, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    uint32_t keyLen = ((count + 3) / 4); // 2-bits per key (rounded up)
    const uint8_t *keyPtr = in;
    const uint8_t *dataPtr = keyPtr + keyLen; // data starts at end of keys
    const uint32_t *outStartPtr = out;

    // FIXME: we're not checking whether there's enough "in" bytes left
    dataPtr = svb_delta_decode_quad_quad(out, keyPtr, dataPtr, count, prev);
    out += count & ~31U;
    keyPtr += (count / 4) & ~7U;
    count &= 31;

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    dataPtr = svb_scalar_delta_decode(&out, keyPtr, dataPtr, count, stdEncode, prev);
    if (dataPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_decode_alt(in []byte, count int, prev uint32, out *uint32) uint64
uint64_t svb_delta_decode_alt(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                              int64_t count, uint32_t prev, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    uint32_t keyLen = ((count + 3) / 4); // 2-bits per key (rounded up)
    const uint8_t *keyPtr = in;
    const uint8_t *dataPtr = keyPtr + keyLen; // data starts at end of keys
    const uint32_t *outStartPtr = out;

    // FIXME: we're not checking whether there's enough "in" bytes left
    dataPtr = svb_delta_decode_quad_quad_alt(out, keyPtr, dataPtr, count, prev);
    out += count & ~31U;
    keyPtr += (count / 4) & ~7U;
    count &= 31;

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    dataPtr = svb_scalar_delta_decode(&out, keyPtr, dataPtr, count, altEncode, prev);
    if (dataPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}