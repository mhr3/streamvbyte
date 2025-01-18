package streamvbyte

type Scheme byte

const (
	// Scheme1234 is the standard streamvbyte encoding, which encodes the data with the 1234 scheme.
	// This means every uint32 value is encoded with 1, 2, 3 or 4 bytes (plus 2 control bits).
	Scheme1234 Scheme = 0

	// Scheme0124 optimizes for data containing lots of zeros, and encodes the data with the 0124 scheme.
	// This means zeroes are only represented in the control bits, but don't add to the data bytes.
	// As an example a slice of 2000 zeroes would be encoded in 2500 bytes with standard encoding, but only
	// 500 bytes with the alternative encoding.
	Scheme0124 Scheme = 1
)

func (s Scheme) String() string {
	switch s {
	case Scheme0124:
		return "0124"
	case Scheme1234:
		return "1234"
	default:
		return "unknown"
	}
}

type Encoding[T uint32 | int32] interface {
	// Encode encodes the input slice of uint32 values.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	Encode(input []T, output []byte, scheme Scheme) []byte
	// Decode decodes the input slice of bytes.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	Decode(input []byte, count int, output []T, scheme Scheme) []T

	// EncodeDelta encodes the input slice of uint32 values.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	EncodeDelta(input []T, output []byte, prev T, scheme Scheme) []byte
	// DecodeDelta decodes the input slice of bytes.
	// If the output slice is too small or nil, a new slice will be allocated and returned.
	DecodeDelta(input []byte, count int, output []T, prev T, scheme Scheme) []T
}

// MaxEncodedLen returns the maximum number of bytes required to encode n uint32 values.
func MaxEncodedLen(n int) int {
	numControlBytes := (n + 3) / 4
	maxNumDataBytes := 4 * n
	return numControlBytes + maxNumDataBytes
}

type uintEncoding struct{}
type intEncoding struct{}

var (
	UintEncoding = uintEncoding{}

	IntEncoding = intEncoding{}

	_ Encoding[uint32] = UintEncoding
	_ Encoding[int32]  = IntEncoding
)
