package streamvbyte

type Encoding interface {
	Encode(input []uint32, output []byte) []byte
	Decode(input []byte, output []uint32) []uint32

	EncodeDelta(input []uint32, output []byte, prev uint32) []byte
	DecodeDelta(input []byte, output []uint32, prev uint32) []uint32
}

func MaxEncodedLen(n int) int {
	numControlBytes := (n + 3) / 4
	maxNumDataBytes := 4 * n
	return numControlBytes + maxNumDataBytes
}

type stdEncoding struct{}
type compactEncoding struct{}

var (
	StdEncoding     = stdEncoding{}
	CompactEncoding = compactEncoding{}
)

func (e stdEncoding) Encode(input []uint32) []byte {
	panic("implement me")
}
