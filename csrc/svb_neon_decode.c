#include <stddef.h>
#include <arm_neon.h>

#include "svb_tables_decode.h"

static inline uint8_t length_1234_for_key(uint8_t key)
{
    uint8_t len = 4 + (key & 3);
    len += (key / 4) & 3;
    len += (key / 16) & 3;
    len += (key / 64) & 3;
    return len;
}

static uint8_t lt_base_1234[16] = {0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6};

static uint8_t lt_base_0124[16] = {0, 1, 2, 4, 1, 2, 3, 5, 2, 3, 4, 6, 4, 5, 6, 8};

static inline uint8x8_t length_1234_for_keys(uint8x8_t keys)
{
    const uint8x16_t table = vld1q_u8(lt_base_1234);

    uint8x8_t t1 = vqtbl1_u8(table, vand_u8(keys, vdup_n_u8(15)));
    uint8x8_t t2 = vqtbl1_u8(table, vshr_n_u8(keys, 4));

    uint8x8_t lengths = vadd_u8(t1, t2);
    return vadd_u8(lengths, vdup_n_u8(4));
    /*
        const uint8x8_t threes = vdup_n_u8(3);
        uint8x8_t lengths;

        lengths = vadd_u8(vdup_n_u8(4), vand_u8(keys, threes));
        keys = vshr_n_u8(keys, 2);
        lengths = vadd_u8(lengths, vand_u8(keys, threes));
        keys = vshr_n_u8(keys, 2);
        lengths = vadd_u8(lengths, vand_u8(keys, threes));
        keys = vshr_n_u8(keys, 2);
        lengths = vadd_u8(lengths, vand_u8(keys, threes));
        return lengths;
    */
}

static inline uint8_t length_0124_for_key(uint8_t key)
{
    uint8_t len = (1 << (key & 3)) / 2;
    len += (1 << ((key / 4) & 3)) / 2;
    len += (1 << ((key / 16) & 3)) / 2;
    len += (1 << ((key / 64) & 3)) / 2;
    return len;
}

static inline uint8x8_t length_0124_for_keys(uint8x8_t keys)
{
    const uint8x16_t table = vld1q_u8(lt_base_0124);

    uint8x8_t t1 = vqtbl1_u8(table, vand_u8(keys, vdup_n_u8(15)));
    uint8x8_t t2 = vqtbl1_u8(table, vshr_n_u8(keys, 4));

    return vadd_u8(t1, t2);
/*
    const uint8x8_t ones = vdup_n_u8(1);
    const uint8x8_t threes = vdup_n_u8(3);
    uint8x8_t lengths;

    lengths = vshr_n_u8(vshl_u8(ones, vand_u8(keys, threes)), 1);
    keys = vshr_n_u8(keys, 2);
    lengths = vadd_u8(lengths, vshr_n_u8(vshl_u8(ones, vand_u8(keys, threes)), 1));
    keys = vshr_n_u8(keys, 2);
    lengths = vadd_u8(lengths, vshr_n_u8(vshl_u8(ones, vand_u8(keys, threes)), 1));
    keys = vshr_n_u8(keys, 2);
    lengths = vadd_u8(lengths, vshr_n_u8(vshl_u8(ones, vand_u8(keys, threes)), 1));
    return lengths;
*/
}

static inline uint32x4_t svb_decode_quad(uint8_t key, const uint8_t **dataPtrPtr)
{
#ifdef AVOIDLENGTHLOOKUP
    // this avoids the dependency on lengthTable,
    // see https://github.com/lemire/streamvbyte/issues/12
    uint8_t len = pshuf[12 + (key >> 6)] + 1;
#else
    uint8_t len = lengthTable_1234[key];
#endif
    uint8_t *pshuf = (uint8_t *)&shuffleTable_1234[key];
    uint8x16_t decodingShuffle = vld1q_u8(pshuf);
    uint8x16_t compressed = vld1q_u8(*dataPtrPtr);

    *dataPtrPtr += len;

    return vreinterpretq_u32_u8(vqtbl1q_u8(compressed, decodingShuffle));
}

static inline uint32x4_t svb_decode_quad_lite(uint8_t key, const uint8_t *dataPtrPtr)
{
    uint8_t *pshuf = (uint8_t *)&shuffleTable_1234[key];
    uint8x16_t decodingShuffle = vld1q_u8(pshuf);
    uint8x16_t compressed = vld1q_u8(dataPtrPtr);

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

static inline uint32x4_t svb_decode_quad_alt_lite(uint8_t key, const uint8_t *dataPtrPtr)
{
    uint8_t *pshuf = (uint8_t *)&shuffleTable_0124[key];
    uint8x16_t decodingShuffle = vld1q_u8(pshuf);
    uint8x16_t compressed = vld1q_u8(dataPtrPtr);

    return vreinterpretq_u32_u8(vqtbl1q_u8(compressed, decodingShuffle));
}

static inline int32x4_t svb_zigzag_decode_neon(const uint32x4_t data)
{
    // NEON for: (val >> 1) ^ (0 - (val & 1));
    uint32x4_t half = vshrq_n_u32(data, 1);
    uint32x4_t mask = vandq_u32(data, vdupq_n_u32(1));
    int32x4_t neg_mask = vnegq_s32(vreinterpretq_s32_u32(mask));

    // XOR to complete zigzag
    return veorq_s32(vreinterpretq_s32_u32(half), neg_mask);
}

static inline uint32x4_t svb_prefix_sum_u32(uint32x4_t curr, uint32x4_t prev)
{
    const uint32x4_t zero = vdupq_n_u32(0);
    uint32x4_t add = vextq_u32(zero, curr, 3);

    prev = vdupq_laneq_u32(prev, 3);
    curr = vaddq_u32(curr, add);
    add = vextq_u32(zero, curr, 2);
    curr = vaddq_u32(curr, prev);
    curr = vaddq_u32(curr, add);
    return curr;
}

static inline int32x4_t svb_prefix_sum_s32(int32x4_t curr, int32x4_t prev)
{
    const int32x4_t zero = vdupq_n_s32(0);
    int32x4_t add = vextq_s32(zero, curr, 3);

    prev = vdupq_laneq_s32(prev, 3);
    curr = vaddq_s32(curr, add);
    add = vextq_s32(zero, curr, 2);
    curr = vaddq_s32(curr, prev);
    curr = vaddq_s32(curr, add);
    return curr;
}

static inline uint32x4_t svb_write_u32_delta(uint32_t *out, uint32x4_t vec, uint32x4_t prev)
{
    vec = svb_prefix_sum_u32(vec, prev);
    vst1q_u32(out, vec);
    return vec;
}

static inline int32x4_t svb_write_s32_delta(int32_t *out, int32x4_t vec, int32x4_t prev)
{
    vec = svb_prefix_sum_s32(vec, prev);
    vst1q_s32(out, vec);
    return vec;
}