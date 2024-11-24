//go:build !noasm

package streamvbyte

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	// TODO: missing optimized implementation
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalar1234(output[:sz], input)
	return output[:n]
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode(input, count, &output[0])
	return output[:sz]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	// TODO: missing optimized implementation
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalar1234(output[:sz], input, prev)
	return output[:n]
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_delta_decode(input, count, prev, &output[0])
	return output[:sz]
}

/*
	!!! AltEncoding below !!!
*/

func (e altEncoding) Encode(input []uint32, output []byte) []byte {
	// TODO: missing optimized implementation
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalar0124(output[:sz], input)
	return output[:n]
}

func (e altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	// TODO: missing optimized implementation
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeScalar0124(output, input)
	return output[:count]
}

func (e altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	// TODO: missing optimized implementation
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalar0124(output[:sz], input, prev)
	return output[:n]
}

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	// TODO: missing optimized implementation
	if len(output) < count {
		output = make([]uint32, count)
	}
	decodeDeltaScalar0124(output, input, prev)
	return output[:count]
}
