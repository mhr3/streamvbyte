//go:build !noasm

package streamvbyte

func (uintEncoding) Encode(input []uint32, output []byte, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_encode_u32(input, &output[0], byte(scheme))
	return output[:n]
}

func (uintEncoding) Decode(input []byte, count int, output []uint32, scheme Scheme) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode_u32(input, count, &output[0], byte(scheme))
	return output[:sz]
}

func (uintEncoding) EncodeDelta(input []uint32, output []byte, prev uint32, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_delta_encode_u32(input, prev, &output[0], byte(scheme))
	return output[:n]
}

func (uintEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32, scheme Scheme) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_delta_decode_u32(input, count, prev, &output[0], byte(scheme))
	return output[:sz]
}

/*
	!!! Int32Encoding below !!!
*/

func (intEncoding) Encode(input []int32, output []byte, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_encode_s32(input, &output[0], byte(scheme))
	return output[:n]
}

func (intEncoding) Decode(input []byte, count int, output []int32, scheme Scheme) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}
	sz := svb_decode_s32(input, count, &output[0], byte(scheme))
	return output[:sz]
}

func (intEncoding) EncodeDelta(input []int32, output []byte, prev int32, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	n := svb_delta_encode_s32(input, prev, &output[0], byte(scheme))
	return output[:n]
}

func (intEncoding) DecodeDelta(input []byte, count int, output []int32, prev int32, scheme Scheme) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}
	sz := svb_delta_decode_s32(input, count, prev, &output[0], byte(scheme))
	return output[:sz]
}
