//go:build noasm || (!amd64 && !arm64)

package streamvbyte

func (uintEncodingWrapper) Encode(input []uint32, output []byte, scheme Scheme) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalar(output[:sz], input, scheme)
	return output[:n]
}

func (uintEncodingWrapper) Decode(input []byte, count int, output []uint32, scheme Scheme) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar(output, input, scheme)
	return output[:count]
}

func (uintEncodingWrapper) EncodeDelta(input []uint32, output []byte, prev uint32, scheme Scheme) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalar(output[:sz], input, prev, scheme)
	return output[:n]
}

func (uintEncodingWrapper) DecodeDelta(input []byte, count int, output []uint32, prev uint32, scheme Scheme) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeDeltaScalar(output, input, prev, scheme)
	return output[:count]
}

/*
	!!! Int32Encoding below !!!
*/

func (intEncodingWrapper) Encode(input []int32, output []byte, scheme Scheme) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalarZigzag(output[:sz], input, scheme)
	return output[:n]
}

func (intEncodingWrapper) Decode(input []byte, count int, output []int32, scheme Scheme) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeScalarZigzag(output, input, scheme)
	return output[:count]
}

func (intEncodingWrapper) EncodeDelta(input []int32, output []byte, prev int32, scheme Scheme) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalarZigzag(output[:sz], input, prev, scheme)
	return output[:n]
}

func (intEncodingWrapper) DecodeDelta(input []byte, count int, output []int32, prev int32, scheme Scheme) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeDeltaScalarZigzag(output, input, prev, scheme)
	return output[:count]
}
