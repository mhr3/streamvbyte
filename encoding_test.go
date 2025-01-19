package streamvbyte

import (
	"encoding/binary"
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
		t.Run("direct-"+tc.name, func(t *testing.T) {
			input := tc.generator()

			t.Run("uint32", func(t *testing.T) {
				for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
					t.Run(scheme.String(), func(t *testing.T) {
						// Encode
						encoded := EncodeUint32(input, &EncodeOptionsNew[uint32]{Scheme: scheme})
						decoded := DecodeUint32(encoded, len(input), &DecodeOptionsNew[uint32]{Scheme: scheme})

						require.Len(t, decoded, len(input))
						assert.Equal(t, input, decoded)

						// re-check against the scalar implementation
						scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
						n := encodeScalar(scalar_encoded, input, scheme)
						scalar_encoded = scalar_encoded[:n]
						assert.Equal(t, scalar_encoded, encoded)
					})
				}
			})

			t.Run("int32", func(t *testing.T) {
				inputSigned := make([]int32, len(input))
				for i, v := range input {
					inputSigned[i] = int32(v)
				}

				for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
					t.Run(scheme.String(), func(t *testing.T) {
						// Encode
						encoded := EncodeInt32(inputSigned, &EncodeOptionsNew[int32]{Scheme: scheme})
						decoded := DecodeInt32(encoded, len(inputSigned), &DecodeOptionsNew[int32]{Scheme: scheme})

						require.Len(t, decoded, len(inputSigned))
						assert.Equal(t, inputSigned, decoded)

						// re-check against the scalar implementation
						scalar_encoded := make([]byte, MaxEncodedLen(len(inputSigned)))
						n := encodeScalarZigzag(scalar_encoded, inputSigned, scheme)
						scalar_encoded = scalar_encoded[:n]
						assert.Equal(t, scalar_encoded, encoded)
					})
				}
			})
		})

		t.Run("delta-"+tc.name, func(t *testing.T) {
			input := tc.generator()

			t.Run("uint32", func(t *testing.T) {
				for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
					t.Run(scheme.String(), func(t *testing.T) {
						// Encode
						encoded := DeltaEncodeUint32(input, &EncodeOptionsNew[uint32]{Scheme: scheme})
						decoded := DeltaDecodeUint32(encoded, len(input), &DecodeOptionsNew[uint32]{Scheme: scheme})

						require.Len(t, decoded, len(input))
						assert.Equal(t, input, decoded)

						// re-check against the scalar implementation
						scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
						n := encodeDeltaScalar(scalar_encoded, input, 0, scheme)
						scalar_encoded = scalar_encoded[:n]
						assert.Equal(t, scalar_encoded, encoded)
					})
				}
			})

			t.Run("int32", func(t *testing.T) {
				inputSigned := make([]int32, len(input))
				for i, v := range input {
					inputSigned[i] = int32(v)
				}

				for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
					t.Run(scheme.String(), func(t *testing.T) {
						// Encode
						encoded := DeltaEncodeInt32(inputSigned, &EncodeOptionsNew[int32]{Scheme: scheme})
						decoded := DeltaDecodeInt32(encoded, len(inputSigned), &DecodeOptionsNew[int32]{Scheme: scheme})

						require.Len(t, decoded, len(inputSigned))
						assert.Equal(t, inputSigned, decoded)

						// re-check against the scalar implementation
						scalar_encoded := make([]byte, MaxEncodedLen(len(inputSigned)))
						n := encodeDeltaScalarZigzag(scalar_encoded, inputSigned, 0, scheme)
						scalar_encoded = scalar_encoded[:n]
						assert.Equal(t, scalar_encoded, encoded)
					})
				}
			})
		})
	}
}

func TestLargeDeltas(t *testing.T) {
	input := []uint32{0, 42, math.MaxUint32, 42, 0, 42, 0, 42, 0, 42, 0, 42}
	encoded := DeltaEncodeUint32(input, nil)
	decoded := DeltaDecodeUint32(encoded, len(input), nil)

	require.Len(t, decoded, len(input))
	if !assert.Equal(t, input, decoded) {
		scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
		n := encodeDeltaScalar(scalar_encoded, input, 0, Scheme1234)
		scalar_encoded = scalar_encoded[:n]

		assert.Equal(t, scalar_encoded, encoded)
	}

	sInput := []int32{0, 42, -1, 42, 0, 42, 0, 42, 0, 42, 0, 42}
	encoded = EncodeInt32(sInput, nil)
	sDecoded := DecodeInt32(encoded, len(input), nil)

	require.Len(t, decoded, len(input))
	assert.Equal(t, sInput, sDecoded)
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

	b.Run("uint32", func(b *testing.B) {
		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					encoded = EncodeUint32(benchUint32Data, &EncodeOptionsNew[uint32]{Buffer: encoded, Scheme: scheme})
					_ = encoded
				}
			})
		}
	})

	b.Run("int32", func(b *testing.B) {
		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					encoded = EncodeInt32(benchInt32Data, &EncodeOptionsNew[int32]{Buffer: encoded, Scheme: scheme})
					_ = encoded
				}
			})
		}
	})

	b.Run("zigzag-two-step", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		var zzData []uint32
		for i := 0; i < b.N; i++ {
			zzData = ZigZag.Encode(benchInt32Data, zzData)
			encoded = EncodeUint32(zzData, &EncodeOptionsNew[uint32]{Buffer: encoded})
			_ = encoded
		}
	})
}

func BenchmarkEncodeDelta(b *testing.B) {
	var encoded []byte

	b.Run("uint32", func(b *testing.B) {
		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					encoded = DeltaEncodeUint32(benchUint32Data, &EncodeOptionsNew[uint32]{Buffer: encoded, Scheme: scheme})
					_ = encoded
				}
			})
		}
	})

	b.Run("int32", func(b *testing.B) {
		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					encoded = DeltaEncodeInt32(benchInt32Data, &EncodeOptionsNew[int32]{Buffer: encoded, Scheme: scheme})
					_ = encoded
				}
			})
		}
	})

	b.Run("zigzag-two-step", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		var zzData []uint32
		for i := 0; i < b.N; i++ {
			zzData = ZigZag.EncodeDelta(benchInt32Data, zzData, 0)
			encoded = EncodeUint32(zzData, &EncodeOptionsNew[uint32]{Buffer: encoded})
			_ = encoded
		}
	})
}

func BenchmarkDecode(b *testing.B) {
	encoded := EncodeUint32(benchUint32Data, nil)

	b.Run("uint32", func(b *testing.B) {
		var decoded []uint32

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					decoded = DecodeUint32(encoded, len(benchUint32Data), &DecodeOptionsNew[uint32]{Buffer: decoded, Scheme: scheme})
					_ = decoded
				}
			})
		}
	})

	b.Run("int32", func(b *testing.B) {
		var decodedInt32 []int32

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					decodedInt32 = DecodeInt32(encoded, len(benchUint32Data), &DecodeOptionsNew[int32]{Buffer: decodedInt32, Scheme: scheme})
					_ = decodedInt32
				}
			})
		}
	})

	b.Run("zigzag-two-step", func(b *testing.B) {
		b.SetBytes(int64(4 * benchSize))
		var zzData []uint32
		var decoded []int32
		for i := 0; i < b.N; i++ {
			zzData = DecodeUint32(encoded, len(benchUint32Data), &DecodeOptionsNew[uint32]{Buffer: zzData})
			decoded = ZigZag.Decode(zzData, decoded)
			_ = decoded
		}
	})
}

func BenchmarkDecodeDelta(b *testing.B) {
	encoded := DeltaEncodeUint32(benchUint32DataSorted, nil)

	b.Run("uint32", func(b *testing.B) {
		var decoded []uint32

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					decoded = DeltaDecodeUint32(encoded, len(benchUint32Data), &DecodeOptionsNew[uint32]{Buffer: decoded, Scheme: scheme})
					_ = decoded
				}
			})
		}
	})

	b.Run("int32", func(b *testing.B) {
		var decodedInt32 []int32

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			b.Run(scheme.String(), func(b *testing.B) {
				b.SetBytes(int64(4 * benchSize))
				for i := 0; i < b.N; i++ {
					decodedInt32 = DeltaDecodeInt32(encoded, len(benchUint32Data), &DecodeOptionsNew[int32]{Buffer: decodedInt32, Scheme: scheme})
					_ = decodedInt32
				}
			})
		}
	})
}

func FuzzEncodeDecode(f *testing.F) {
	byteSliceFromUint32 := func(input []uint32) []byte {
		output := make([]byte, len(input)*4)
		for i, v := range input {
			binary.LittleEndian.PutUint32(output[i*4:], v)
		}
		return output
	}

	f.Add([]byte("0"))
	f.Add(byteSliceFromUint32([]uint32{0}))
	f.Add(byteSliceFromUint32([]uint32{0x03020100}))
	f.Add(byteSliceFromUint32([]uint32{0, 1, 15, 128, 256, math.MaxInt16, math.MaxUint16 - 1, math.MaxUint16, 16777216, math.MaxInt32, math.MaxUint32}))

	f.Fuzz(func(t *testing.T, inputByteSlc []byte) {
		input := make([]uint32, len(inputByteSlc)/4)
		for i := 0; i < len(input); i++ {
			input[i] = binary.LittleEndian.Uint32(inputByteSlc[i*4:])
		}

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			encoded := EncodeUint32(input, &EncodeOptionsNew[uint32]{Scheme: scheme})
			decoded := DecodeUint32(encoded, len(input), &DecodeOptionsNew[uint32]{Scheme: scheme})

			require.Len(t, decoded, len(input))
			if len(input) == 0 && len(encoded) == 0 {
				continue
			}
			assert.Equal(t, input, decoded)

			// re-check against the scalar implementation
			scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
			n := encodeScalar(scalar_encoded, input, scheme)
			scalar_encoded = scalar_encoded[:n]
			assert.Equal(t, scalar_encoded, encoded)
		}

		inputSigned := convertSliceTo[int32](input)

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			encoded := EncodeInt32(inputSigned, &EncodeOptionsNew[int32]{Scheme: scheme})
			decoded := DecodeInt32(encoded, len(input), &DecodeOptionsNew[int32]{Scheme: scheme})

			require.Len(t, decoded, len(input))
			if len(input) == 0 && len(encoded) == 0 {
				continue
			}
			assert.Equal(t, inputSigned, decoded)

			// re-check against the scalar implementation
			scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
			n := encodeScalarZigzag(scalar_encoded, inputSigned, scheme)
			scalar_encoded = scalar_encoded[:n]
			assert.Equal(t, scalar_encoded, encoded)
		}
	})
}

func FuzzDeltaEncodeDecode(f *testing.F) {
	byteSliceFromUint32 := func(input []uint32) []byte {
		output := make([]byte, len(input)*4)
		for i, v := range input {
			binary.LittleEndian.PutUint32(output[i*4:], v)
		}
		return output
	}

	f.Add([]byte("0"))
	f.Add(byteSliceFromUint32([]uint32{0}))
	f.Add(byteSliceFromUint32([]uint32{0x03020100}))
	f.Add(byteSliceFromUint32([]uint32{0, 1, 15, 128, 256, math.MaxInt16, math.MaxUint16 - 1, math.MaxUint16, 16777216, math.MaxInt32, math.MaxUint32}))

	f.Fuzz(func(t *testing.T, inputByteSlc []byte) {
		input := make([]uint32, len(inputByteSlc)/4)
		for i := 0; i < len(input); i++ {
			input[i] = binary.LittleEndian.Uint32(inputByteSlc[i*4:])
		}

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			encoded := DeltaEncodeUint32(input, &EncodeOptionsNew[uint32]{Scheme: scheme})
			decoded := DeltaDecodeUint32(encoded, len(input), &DecodeOptionsNew[uint32]{Scheme: scheme})

			require.Len(t, decoded, len(input))
			if len(input) == 0 && len(encoded) == 0 {
				continue
			}
			assert.Equal(t, input, decoded)

			// re-check against the scalar implementation
			scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
			n := encodeDeltaScalar(scalar_encoded, input, 0, scheme)
			scalar_encoded = scalar_encoded[:n]
			assert.Equal(t, scalar_encoded, encoded)
		}

		inputSigned := convertSliceTo[int32](input)

		for _, scheme := range []Scheme{Scheme1234, Scheme0124} {
			encoded := DeltaEncodeInt32(inputSigned, &EncodeOptionsNew[int32]{Scheme: scheme})
			decoded := DeltaDecodeInt32(encoded, len(input), &DecodeOptionsNew[int32]{Scheme: scheme})

			require.Len(t, decoded, len(input))
			if len(input) == 0 && len(encoded) == 0 {
				continue
			}
			assert.Equal(t, inputSigned, decoded)

			// re-check against the scalar implementation
			scalar_encoded := make([]byte, MaxEncodedLen(len(input)))
			n := encodeDeltaScalarZigzag(scalar_encoded, inputSigned, 0, scheme)
			scalar_encoded = scalar_encoded[:n]
			assert.Equal(t, scalar_encoded, encoded)
		}
	})
}
