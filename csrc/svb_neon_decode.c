#include <stddef.h>
#include <arm_neon.h>

#include "svb_tables_decode.h"

static inline uint32x4_t svb_decode_quad(uint8_t key, const uint8_t **dataPtrPtr)
{
    uint8_t len;
    uint8_t *pshuf = (uint8_t *)&shuffleTable_1234[key];
    uint8x16_t decodingShuffle = vld1q_u8(pshuf);
    uint8x16_t compressed = vld1q_u8(*dataPtrPtr);

#ifdef AVOIDLENGTHLOOKUP
    // this avoids the dependency on lengthTable,
    // see https://github.com/lemire/streamvbyte/issues/12
    len = pshuf[12 + (key >> 6)] + 1;
#else
    len = lengthTable_1234[key];
#endif

    *dataPtrPtr += len;

    return vreinterpretq_u32_u8(vqtbl1q_u8(compressed, decodingShuffle));
}

static inline uint32x4_t svb_decode_quad_alt(uint8_t key, const uint8_t **dataPtrPtr)
{
    uint8_t len = lengthTable_0124[key];
    uint8_t *pshuf = (uint8_t *)&shuffleTable_0124[key];
    uint8x16_t decodingShuffle = vld1q_u8(pshuf);
    uint8x16_t compressed = vld1q_u8(*dataPtrPtr);

    *dataPtrPtr += len;

    return vreinterpretq_u32_u8(vqtbl1q_u8(compressed, decodingShuffle));
}

static inline uint32x4_t svb_prefix_sum(uint32x4_t curr, uint32x4_t prev)
{
    uint32x4_t zero = vdupq_n_u32(0);
    uint32x4_t add = vextq_u32(zero, curr, 3);

    prev = vdupq_laneq_u32(prev, 3);
    curr = vaddq_u32(curr, add);
    add = vextq_u32(zero, curr, 2);
    curr = vaddq_u32(curr, prev);
    curr = vaddq_u32(curr, add);
    return curr;
}