//go:build !noasm

package streamvbyte

import (
	"golang.org/x/sys/cpu"
)

var hasSSE41 = cpu.X86.HasSSE41

func (stdEncoding) Encode(input []uint32, output []byte) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		n = int(svb_encode(input, &output[0]))
	} else {
		n = encodeScalar(output[:sz], input, Scheme1234)
	}
	return output[:n]
}

func (stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		n = int(svb_decode(input, count, &output[0]))
	} else {
		decodeScalar(output, input, Scheme1234)
		n = count
	}
	return output[:n]
}

func (stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		n = int(svb_delta_encode(input, prev, &output[0]))
	} else {
		n = encodeDeltaScalar(output[:sz], input, prev, Scheme1234)
	}
	return output[:n]
}

func (stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		n = int(svb_delta_decode(input, count, prev, &output[0]))
	} else {
		decodeDeltaScalar(output, input, prev, Scheme1234)
		n = count
	}
	return output[:n]
}

func (e zigzagEncoding) Encode(input []int32, output []byte) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeScalarZigzag(output[:sz], input, Scheme1234)
	return output[:n]
}

func (e zigzagEncoding) Decode(input []byte, count int, output []int32) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeScalarZigzag(output, input, Scheme1234)
	return output[:count]
}

func (e zigzagEncoding) EncodeDelta(input []int32, output []byte, prev int32) []byte {
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}
	n := encodeDeltaScalarZigzag(output[:sz], input, prev, Scheme1234)
	return output[:n]
}

func (e zigzagEncoding) DecodeDelta(input []byte, count int, output []int32, prev int32) []int32 {
	if len(output) < count {
		output = make([]int32, count)
	}
	decodeDeltaScalarZigzag(output, input, prev, Scheme1234)
	return output[:count]
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

	var n int
	if hasSSE41 {
		n = int(svb_encode_alt(input, &output[0]))
	} else {
		n = encodeScalar(output[:sz], input, Scheme0124)
	}
	return output[:n]
}

func (altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		n = int(svb_decode_alt(input, count, &output[0]))
	} else {
		decodeScalar(output, input, Scheme0124)
		n = count
	}
	return output[:n]
}

func (altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	if len(input) == 0 {
		return nil
	}
	sz := MaxEncodedLen(len(input))
	if cap(output) < sz {
		output = make([]byte, sz)
	}

	var n int
	if hasSSE41 {
		n = int(svb_delta_encode_alt(input, prev, &output[0]))
	} else {
		n = encodeDeltaScalar(output[:sz], input, prev, Scheme0124)
	}
	return output[:n]
}

func (altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	if count <= 0 {
		return nil
	}
	if len(output) < count {
		output = make([]uint32, count)
	}

	var n int
	if hasSSE41 {
		n = int(svb_delta_decode_alt(input, count, prev, &output[0]))
	} else {
		decodeDeltaScalar(output, input, prev, Scheme0124)
		n = count
	}
	return output[:n]
}
