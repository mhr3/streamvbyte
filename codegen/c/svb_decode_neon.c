#include <stddef.h>
#include <arm_neon.h>

#include "svb_tables_1234_dec.h"

#include "svb_scalar_dec.c"

static inline uint32x4_t svb_decode_quad(uint8_t key, const uint8_t **dataPtrPtr)
{
  uint8_t len;
  uint8_t *pshuf = (uint8_t *)&shuffleTable[key];
  uint8x16_t decodingShuffle = vld1q_u8(pshuf);
  uint8x16_t compressed = vld1q_u8(*dataPtrPtr);

#ifdef AVOIDLENGTHLOOKUP
  // this avoids the dependency on lengthTable,
  // see https://github.com/lemire/streamvbyte/issues/12
  len = pshuf[12 + (key >> 6)] + 1;
#else
  len = lengthTable[key];
#endif

  *dataPtrPtr += len;

  return vreinterpretq_u32_u8(vqtbl1q_u8(compressed, decodingShuffle));
}

// gocc: svb_decode(in []byte, count int, out *uint32) uint64
uint64_t svb_decode(const uint8_t *in, const uint64_t in_len, uint64_t in_cap,
                    int64_t count, uint32_t *out)
{
  if (count <= 0 || in_len < (count + 3) / 4) return 0;

  const uint8_t *dataStartPtr = &in[(count + 3) / 4];
  const uint8_t *dataEndPtr = in + in_len;
  const uint8_t *dataNeonBound = in + (in_len - (in_len % 16));
  uint8_t *keyPtr = (uint8_t*)in;
  const uint32_t *outStartPtr = out;
  const uint8_t *currPtr = dataStartPtr;

  for (const uint8_t *keyBoundPtr = in + (count / 4); keyPtr < keyBoundPtr && currPtr < dataNeonBound; keyPtr++)
  {
    uint8_t key = *keyPtr;
    uint32x4_t data = svb_decode_quad(key, &currPtr);
    vst1q_u32((uint32_t *)out, data);

    out += 4; // 16-byte shift
    count -= 4;
  }

  currPtr = svb_decode_scalar(&out, keyPtr, currPtr, count);
  if (currPtr == NULL) return 0;

  return (uint64_t)(out - outStartPtr);
}

// for encoding
static inline uint32x4_t svb_delta(uint32x4_t curr, uint32x4_t prev)
{
  return vsubq_u32(curr, vextq_u32(prev, curr, 3));
}

static inline uint32x4_t svb_prefix_sum(uint32x4_t curr, uint32x4_t prev)
{
  uint32x4_t zero = {0, 0, 0, 0};
  uint32x4_t add = vextq_u32(zero, curr, 3);
  uint8x16_t BroadcastLast = {12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15};
  prev = vreinterpretq_u32_u8(vqtbl1q_u8(vreinterpretq_u8_u32(prev), BroadcastLast));
  curr = vaddq_u32(curr, add);
  add = vextq_u32(zero, curr, 2);
  curr = vaddq_u32(curr, prev);
  curr = vaddq_u32(curr, add);
  return curr;
}

// gocc: svb_delta_decode_vector(out []uint32, in []uint8, prev uint32) uint64
uint64_t svb_delta_decode_vector(const uint32_t *out, const uint64_t out_len, const uint64_t out_cap,
                                 const uint8_t *in, const uint64_t in_len, uint64_t in_cap, uint32_t prev)
{
  const uint64_t count = out_len;
  const uint8_t *dataPtr = &in[(count + 3) / 4];
  const uint8_t *currPtr = dataPtr;

  uint64_t keybytes = count / 4; // number of key bytes
  uint32x4_t previous = vdupq_n_u32(prev);
  uint32x4_t data;

  for (uint64_t i = 0; i < keybytes; i++)
  {
    uint8_t key = in[i];
    data = svb_decode_quad(key, &currPtr);
    previous = svb_prefix_sum(data, previous);
    vst1q_u32((uint32_t *)out, previous);
    out += 4;
  }

  return (uint64_t)(currPtr - dataPtr);
}