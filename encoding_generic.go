//go:build noasm || (!amd64 && !arm64)

package streamvbyte

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	if len(output) < MaxEncodedLen(len(input)) {
		output = make([]byte, MaxEncodedLen(len(input)))
	}
	n := encodeScalar1234(output, input)
	return output[:n]
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar1234(output, input)
	return output[:count]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	panic("not implemented")
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	panic("not implemented")
}

/*
	!!! AltEncoding below !!!
*/

func (e altEncoding) Encode(input []uint32, output []byte) []byte {
	if len(output) < MaxEncodedLen(len(input)) {
		output = make([]byte, MaxEncodedLen(len(input)))
	}
	n := encodeScalar0124(output, input)
	return output[:n]
}

func (e altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar0124(output, input)
	return output[:count]
}

func (e altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	panic("not implemented")
}

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	panic("not implemented")
}
