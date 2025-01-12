#include "svb_type.h"

static inline uint8_t svb_encode_data_1234(uint32_t val, uint8_t *__restrict__ *dataPtrPtr)
{
    uint8_t *dataPtr = *dataPtrPtr;
    uint8_t code;

    if (val < (1 << 8))
    {
        // 1 byte
        *dataPtr = (uint8_t)(val);
        *dataPtrPtr += 1;
        code = 0;
    }
    else if (val < (1 << 16))
    {
        // 2 bytes
        __builtin_memcpy(dataPtr, &val, 2); // assumes little endian
        *dataPtrPtr += 2;
        code = 1;
    }
    else if (val < (1 << 24))
    {
        // 3 bytes
        __builtin_memcpy(dataPtr, &val, 3); // assumes little endian
        *dataPtrPtr += 3;
        code = 2;
    }
    else
    {
        // 4 bytes
        __builtin_memcpy(dataPtr, &val, sizeof(uint32_t));
        *dataPtrPtr += sizeof(uint32_t);
        code = 3;
    }

    return code;
}

static inline uint8_t svb_encode_data_0124(uint32_t val, uint8_t *__restrict__ *dataPtrPtr)
{
    uint8_t *dataPtr = *dataPtrPtr;
    uint8_t code;

    if (val == 0)
    {
        code = 0;
    }
    else if (val < (1 << 8))
    {
        // 1 byte
        *dataPtr = (uint8_t)(val);
        *dataPtrPtr += 1;
        code = 1;
    }
    else if (val < (1 << 16))
    {
        // 2 bytes
        __builtin_memcpy(dataPtr, &val, 2); // assumes little endian
        *dataPtrPtr += 2;
        code = 2;
    }
    else
    {
        // 4 bytes
        __builtin_memcpy(dataPtr, &val, sizeof(uint32_t));
        *dataPtrPtr += sizeof(uint32_t);
        code = 3;
    }

    return code;
}

static inline uint32_t svb_zigzag_encode_32(uint32_t val)
{
    return (val + val) ^ (uint32_t)((int32_t)val >> 31);
}

static inline uint8_t *svb_scalar_encode(const uint32_t *in,
                                         uint8_t *__restrict__ keyPtr,
                                         uint8_t *__restrict__ dataPtr,
                                         uint32_t count, EncodeType encodeType)
{
    if (count == 0)
        return dataPtr; // exit immediately if no data

    uint8_t shift = 0; // cycles 0, 2, 4, 6, 0, 2, 4, 6, ...
    uint8_t key = 0;

    for (uint32_t c = 0; c < count; c++)
    {
        if (shift == 8)
        {
            shift = 0;
            *keyPtr++ = key;
            key = 0;
        }
        uint32_t val = in[c];
        uint8_t code;
        switch (encodeType)
        {
        case stdEncode:
            code = svb_encode_data_1234(val, &dataPtr);
            break;
        case zzEncode:
            code = svb_encode_data_1234(svb_zigzag_encode_32(val), &dataPtr);
            break;
        case altEncode:
            code = svb_encode_data_0124(val, &dataPtr);
            break;
        }
        key |= code << shift;
        shift += 2;
    }

    *keyPtr = key;  // write last key (no increment needed)
    return dataPtr; // pointer to first unused data byte
}

static inline uint8_t *svb_scalar_delta_encode(const uint32_t *in,
                                               uint8_t *__restrict__ keyPtr,
                                               uint8_t *__restrict__ dataPtr,
                                               uint32_t count, EncodeType encodeType, uint32_t prev)
{
    if (count == 0)
        return dataPtr; // exit immediately if no data

    uint8_t shift = 0; // cycles 0, 2, 4, 6, 0, 2, 4, 6, ...
    uint8_t key = 0;

    for (uint32_t c = 0; c < count; c++)
    {
        if (shift == 8)
        {
            shift = 0;
            *keyPtr++ = key;
            key = 0;
        }
        prev = in[c];
        uint8_t code;
        switch (encodeType)
        {
        case stdEncode:
            code = svb_encode_data_1234(in[c] - prev, &dataPtr);
            break;
        case zzEncode:
            code = svb_encode_data_1234(svb_zigzag_encode_32((int32_t)in[c] - (int32_t)prev), &dataPtr);
            break;
        case altEncode:
            code = svb_encode_data_0124(in[c] - prev, &dataPtr);
            break;
        }
        key |= code << shift;
        shift += 2;
    }

    *keyPtr = key;  // write last key (no increment needed)
    return dataPtr; // pointer to first unused data byte
}
