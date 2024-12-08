package streamvbyte_test

import (
	"math"
	"math/rand"
	"slices"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/mhr3/streamvbyte"
)

func TestZigZagConvert(t *testing.T) {
	input := make([]int32, 10000)
	for i := range input {
		input[i] = int32(rand.NormFloat64() * math.MaxInt16)
	}

	// Encode
	encoded := streamvbyte.ZizZag.Encode(input, nil)
	decoded := streamvbyte.ZizZag.Decode(encoded, nil)

	assert.Equal(t, input, decoded)

	// EncodeDelta
	encoded = streamvbyte.ZizZag.EncodeDelta(input, nil, 0)
	decoded = streamvbyte.ZizZag.DecodeDelta(encoded, nil, 0)

	assert.Equal(t, input, decoded)
}

func BenchmarkZigZag(b *testing.B) {
	const sz = 32 * 1024

	input := make([]int32, sz)
	for i := range input {
		input[i] = rand.Int31n(100) - 50
	}

	b.Run("Encode", func(b *testing.B) {
		var output []uint32
		for i := 0; i < b.N; i++ {
			output = streamvbyte.ZizZag.Encode(input, output)
		}
		b.SetBytes(int64(sz) * 4)
	})

	b.Run("Decode", func(b *testing.B) {
		encoded := streamvbyte.ZizZag.Encode(input, nil)
		var output []int32

		b.ResetTimer()
		for i := 0; i < b.N; i++ {
			output = streamvbyte.ZizZag.Decode(encoded, output)
		}
		b.SetBytes(int64(sz) * 4)
	})
}

func BenchmarkZigZagDelta(b *testing.B) {
	const sz = 32 * 1024

	input := make([]int32, sz)
	for i := range input {
		input[i] = rand.Int31n(100) - 50
	}
	slices.Sort(input)

	b.Run("Encode", func(b *testing.B) {
		var output []uint32
		for i := 0; i < b.N; i++ {
			output = streamvbyte.ZizZag.EncodeDelta(input, output, 0)
		}
		b.SetBytes(int64(sz) * 4)
	})

	b.Run("Decode", func(b *testing.B) {
		encoded := streamvbyte.ZizZag.EncodeDelta(input, nil, 0)
		var output []int32

		b.ResetTimer()
		for i := 0; i < b.N; i++ {
			output = streamvbyte.ZizZag.DecodeDelta(encoded, output, 0)
		}
		b.SetBytes(int64(sz) * 4)
	})
}
