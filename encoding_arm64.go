//go:build !noasm

package streamvbyte

func (stdEncoding) Encode(input []uint32, output []byte) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_encode(input, &output[0])
	return output[:n]
}

func (stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode(input, count, &output[0])
	return output[:sz]
}

func (stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_delta_encode(input, prev, &output[0])
	return output[:n]
}

func (stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_delta_decode(input, count, prev, &output[0])
	return output[:sz]
}

func (zigzagEncoding) Encode(input []int32, output []byte) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_encode_zz(input, &output[0])
	return output[:n]
}

func (zigzagEncoding) Decode(input []byte, count int, output []int32) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}
	sz := svb_decode_zz(input, count, &output[0])
	return output[:sz]
}

func (zigzagEncoding) EncodeDelta(input []int32, output []byte, prev int32) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_delta_encode_zz(input, prev, &output[0])
	return output[:n]
}

func (zigzagEncoding) DecodeDelta(input []byte, count int, output []int32, prev int32) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}
	sz := svb_delta_decode_zz(input, count, prev, &output[0])
	return output[:sz]
}

/*
	!!! AltEncoding below !!!
*/

func (altEncoding) Encode(input []uint32, output []byte) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_encode_alt(input, &output[0])
	return output[:n]
}

func (altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode_alt(input, count, &output[0])
	return output[:sz]
}

func (altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_delta_encode_alt(input, prev, &output[0])
	return output[:n]
}

func (altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_delta_decode_alt(input, count, prev, &output[0])
	return output[:sz]
}
