//go:build !noasm

package streamvbyte

import (
	"golang.org/x/sys/cpu"
)

var hasSSE41 = cpu.X86.HasSSE41

func (uintEncodingWrapper) Encode(input []uint32, output []byte, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_encode_u32_std(input, &output[0]))
		case Scheme0124:
			n = int(svb_encode_u32_alt(input, &output[0]))
		}
	} else {
		n = encodeScalar(output[:sz], input, scheme)
	}
	return output[:n]
}

func (uintEncodingWrapper) Decode(input []byte, count int, output []uint32, scheme Scheme) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_decode_u32_std(input, count, &output[0]))
		case Scheme0124:
			n = int(svb_decode_u32_alt(input, count, &output[0]))
		}
	} else {
		decodeScalar(output, input, scheme)
		n = count
	}
	return output[:n]
}

func (uintEncodingWrapper) EncodeDelta(input []uint32, output []byte, prev uint32, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_delta_encode_u32_std(input, prev, &output[0]))
		case Scheme0124:
			n = int(svb_delta_encode_u32_alt(input, prev, &output[0]))
		}
	} else {
		n = encodeDeltaScalar(output[:sz], input, prev, scheme)
	}
	return output[:n]
}

func (uintEncodingWrapper) DecodeDelta(input []byte, count int, output []uint32, prev uint32, scheme Scheme) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_delta_decode_u32_std(input, count, prev, &output[0]))
		case Scheme0124:
			n = int(svb_delta_decode_u32_alt(input, count, prev, &output[0]))
		}
	} else {
		decodeDeltaScalar(output, input, prev, scheme)
		n = count
	}
	return output[:n]
}

/*
	!!! Int32Encoding below !!!
*/

func (intEncodingWrapper) Encode(input []int32, output []byte, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_encode_s32_std(input, &output[0]))
		case Scheme0124:
			n = int(svb_encode_s32_alt(input, &output[0]))
		}
	} else {
		n = encodeScalarZigzag(output[:sz], input, scheme)
	}
	return output[:n]
}

func (intEncodingWrapper) Decode(input []byte, count int, output []int32, scheme Scheme) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_decode_s32_std(input, count, &output[0]))
		case Scheme0124:
			n = int(svb_decode_s32_alt(input, count, &output[0]))
		}
	} else {
		decodeScalarZigzag(output, input, scheme)
		n = count
	}
	return output[:n]
}

func (intEncodingWrapper) EncodeDelta(input []int32, output []byte, prev int32, scheme Scheme) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_delta_encode_s32_std(input, prev, &output[0]))
		case Scheme0124:
			n = int(svb_delta_encode_s32_alt(input, prev, &output[0]))
		}
	} else {
		n = encodeDeltaScalarZigzag(output[:sz], input, prev, scheme)
	}
	return output[:n]
}

func (intEncodingWrapper) DecodeDelta(input []byte, count int, output []int32, prev int32, scheme Scheme) []int32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]int32, count)
	}

	var n int
	if hasSSE41 {
		switch scheme {
		case Scheme1234:
			n = int(svb_delta_decode_s32_std(input, count, prev, &output[0]))
		case Scheme0124:
			n = int(svb_delta_decode_s32_alt(input, count, prev, &output[0]))
		}
	} else {
		decodeDeltaScalarZigzag(output, input, prev, scheme)
		n = count
	}
	return output[:n]
}
