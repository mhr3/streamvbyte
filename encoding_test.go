package streamvbyte_test

import (
	"math/rand"
	"testing"

	"github.com/mhr3/streamvbyte"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestEncodeDecode(t *testing.T) {
	input := make([]uint32, 10000)
	for i := range input {
		input[i] = rand.Uint32() % 131072
	}

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
}

func TestEncodeDecodeZeroes(t *testing.T) {
	input := make([]uint32, 10000)

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
}
