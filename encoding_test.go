package streamvbyte_test

import (
	"math"
	"math/rand"
	"sort"
	"testing"

	"github.com/mhr3/streamvbyte"

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
			// Encode
			encoded := streamvbyte.StdEncoding.Encode(input, nil)
			decoded := streamvbyte.StdEncoding.Decode(encoded, len(input), nil)

			require.Len(t, decoded, len(input))
			assert.Equal(t, input, decoded)

			// AltEncoding
			encoded = streamvbyte.AltEncoding.Encode(input, nil)
			decoded = streamvbyte.AltEncoding.Decode(encoded, len(input), nil)

			require.Len(t, decoded, len(input))
			assert.Equal(t, input, decoded)
		})

		t.Run("delta-"+tc.name, func(t *testing.T) {
			input := tc.generator()
			// Encode
			encoded := streamvbyte.StdEncoding.EncodeDelta(input, nil, 0)
			decoded := streamvbyte.StdEncoding.DecodeDelta(encoded, len(input), nil, 0)

			require.Len(t, decoded, len(input))
			assert.Equal(t, input, decoded)

			// AltEncoding
			encoded = streamvbyte.AltEncoding.EncodeDelta(input, nil, 0)
			decoded = streamvbyte.AltEncoding.DecodeDelta(encoded, len(input), nil, 0)

			require.Len(t, decoded, len(input))
			assert.Equal(t, input, decoded)
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

	for i := 0; i < b.N; i++ {
		encoded = streamvbyte.StdEncoding.Encode(benchUint32Data, encoded)
		_ = encoded
	}

	b.SetBytes(int64(4 * benchSize))
}

func BenchmarkDeltaEncode(b *testing.B) {
	var encoded []byte

	for i := 0; i < b.N; i++ {
		encoded = streamvbyte.StdEncoding.EncodeDelta(benchUint32DataSorted, encoded, 0)
		_ = encoded
	}

	b.SetBytes(int64(4 * benchSize))
}

func BenchmarkDecode(b *testing.B) {
	b.SetBytes(int64(4 * benchSize))
	encoded := streamvbyte.StdEncoding.Encode(benchUint32Data, nil)

	b.ResetTimer()
	var decoded []uint32
	for i := 0; i < b.N; i++ {
		decoded = streamvbyte.StdEncoding.Decode(encoded, len(benchUint32Data), decoded)
		_ = decoded
	}
}

func BenchmarkDeltaDecode(b *testing.B) {
	b.SetBytes(int64(4 * benchSize))
	encoded := streamvbyte.StdEncoding.EncodeDelta(benchUint32DataSorted, nil, 0)

	b.ResetTimer()
	var decoded []uint32
	for i := 0; i < b.N; i++ {
		decoded = streamvbyte.StdEncoding.DecodeDelta(encoded, len(benchUint32DataSorted), decoded, 0)
		_ = decoded
	}
}
