#include "svb_type.h"

static inline uint32_t svb_decode_data_1234(const uint8_t **dataPtrPtr, uint8_t code)
{
    const uint8_t *dataPtr = *dataPtrPtr;
    uint32_t val;

    if (code == 0)
    {
        // 1 byte
        val = (uint32_t)*dataPtr;
        dataPtr += 1;
    }
    else if (code == 1)
    {
        // 2 bytes
        val = 0;
        __builtin_memcpy(&val, dataPtr, 2); // assumes little endian
        dataPtr += 2;
    }
    else if (code == 2)
    {
        // 3 bytes
        val = 0;
        __builtin_memcpy(&val, dataPtr, 3); // assumes little endian
        dataPtr += 3;
    }
    else // code == 3
    {
        __builtin_memcpy(&val, dataPtr, 4);
        dataPtr += 4;
    }

    *dataPtrPtr = dataPtr;
    return val;
}

static inline uint32_t svb_decode_data_0124(const uint8_t **dataPtrPtr, uint8_t code)
{
    const uint8_t *dataPtr = *dataPtrPtr;
    uint32_t val;

    if (code == 0)
    {
        // 0 byte
        val = 0;
        // no advance
    }
    else if (code == 1)
    {
        // 1 byte
        // extend to 4 bytes
        val = (uint32_t)*dataPtr;
        dataPtr += 1;
    }
    else if (code == 2)
    {
        // 2 bytes
        val = 0;
        __builtin_memcpy(&val, dataPtr, 2); // assumes little endian
        dataPtr += 2;
    }
    else
    {
        // code == 3, 4 bytes
        __builtin_memcpy(&val, dataPtr, 4);
        dataPtr += 4;
    }

    *dataPtrPtr = dataPtr;
    return val;
}

static inline int32_t svb_zigzag_decode_32(uint32_t val)
{
    return (val >> 1) ^ (0 - (val & 1));
}

// FIXME: this isn't checking whether dataPtr is within bounds
static inline const uint8_t *svb_scalar_decode(uint32_t **outPtrPtr, const uint8_t *keyPtr,
                                               const uint8_t *dataPtr,
                                               uint32_t count, EncodeType encodeType)
{
    // no reads or writes if no data
    if (count == 0 || dataPtr == NULL)
        return dataPtr;

    uint8_t shift = 0;
    uint32_t key = *keyPtr++;
    uint32_t *outPtr = *outPtrPtr;

    for (uint32_t c = 0; c < count; c++)
    {
        if (shift == 8)
        {
            shift = 0;
            key = *keyPtr++;
        }
        uint32_t val;
        switch (encodeType)
        {
        case stdEncode:
            val = svb_decode_data_1234(&dataPtr, (key >> shift) & 0x3);
            break;
        case zzEncode:
            val = svb_decode_data_1234(&dataPtr, (key >> shift) & 0x3);
            val = (uint32_t)svb_zigzag_decode_32(val);
            break;
        case altEncode:
            val = svb_decode_data_0124(&dataPtr, (key >> shift) & 0x3);
            break;
        }
        *outPtr++ = val;
        shift += 2;
    }

    *outPtrPtr = outPtr;

    return dataPtr; // pointer to first unused byte after end
}

// FIXME: this isn't checking whether dataPtr is within bounds
static inline const uint8_t *svb_scalar_delta_decode(uint32_t **outPtrPtr, const uint8_t *keyPtr,
                                                     const uint8_t *dataPtr, uint32_t count, EncodeType encodeType, uint32_t prev)
{
    // no reads or writes if no data
    if (count == 0 || dataPtr == NULL)
        return dataPtr;

    uint8_t shift = 0;
    uint32_t key = *keyPtr++;
    uint32_t *outPtr = *outPtrPtr;

    DecodeFunc decodeFn = encodeType == stdEncode ? svb_decode_data_1234 : svb_decode_data_0124;

    for (uint32_t c = 0; c < count; c++)
    {
        if (shift == 8)
        {
            shift = 0;
            key = *keyPtr++;
        }
        uint32_t val = decodeFn(&dataPtr, (key >> shift) & 0x3);
        val += prev;
        *outPtr++ = val;
        prev = val;
        shift += 2;
    }

    *outPtrPtr = outPtr;

    return dataPtr; // pointer to first unused byte after end
}