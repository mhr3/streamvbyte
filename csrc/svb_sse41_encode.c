#include <stddef.h>
#include <x86intrin.h>

#include "svb_tables_encode.h"
#include "svb_tables_decode.h"

static inline uint16_t svb_control_sse41(__m128i lo, __m128i hi)
{
    const __m128i mask_01 = _mm_set1_epi8(0x01);
    const __m128i mask_7F00 = _mm_set1_epi16(0x7F00);

    __m128i m0, m1;
    uint16_t keys;

    m0 = _mm_min_epu8(mask_01, lo);
    m1 = _mm_min_epu8(mask_01, hi);
    m0 = _mm_packus_epi16(m0, m1);
    m0 = _mm_min_epi16(m0, mask_01);    // convert 0x01FF to 0x0101
    m0 = _mm_adds_epu16(m0, mask_7F00); // convert: 0x0101 to 0x8001, 0xFF01 to 0xFFFF
    keys = (uint16_t)_mm_movemask_epi8(m0);
    return keys;
}

static inline size_t svb_encode_u256_sse4(__m128i r0, __m128i r1, uint8_t *__restrict__ dataPtr, uint16_t *__restrict__ outCode)
{
    uint8_t l0, l1;
    uint16_t keys = svb_control_sse41(r0, r1);

    __m128i t0 = _mm_loadu_si128((const __m128i *)&shuf_lut[(keys << 4) & 0x03F0]);
    __m128i t1 = _mm_loadu_si128((const __m128i *)&shuf_lut[(keys >> 4) & 0x03F0]);
    r0 = _mm_shuffle_epi8(r0, t0);
    r1 = _mm_shuffle_epi8(r1, t1);

    _mm_storeu_si128((__m128i *)dataPtr, r0);
    l0 = lengthTable_1234[keys & 0xFF];
    _mm_storeu_si128((__m128i *)(dataPtr + l0), r1);
    l1 = lengthTable_1234[keys >> 8];

    *outCode = keys;

    return l0 + l1;
}

static inline size_t svb_encode_quad_alt(const __m128i data, uint8_t *__restrict__ outData, uint8_t *__restrict__ outCode)
{
    const __m128i Ones = _mm_set1_epi32(0x01010101);
    const __m128i GatherBits = _mm_set1_epi32(0x08040102);
    const __m128i CodeTable = _mm_set_epi32(0x03030303, 0x03030303, 0x03030303, 0x02020100);
    const __m128i GatherBytes = _mm_set_epi32(0, 0, 0x0D090501, 0x0D090501);
    const __m128i Aggregators = _mm_set_epi32(0, 0, 0x01010101, 0x10400104);

    __m128i m0, m1;
    m0 = _mm_min_epu8(data, Ones);          // set byte to 1 if it is not zero
    m0 = _mm_madd_epi16(m0, GatherBits);    // gather bits 8,16,24 to bits 8,9,10
    m1 = _mm_shuffle_epi8(CodeTable, m0);   // translate to a 2-bit encoded symbol
    m1 = _mm_shuffle_epi8(m1, GatherBytes); // gather bytes holding symbols; 2 copies
    m1 = _mm_madd_epi16(m1, Aggregators);   // sum dword_1, pack dword_0

    size_t code = (size_t)_mm_extract_epi8(m1, 1);
    size_t length = lengthTable_0124[code];

    __m128i *shuf = (__m128i *)(((uint8_t *)encodingShuffleTable_0124) + code * 16);
    __m128i out = _mm_shuffle_epi8(data, _mm_loadu_si128(shuf)); // todo: aligned access

    _mm_storeu_si128((__m128i *)outData, out);
    *outCode = (uint8_t)code;
    return length;
}

static inline __m128i svb_zigzag_encode_sse4(__m128i val)
{
    // SSE4 for: (val + val) ^ (uint32_t)((int32_t)val >> 31)
    __m128i doubled = _mm_slli_epi32(val, 1);

    // arithmetic right shift by 31 bits to get the sign extended
    __m128i sign = _mm_srai_epi32(val, 31);

    // XOR
    return _mm_xor_si128(doubled, sign);
}

static inline __m128i svb_differences(__m128i curr, __m128i prev)
{
    return _mm_sub_epi32(curr, _mm_alignr_epi8(curr, prev, 12));
}