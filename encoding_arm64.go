//go:build !noasm

package streamvbyte

import "unsafe"

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	// TODO: missing optimized implementation
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
	sz := svb_decode(input, count, unsafe.SliceData(output))
	return output[:sz]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	panic("not implemented")
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	sz := svb_delta_decode_vector(output, input, prev)
	return output[:sz]
}

/*
	!!! AltEncoding below !!!
*/

func (e altEncoding) Encode(input []uint32, output []byte) []byte {
	// TODO: missing optimized implementation
	if len(output) < MaxEncodedLen(len(input)) {
		output = make([]byte, MaxEncodedLen(len(input)))
	}
	n := encodeScalar0124(output, input)
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
	panic("not implemented")
}

func (e altEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	panic("not implemented")
}
