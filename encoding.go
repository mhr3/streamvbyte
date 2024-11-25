package streamvbyte

type Encoding interface {
	Encode(input []uint32, output []byte) []byte
	Decode(input []byte, count int, output []uint32) []uint32

	EncodeDelta(input []uint32, output []byte, prev uint32) []byte
	DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32
}

func MaxEncodedLen(n int) int {
	numControlBytes := (n + 3) / 4
	maxNumDataBytes := 4 * n
	return numControlBytes + maxNumDataBytes
}

type stdEncoding struct{}
type altEncoding struct{}

var (
	// StdEncoding is the standard streamvbyte encoding, which encodes the data with the 1234 scheme.
	// This means every uint32 value is encoded with 1, 2, 3 or 4 bytes (plus 2 control bits).
	StdEncoding = stdEncoding{}

	// AltEncoding optimizes for data containing lots of zeros, and encodes the data with the 0124 scheme.
	// This means zeroes are only represented in the control bits, but don't add to the data bytes.
	// As an example a slice of 2000 zeroes would be encoded in 2500 bytes with standard encoding, but only
	// 500 bytes with the alternative encoding.
	AltEncoding = altEncoding{}

	_ Encoding = StdEncoding
	_ Encoding = AltEncoding
)
