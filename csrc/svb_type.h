#ifndef SVB_TYPE_ENUM_H
#define SVB_TYPE_ENUM_H

typedef enum
{
    stdEncode,
    altEncode
} EncodeType;

typedef uint32_t (*DecodeFunc)(const uint8_t **, uint8_t);
typedef uint8_t (*EncodeFunc)(uint32_t, uint8_t *__restrict__ *);

#endif