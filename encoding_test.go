package streamvbyte

import (
	"math"
	"math/rand"
	"slices"
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
				n := encodeScalar(scalar_encoded, input, encodingScheme1234)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})

			t.Run("zigzag", func(t *testing.T) {
				inputSigned := make([]int32, len(input))
				for i, v := range input {
					inputSigned[i] = int32(v)
				}

				// Encode
				encoded := ZigZagEncoding.Encode(inputSigned, nil)
				decoded := ZigZagEncoding.Decode(encoded, len(input), nil)

				require.Len(t, decoded, len(input))
				assert.Equal(t, inputSigned, decoded)

				// re-check against the scalar implementation
				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				input = ZigZag.Encode(inputSigned, nil)
				n := encodeScalar(scalar_encoded, input, encodingScheme1234)
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
				n := encodeScalar(scalar_encoded, input, encodingScheme0124)
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
				n := encodeDeltaScalar(scalar_encoded, input, 0, encodingScheme1234)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})

			t.Run("zigzag", func(t *testing.T) {
				inputSigned := make([]int32, len(input))
				for i, v := range input {
					inputSigned[i] = int32(v)
				}

				// Encode
				encoded := ZigZagEncoding.EncodeDelta(inputSigned, nil, 0)
				decoded := ZigZagEncoding.DecodeDelta(encoded, len(input), nil, 0)

				require.Len(t, decoded, len(input))
				assert.Equal(t, inputSigned, decoded)

				// re-check against the scalar implementation
				scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
				input = ZigZag.EncodeDelta(inputSigned, nil, 0)
				n := encodeScalar(scalar_encoded, input, encodingScheme1234)
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
				n := encodeDeltaScalar(scalar_encoded, input, 0, encodingScheme0124)
				scalar_encoded = scalar_encoded[:n]
				assert.Equal(t, scalar_encoded, encoded)
			})
		})
	}
}

func TestLargeDeltas(t *testing.T) {
	input := []uint32{0, 42, math.MaxUint32, 42, 0, 42, 0, 42, 0, 42, 0, 42}
	encoded := StdEncoding.EncodeDelta(input, nil, 0)
	decoded := StdEncoding.DecodeDelta(encoded, len(input), nil, 0)

	require.Len(t, decoded, len(input))
	assert.Equal(t, input, decoded)

	sInput := []int32{0, 42, -1, 42, 0, 42, 0, 42, 0, 42, 0, 42}
	inputZz := ZigZag.EncodeDelta(sInput, nil, 0)
	encoded = StdEncoding.Encode(inputZz, nil)
	decoded = StdEncoding.Decode(encoded, len(input), nil)
	outputZz := ZigZag.DecodeDelta(decoded, nil, 0)

	require.Len(t, decoded, len(input))
	assert.Equal(t, sInput, outputZz)
}

const (
	benchSize = 100000
	zipfV     = 1.0
	zipfS     = 1.1
)

var (
	benchUint32Data       = make([]uint32, benchSize)
	benchUint32DataSorted = make([]uint32, benchSize)
	benchInt32Data        = make([]int32, benchSize)
)

func init() {
	seed := int64(42)
	zipf := rand.NewZipf(rand.New(rand.NewSource(seed)), zipfS, zipfV, math.MaxUint32)
	for i := range benchUint32Data {
		randUint32 := uint32(zipf.Uint64())
		benchUint32Data[i] = randUint32
		benchInt32Data[i] = int32((randUint32 >> 1) ^ -(randUint32 & 1))
	}

	copy(benchUint32DataSorted, benchUint32Data)
	slices.Sort(benchUint32DataSorted)
}

func BenchmarkEncode(b *testing.B) {
	var encoded []byte

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = Encode(benchUint32Data, &EncodeOptions{Output: encoded})
			_ = encoded
		}
	})

	b.Run("zigzag", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = Encode(benchInt32Data, &EncodeOptions{Output: encoded})
			_ = encoded
		}
	})

	b.Run("zigzag-two-step", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		var zzData []uint32
		for i := 0; i < b.N; i++ {
			zzData = ZigZag.Encode(benchInt32Data, zzData)
			encoded = Encode(zzData, &EncodeOptions{Output: encoded})
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
			encoded = EncodeDelta(benchUint32DataSorted, &EncodeOptions{Output: encoded})
			_ = encoded
		}
	})

	b.Run("zigzag", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = EncodeDelta(benchInt32Data, &EncodeOptions{Output: encoded})
			_ = encoded
		}
	})

	b.Run("zigzag-two-step", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		var zzData []uint32
		for i := 0; i < b.N; i++ {
			zzData = ZigZag.EncodeDelta(benchInt32Data, zzData, 0)
			encoded = Encode(zzData, &EncodeOptions{Output: encoded})
			_ = encoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			encoded = EncodeDelta(benchUint32DataSorted, &EncodeOptions{Output: encoded, Scheme: Scheme0124})
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
			decoded = Decode(encoded, len(benchUint32Data), &DecodeOptions[uint32]{Output: decoded})
			_ = decoded
		}
	})

	b.Run("zigzag", func(b *testing.B) {
		var decodedInt32 []int32
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decodedInt32 = Decode(encoded, len(benchUint32Data), &DecodeOptions[int32]{Output: decodedInt32})
			_ = decodedInt32
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = Decode(encoded, len(benchUint32Data), &DecodeOptions[uint32]{Output: decoded, Scheme: Scheme0124})
			_ = decoded
		}
	})
}

func BenchmarkDecodeDelta(b *testing.B) {
	encoded := EncodeDelta(benchUint32DataSorted, nil)

	var decoded []uint32

	b.Run("std", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = DecodeDelta(encoded, len(benchUint32DataSorted), &DecodeOptions[uint32]{Output: decoded})
			_ = decoded
		}
	})

	b.Run("alt", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		for i := 0; i < b.N; i++ {
			decoded = DecodeDelta(encoded, len(benchUint32DataSorted), &DecodeOptions[uint32]{Output: decoded, Scheme: Scheme0124})
			_ = decoded
		}
	})
}
