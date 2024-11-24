package streamvbyte_test

import (
	"math"
	"math/rand"
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

func BenchmarkZigZagEncode(b *testing.B) {
	const sz = 32 * 1024

	input := make([]int32, sz)
	for i := range input {
		input[i] = rand.Int31n(100) - 50
	}
	output := make([]uint32, len(input))

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		streamvbyte.ZizZag.Encode(input, output)
	}
	b.SetBytes(int64(sz) * 4)
}

func BenchmarkZigZagDecode(b *testing.B) {
	const sz = 32 * 1024

	input := make([]int32, sz)
	for i := range input {
		input[i] = rand.Int31n(100) - 50
	}
	output := make([]int32, len(input))
	encoded := streamvbyte.ZizZag.Encode(input, nil)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		streamvbyte.ZizZag.Decode(encoded, output)
	}
	b.SetBytes(int64(sz) * 4)
}
