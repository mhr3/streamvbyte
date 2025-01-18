#include <stddef.h>
#include <arm_neon.h>

#include "svb_type.h"

#include "svb_scalar_decode.c"
#include "svb_scalar_encode.c"
#include "svb_neon_decode.c"
#include "svb_neon_encode.c"

// gocc: svb_encode_u32(in []uint32, out *byte, scheme byte) uint64
uint64_t svb_encode_u32(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint8_t *out, char scheme)
{
    const EncodeType encodeType = scheme != 0 ? altEncode : stdEncode;
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const uint32x4_t data = vld1q_u32(in);
        dataPtr += encodeType == stdEncode ? svb_encode_quad(data, dataPtr, keyPtr) : svb_encode_quad_alt(data, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    return (uint64_t)(svb_scalar_encode(in, keyPtr, dataPtr, count, encodeType) - out);
}

// gocc: svb_encode_s32(in []int32, out *byte, scheme byte) int64
uint64_t svb_encode_s32(const int32_t *in, const uint64_t in_len, uint64_t in_cap, uint8_t *out, char scheme)
{
    const EncodeType encodeType = scheme != 0 ? zzAltEncode : zzEncode;
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const int32x4_t data = vld1q_s32(in);
        const uint32x4_t zzData = svb_zigzag_encode_neon(data);
        dataPtr += encodeType == zzEncode ? svb_encode_quad(zzData, dataPtr, keyPtr) : svb_encode_quad_alt(zzData, dataPtr, keyPtr);
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    return (uint64_t)(svb_scalar_encode((const uint32_t*)in, keyPtr, dataPtr, count, encodeType) - out);
}

// gocc: svb_decode_u32(in []byte, count int, out *uint32, scheme byte) uint64
uint64_t svb_decode_u32(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                    int64_t count, uint32_t *out, char scheme)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const EncodeType encodeType = scheme != 0 ? altEncode : stdEncode;
    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = encodeType == stdEncode ? svb_decode_quad(*keyPtr, &currPtr) : svb_decode_quad_alt(*keyPtr, &currPtr);
        vst1q_u32(out, data);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    currPtr = svb_scalar_decode(&out, keyPtr, currPtr, count, encodeType);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_decode_s32(in []byte, count int, out *int32, scheme byte) uint64
uint64_t svb_decode_s32(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                    int64_t count, int32_t *out, char scheme)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const EncodeType encodeType = scheme != 0 ? zzAltEncode : zzEncode;
    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const int32_t *outStartPtr = out;

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = encodeType == zzEncode ? svb_decode_quad(*keyPtr, &currPtr) : svb_decode_quad_alt(*keyPtr, &currPtr);
        int32x4_t zzData = svb_zigzag_decode_neon(data);
        vst1q_s32(out, zzData);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    currPtr = svb_scalar_decode((uint32_t**)&out, keyPtr, currPtr, count, encodeType);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_encode_u32(in []uint32, prev uint32, out *byte, scheme byte) uint64
uint64_t svb_delta_encode_u32(const uint32_t *in, const uint64_t in_len, uint64_t in_cap, uint32_t prev, uint8_t *out, char scheme)
{
    const EncodeType encodeType = scheme != 0 ? altEncode : stdEncode;
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    uint32x4_t previous = vdupq_n_u32(prev);

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const uint32x4_t data = vld1q_u32(in);
        const uint32x4_t diff = svb_differences_u32(data, previous);
        dataPtr += encodeType == stdEncode ? svb_encode_quad(diff, dataPtr, keyPtr) : svb_encode_quad_alt(diff, dataPtr, keyPtr);
        previous = data;
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    if (count > 0 && count_quads > 0)
        prev = in[-1];

    return (uint64_t)(svb_scalar_delta_encode(in, keyPtr, dataPtr, count, encodeType, prev) - out);
}

// gocc: svb_delta_encode_s32(in []int32, prev int32, out *byte, scheme byte) uint64
uint64_t svb_delta_encode_s32(const int32_t *in, const uint64_t in_len, uint64_t in_cap, int32_t prev, uint8_t *out, char scheme)
{
    const EncodeType encodeType = scheme != 0 ? zzAltEncode : zzEncode;
    uint8_t *keyPtr = out;
    uint32_t count = in_len;
    uint32_t keyLen = (count + 3) / 4;  // 2-bits rounded to full byte
    uint8_t *dataPtr = keyPtr + keyLen; // variable byte data after all keys

    uint32_t count_quads = count / 4;

    int32x4_t previous = vdupq_n_s32(prev);

    for (uint32_t c = 0; c < count_quads; c++)
    {
        const int32x4_t data = vld1q_s32(in);
        const int32x4_t diff = svb_differences_s32(data, previous);
        const uint32x4_t zzData = svb_zigzag_encode_neon(diff);
        dataPtr += encodeType == zzEncode ? svb_encode_quad(zzData, dataPtr, keyPtr) : svb_encode_quad_alt(zzData, dataPtr, keyPtr);
        previous = data;
        keyPtr++;
        in += 4;
    }
    count -= 4 * count_quads;

    if (count > 0 && count_quads > 0)
        prev = in[-1];

    return (uint64_t)(svb_scalar_delta_encode((const uint32_t*)in, keyPtr, dataPtr, count, encodeType, prev) - out);
}

// gocc: svb_delta_decode_u32(in []byte, count int, prev uint32, out *uint32, scheme byte) uint64
uint64_t svb_delta_decode_u32(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                          int64_t count, uint32_t prev, uint32_t *out, char scheme)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const EncodeType encodeType = scheme != 0 ? altEncode : stdEncode;
    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const uint32_t *outStartPtr = out;

    uint32x4_t previous = vdupq_n_u32(prev);

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = encodeType == stdEncode ? svb_decode_quad(*keyPtr, &currPtr) : svb_decode_quad_alt(*keyPtr, &currPtr);
        previous = svb_prefix_sum_u32(data, previous);
        vst1q_u32(out, previous);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    currPtr = svb_scalar_delta_decode(&out, keyPtr, currPtr, count, encodeType, prev);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}

// gocc: svb_delta_decode_s32(in []byte, count int, prev int32, out *int32, scheme byte) uint64
uint64_t svb_delta_decode_s32(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                          int64_t count, int32_t prev, int32_t *out, char scheme)
{
    if (count <= 0 || in_len < (count + 3) / 4)
        return 0;

    const EncodeType encodeType = scheme != 0 ? zzAltEncode : zzEncode;
    const uint8_t *dataStartPtr = &in[(count + 3) / 4];
    const uint8_t *dataEndPtr = in + in_len;
    const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
    const uint8_t *keyPtr = in;
    const uint8_t *currPtr = dataStartPtr;
    const int32_t *outStartPtr = out;

    int32x4_t previous = vdupq_n_s32(prev);

    for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
    {
        uint32x4_t data = encodeType == zzEncode ? svb_decode_quad(*keyPtr, &currPtr) : svb_decode_quad_alt(*keyPtr, &currPtr);
        int32x4_t zzData = svb_zigzag_decode_neon(data);
        previous = svb_prefix_sum_s32(zzData, previous);
        vst1q_s32(out, previous);

        out += 4; // 16-byte shift
    }
    count -= (out - outStartPtr);

    if (count > 0 && out > outStartPtr)
        prev = out[-1];

    currPtr = svb_scalar_delta_decode((uint32_t**)&out, keyPtr, currPtr, count, encodeType, (uint32_t)prev);
    if (currPtr == NULL)
        return 0;

    return (uint64_t)(out - outStartPtr);
}
