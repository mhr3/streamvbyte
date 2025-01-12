package streamvbyte

type Encoding[T uint32 | int32] interface {
	// Encode encodes the input slice of uint32 values.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	Encode(input []T, output []byte) []byte
	// Decode decodes the input slice of bytes.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	Decode(input []byte, count int, output []T) []T

	// EncodeDelta encodes the input slice of uint32 values.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	EncodeDelta(input []T, output []byte, prev T) []byte
	// DecodeDelta decodes the input slice of bytes.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	DecodeDelta(input []byte, count int, output []T, prev T) []T
}

// MaxEncodedLen returns the maximum number of bytes required to encode n uint32 values.
func MaxEncodedLen(n int) int {
	numControlBytes := (n + 3) / 4
	maxNumDataBytes := 4 * n
	return numControlBytes + maxNumDataBytes
}

type stdEncoding struct{}
type zigzagEncoding struct{}
type altEncoding struct{}

var (
	// StdEncoding is the standard streamvbyte encoding, which encodes the data with the 1234 scheme.
	// This means every uint32 value is encoded with 1, 2, 3 or 4 bytes (plus 2 control bits).
	StdEncoding = stdEncoding{}

	ZigZagEncoding = zigzagEncoding{}

	// AltEncoding optimizes for data containing lots of zeros, and encodes the data with the 0124 scheme.
	// This means zeroes are only represented in the control bits, but don't add to the data bytes.
	// As an example a slice of 2000 zeroes would be encoded in 2500 bytes with standard encoding, but only
	// 500 bytes with the alternative encoding.
	AltEncoding = altEncoding{}

	_ Encoding[uint32] = StdEncoding
	_ Encoding[int32]  = ZigZagEncoding
	_ Encoding[uint32] = AltEncoding
)
