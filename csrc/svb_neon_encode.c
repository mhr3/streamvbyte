#include <stddef.h>
#include <arm_neon.h>

#include "svb_tables_encode.h"

static const uint8_t pgatherlo[] = {12, 8, 4, 0, 12, 8, 4, 0}; // apparently only used in svb_encode_quad
static const uint8_t pAltLaneCodes[] = {3, 3, 2, 1, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
#define concat (1 | 1 << 10 | 1 << 20 | 1 << 30)
#define sum (1 | 1 << 8 | 1 << 16 | 1 << 24)
static const uint32_t pAggregators[2] = {concat, sum}; // apparently only used in svb_encode_quad

static inline size_t svb_encode_quad(const uint32x4_t data, uint8_t *__restrict__ outData, uint8_t *__restrict__ outCode)
{
    const uint8x8_t gatherlo = vld1_u8(pgatherlo);
    const uint32x2_t aggregators = vld1_u32(pAggregators);

    // lane code is: 3 - [saturating sub] (clz(data)/8)
    uint32x4_t clzbytes = vshrq_n_u32(vclzq_u32(data), 3);
    uint32x4_t lanecodes = vqsubq_u32(vdupq_n_u32(3), clzbytes);
    uint8x8_t lobytes = vqtbl1_u8(vreinterpretq_u8_u32(lanecodes), gatherlo);
    uint32x2_t mulshift = vreinterpret_u32_u8(lobytes);  // [0x(l3 l2 l1 l0), 0x(l3 l2 l1 l0)]

    uint32_t codeAndLength[2];
    vst1_u32(codeAndLength, vmul_u32(mulshift, aggregators));

    uint32_t code = codeAndLength[0] >> 24;
    size_t length = 4 + (codeAndLength[1] >> 24);

    // shuffle in 8-byte chunks
    uint8x16_t databytes = vreinterpretq_u8_u32(data);
    uint8x16_t encodingShuffle = vld1q_u8((uint8_t *)&shuf_lut[(code << 4) & 0x03F0]);
    vst1q_u8(outData, vqtbl1q_u8(databytes, encodingShuffle));

    *outCode = (uint8_t)code;
    return length;
}

static inline size_t svb_encode_quad_alt(const uint32x4_t data, uint8_t *__restrict__ outData, uint8_t *__restrict__ outCode)
{
    const uint8x8_t gatherlo = vld1_u8(pgatherlo);
    const uint8x16_t lanecodesTbl = vld1q_u8(pAltLaneCodes);

    // lane code is: min(4 - (clz(data)/8)), 3)
    uint32x4_t clzbytes = vshrq_n_u32(vclzq_u32(data), 3);      // 0 -> 4, <256 -> 3, <65536 -> 2, <16777216 -> 1, else 0
    //uint32x4_t lanecodes = vsubq_u32(vdupq_n_u32(4), clzbytes); // 4 -> 0, 3 -> 1, 2 -> 2, 1 -> 3, 0 -> 4
    //lanecodes = vminq_u32(lanecodes, vdupq_n_u32(3));
    // using a table saves one instruction
    uint8x16_t lanecodes = vqtbl1q_u8(lanecodesTbl, vreinterpretq_u8_u32(clzbytes));
    uint8x8_t lobytes = vqtbl1_u8(lanecodes, gatherlo);
    uint32x2_t mulshift = vreinterpret_u32_u8(lobytes); // [0x(l3 l2 l1 l0), 0x(l3 l2 l1 l0)]

    uint32_t codes32 = vget_lane_u32(mulshift, 0);
    codes32 = codes32 * concat;
    uint8_t code = (uint8_t)(codes32 >> 24);

    *outCode = code;
    if (code == 0) return 0;

    // no multiplication trick here, use the table
    size_t length = lengthTable_0124[code];

    // shuffle in 8-byte chunks
    uint8x16_t databytes = vreinterpretq_u8_u32(data);
    uint8x16_t encodingShuffle = vld1q_u8((uint8_t *)&encodingShuffleTable_0124[code]);
    vst1q_u8(outData, vqtbl1q_u8(databytes, encodingShuffle));

    return length;
}

static inline uint32x4_t svb_differences(uint32x4_t curr, uint32x4_t prev)
{
    return vsubq_u32(curr, vextq_u32(prev, curr, 3));
}