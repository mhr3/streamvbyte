//go:build noasm || (!amd64 && !arm64)

package streamvbyte

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	if len(output) < MaxEncodedLen(len(input)) {
		output = make([]byte, MaxEncodedLen(len(input)))
	}
	encodeScalar1234(output, input)
	return output
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar1234(output, input)
	return output
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	panic("implement me")
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	panic("implement me")
}
