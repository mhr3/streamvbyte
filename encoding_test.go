package streamvbyte_test

import (
	"math/rand"
	"testing"

	"github.com/mhr3/streamvbyte"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDecode(t *testing.T) {
	input := make([]uint32, 1000)
	for i := range input {
		input[i] = rand.Uint32() % 65536
	}

	// Encode
	encoded := streamvbyte.StdEncoding.Encode(input, nil)
	decoded := streamvbyte.StdEncoding.Decode(encoded, len(input), nil)

	require.Len(t, decoded, len(input))
	assert.Equal(t, input, decoded)
}
