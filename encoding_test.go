package streamvbyte

import (
	"math"
	"math/rand"
	"sort"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestEncodeDecode(t *testing.T) {
	const sliceSize = 10000

	testCases := []struct {
		name      string
		generator func() []uint32
	}{
		{name: "zeroes", generator: func() []uint32 {
			return make([]uint32, sliceSize)
		}},
		{name: "<=1bit", generator: func() []uint32 {
			input := make([]uint32, sliceSize)
			for i := range input {
				input[i] = rand.Uint32() % (1 << 1)
			}
			return input
		}},
		{name: "<=9bit", generator: func() []uint32 {
			input := make([]uint32, sliceSize)
			for i := range input {
				input[i] = rand.Uint32() % (1 << 9)
			}
			return input
		}},
		{name: "<=17bits", generator: func() []uint32 {
			input := make([]uint32, sliceSize)
			for i := range input {
				input[i] = rand.Uint32() % (1 << 17)
			}
			return input
		}},
		{name: "<=25bits", generator: func() []uint32 {
			input := make([]uint32, sliceSize)
			for i := range input {
				input[i] = rand.Uint32() % (1 << 25)
			}
			return input
		}},
		{name: "<=32bits", generator: func() []uint32 {
			input := make([]uint32, sliceSize)
			for i := range input {
				input[i] = rand.Uint32()
			}
			return input
		}},
	}

	for _, tc := range testCases {
		t.Run(tc.name, func(t *testing.T) {
			input := tc.generator()

			t.Run("std", func(t *testing.T) {
				// Encode
				encoded := StdEncoding.Encode(input, nil)
				decoded := StdEncoding.Decode(encoded, len(input), nil)

				require.Len(t, decoded, len(input))
				assert.Equal(t, input, decoded)

				// re-check against the scalar implementation
				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				n := encodeScalar1234(scalar_encoded, input)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})

			t.Run("alt", func(t *testing.T) {
				// AltEncoding
				encoded := AltEncoding.Encode(input, nil)
				decoded := AltEncoding.Decode(encoded, len(input), nil)

				require.Len(t, decoded, len(input))
				assert.Equal(t, input, decoded)

				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				n := encodeScalar0124(scalar_encoded, input)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})
		})

		t.Run("delta-"+tc.name, func(t *testing.T) {
			input := tc.generator()

			t.Run("std", func(t *testing.T) {
				// Encode
				encoded := StdEncoding.EncodeDelta(input, nil, 0)
				decoded := StdEncoding.DecodeDelta(encoded, len(input), nil, 0)

				require.Len(t, decoded, len(input))
				assert.Equal(t, input, decoded)

				// re-check against the scalar implementation
				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				n := encodeDeltaScalar1234(scalar_encoded, input, 0)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})

			t.Run("alt", func(t *testing.T) {
				// AltEncoding
				encoded := AltEncoding.EncodeDelta(input, nil, 0)
				decoded := AltEncoding.DecodeDelta(encoded, len(input), nil, 0)

				require.Len(t, decoded, len(input))
				assert.Equal(t, input, decoded)

				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				n := encodeDeltaScalar0124(scalar_encoded, input, 0)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})
		})
	}
}

const (
	benchSize = 1000000
	zipfV     = 1.0
	zipfS     = 1.1
)

var (
	benchUint32Data       = make([]uint32, benchSize)
	benchUint32DataSorted = make([]uint32, benchSize)
)

func init() {
	seed := int64(42)
	zipf := rand.NewZipf(rand.New(rand.NewSource(seed)), zipfS, zipfV, math.MaxUint32)
	for i := range benchUint32Data {
		randUint32 := uint32(zipf.Uint64())
		benchUint32Data[i] = randUint32
		//benchInt32Data[i] = int32((randUint32 >> 1) ^ -(randUint32 & 1))
	}

	copy(benchUint32DataSorted, benchUint32Data)
	sort.Slice(benchUint32DataSorted, func(i, j int) bool { return benchUint32DataSorted[i] < benchUint32DataSorted[j] })
}

func BenchmarkEncode(b *testing.B) {
	var encoded []byte

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = StdEncoding.Encode(benchUint32Data, encoded)
			_ = encoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = AltEncoding.Encode(benchUint32Data, encoded)
			_ = encoded
		}
	})
}

func BenchmarkEncodeDelta(b *testing.B) {
	var encoded []byte

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = StdEncoding.EncodeDelta(benchUint32DataSorted, encoded, 0)
			_ = encoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = AltEncoding.EncodeDelta(benchUint32DataSorted, encoded, 0)
			_ = encoded
		}
	})
}

func BenchmarkDecode(b *testing.B) {
	encoded := StdEncoding.Encode(benchUint32Data, nil)

	var decoded []uint32

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = StdEncoding.Decode(encoded, len(benchUint32Data), decoded)
			_ = decoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = AltEncoding.Decode(encoded, len(benchUint32Data), decoded)
			_ = decoded
		}
	})
}

func BenchmarkDecodeDelta(b *testing.B) {
	encoded := StdEncoding.EncodeDelta(benchUint32DataSorted, nil, 0)

	var decoded []uint32

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = StdEncoding.DecodeDelta(encoded, len(benchUint32DataSorted), decoded, 0)
			_ = decoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = AltEncoding.DecodeDelta(encoded, len(benchUint32DataSorted), decoded, 0)
			_ = decoded
		}
	})
}
