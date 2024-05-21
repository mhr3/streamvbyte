#include <stdint.h>
#include <stddef.h>

static inline uint32_t svb_zigzag_encode_32(uint32_t val)
{
  return (val + val) ^ (uint32_t)((int32_t)val >> 31);
}

// gocc: zigzag_encode(in *int32, out *uint32, sz int64)
void zigzag_encode(const int32_t *in, uint32_t *out, size_t N)
{
  for (size_t i = 0; i < N; i++)
    out[i] = svb_zigzag_encode_32((uint32_t)in[i]);
}

// gocc: zigzag_delta_encode(in *int32, out *uint32, sz int64, prev int32)
void zigzag_delta_encode(const int32_t *in, uint32_t *out, size_t N, int32_t prev)
{
  for (size_t i = 0; i < N; i++)
  {
    out[i] = svb_zigzag_encode_32((uint32_t)(in[i] - prev));
    prev = in[i];
  }
}

static inline int32_t svb_zigzag_decode_32(uint32_t val)
{
  return (val >> 1) ^ (0 - (val & 1));
}

// gocc: zigzag_decode(in *uint32, out *int32, sz int64)
void zigzag_decode(const uint32_t *in, int32_t *out, size_t N)
{
  for (size_t i = 0; i < N; i++)
    out[i] = svb_zigzag_decode_32(in[i]);
}

// gocc: zigzag_delta_decode(in *uint32, out *int32, sz int64, prev int32)
void zigzag_delta_decode(const uint32_t *in, int32_t *out, size_t N, int32_t prev)
{
  for (size_t i = 0; i < N; i++)
  {
    int32_t val = svb_zigzag_decode_32(in[i]);
    out[i] = val + prev;
    prev += val;
  }
}