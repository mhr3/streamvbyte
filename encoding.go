package streamvbyte

type Encoding interface {
	Encode(input []uint32) []byte
	EncodeTo(input []uint32, output []byte) int
	Decode(input []byte) ([]uint32, error)
	DecodeTo(input []byte, output []uint32) int
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
