//go:build !noasm

package streamvbyte

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
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

func (e altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode_alt(input, count, &output[0])
	return output[:sz]
}

func (e altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
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

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_delta_decode_alt(input, count, prev, &output[0])
	return output[:sz]
}
