package streamvbyte_test

import (
	"math/rand"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/mhr3/streamvbyte"
)

func TestZigZagConvert(t *testing.T) {
	input := make([]int32, 1000)
	for i := range input {
		input[i] = rand.Int31n(100) - 50
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
