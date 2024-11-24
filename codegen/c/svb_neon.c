#include <stddef.h>
#include <arm_neon.h>

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
        dataPtr += streamvbyte_encode_quad(in, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    return (uint64_t)(svb_encode_scalar(in, keyPtr, dataPtr, count) - out);
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
    const uint8_t *keyPtr = (uint8_t *)in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = svb_decode_quad(*keyPtr, &currPtr);
        vst1q_u32(out, data);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    currPtr = svb_decode_scalar(&out, keyPtr, currPtr, count);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_decode(in []byte, count int, prev uint32, out *uint32) uint64
uint64_t svb_delta_decode_vector(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                                 int64_t count, uint32_t prev, uint32_t *out)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = (uint8_t *)in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    uint32x4_t previous = vdupq_n_u32(prev);
    uint32x4_t data;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        data = svb_decode_quad(*keyPtr, &currPtr);
        previous = svb_prefix_sum(data, previous);
        vst1q_u32(out, previous);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    currPtr = svb_decode_scalar_delta(&out, keyPtr, currPtr, count, prev);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}