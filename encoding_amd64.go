//go:build !noasm

package streamvbyte

import (
	"golang.org/x/sys/cpu"
)

var hasSSE41 = cpu.X86.HasSSE41

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
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
		n = encodeScalar1234(output[:sz], input)
	}
	return output[:n]
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
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
		decodeScalar1234(output, input)
		n = count
	}
	return output[:n]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
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
		n = encodeDeltaScalar1234(output[:sz], input, prev)
	}
	return output[:n]
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
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
		decodeDeltaScalar1234(output, input, prev)
		n = count
	}
	return output[:n]
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

	var n int
	if hasSSE41 {
		n = int(svb_encode_alt(input, &output[0]))
	} else {
		n = encodeScalar0124(output[:sz], input)
	}
	return output[:n]
}

func (e altEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
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
		decodeScalar0124(output, input)
		n = count
	}
	return output[:n]
}

func (e altEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
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
		n = encodeDeltaScalar0124(output[:sz], input, prev)
	}
	return output[:n]
}

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
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
		decodeDeltaScalar0124(output, input, prev)
		n = count
	}
	return output[:n]
}
