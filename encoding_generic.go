//go:build noasm || (!amd64 && !arm64)

package streamvbyte

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalar(output[:sz], input, encodingScheme1234)
	return output[:n]
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar(output, input, encodingScheme1234)
	return output[:count]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalar(output[:sz], input, prev, encodingScheme1234)
	return output[:n]
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeDeltaScalar(output, input, prev, encodingScheme1234)
	return output[:count]
}

func (e zigzagEncoding) Encode(input []int32, output []byte) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalarZigzag(output[:sz], input, encodingScheme1234)
	return output[:n]
}

func (e zigzagEncoding) Decode(input []byte, count int, output []int32) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeScalarZigzag(output, input, encodingScheme1234)
	return output[:count]
}

func (e zigzagEncoding) EncodeDelta(input []int32, output []byte, prev int32) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalarZigzag(output[:sz], input, prev, encodingScheme1234)
	return output[:n]
}

func (e zigzagEncoding) DecodeDelta(input []byte, count int, output []int32, prev int32) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeDeltaScalarZigzag(output, input, prev, encodingScheme1234)
	return output[:count]
}

/*
	!!! AltEncoding below !!!
*/

func (e altEncoding) Encode(input []uint32, output []byte) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalar(output[:sz], input, encodingScheme0124)
	return output[:n]
}

func (e altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar(output, input, encodingScheme0124)
	return output[:count]
}

func (e altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalar(output[:sz], input, prev, encodingScheme0124)
	return output[:n]
}

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeDeltaScalar(output, input, prev, encodingScheme0124)
	return output[:count]
}
