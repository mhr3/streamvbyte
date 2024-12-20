#include <stddef.h>
#include <arm_neon.h>

#include "svb_type.h"

#include "svb_scalar_decode.c"
#include "svb_scalar_encode.c"
#include "svb_neon_decode.c"
#include "svb_neon_encode.c"

// gocc: svb_encode(in []uint32, out *byte) uint64
uint64_t svb_encode(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint8_t *out)
{
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const uint32x4_t data = vld1q_u32(in);
        dataPtr += svb_encode_quad(data, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

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
        const uint32x4_t data = vld1q_u32(in);
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

    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = svb_decode_quad(*keyPtr, &currPtr);
        vst1q_u32(out, data);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    currPtr = svb_scalar_decode(&out, keyPtr, currPtr, count, stdEncode);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_decode_alt(in []byte, count int, out *uint32) uint64
uint64_t svb_decode_alt(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                        int64_t count, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = svb_decode_quad_alt(*keyPtr, &currPtr);
        vst1q_u32(out, data);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    currPtr = svb_scalar_decode(&out, keyPtr, currPtr, count, altEncode);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_encode(in []uint32, prev uint32, out *byte) uint64
uint64_t svb_delta_encode(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint32_t prev, uint8_t *out)
{
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    uint32x4_t previous = vdupq_n_u32(prev);

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const uint32x4_t data = vld1q_u32(in);
        const uint32x4_t diff = svb_differences(data, previous);
        dataPtr += svb_encode_quad(diff, dataPtr, keyPtr);
        previous = data;
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    if (count > 0 && count_quads > 0)
        prev = in[-1];

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

    uint32x4_t previous = vdupq_n_u32(prev);

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const uint32x4_t data = vld1q_u32(in);
        const uint32x4_t diff = svb_differences(data, previous);
        dataPtr += svb_encode_quad_alt(diff, dataPtr, keyPtr);
        previous = data;
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    if (count > 0 && count_quads > 0)
        prev = in[-1];

    return (uint64_t)(svb_scalar_delta_encode(in, keyPtr, dataPtr, count, altEncode, prev) - out);
}

// gocc: svb_delta_decode(in []byte, count int, prev uint32, out *uint32) uint64
uint64_t svb_delta_decode(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                          int64_t count, uint32_t prev, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    uint32x4_t previous = vdupq_n_u32(prev);

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = svb_decode_quad(*keyPtr, &currPtr);
        previous = svb_prefix_sum(data, previous);
        vst1q_u32(out, previous);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    currPtr = svb_scalar_delta_decode(&out, keyPtr, currPtr, count, stdEncode, prev);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_decode_alt(in []byte, count int, prev uint32, out *uint32) uint64
uint64_t svb_delta_decode_alt(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                          int64_t count, uint32_t prev, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    uint32x4_t previous = vdupq_n_u32(prev);

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = svb_decode_quad_alt(*keyPtr, &currPtr);
        previous = svb_prefix_sum(data, previous);
        vst1q_u32(out, previous);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    currPtr = svb_scalar_delta_decode(&out, keyPtr, currPtr, count, altEncode, prev);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}