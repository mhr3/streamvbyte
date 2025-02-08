package streamvbyte

type encodingImpl[T uint32 | int32] interface {
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

type uintEncodingWrapper struct{}
type intEncodingWrapper struct{}

var (
	uintEncoding = uintEncodingWrapper{}
	intEncoding  = intEncodingWrapper{}

	_ encodingImpl[uint32] = uintEncoding
	_ encodingImpl[int32]  = intEncoding
)
