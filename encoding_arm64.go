package streamvbyte

import "unsafe"

func (e stdEncoding) Encode(input []uint32, output []byte) []byte {
	// TODO: This is a temporary implementation. It should be replaced with an optimized one.
	if len(output) < MaxEncodedLen(len(input)) {
		output = make([]byte, MaxEncodedLen(len(input)))
	}
	encodeScalar1234(output, input)
	return output
}

func (e stdEncoding) Decode(input []byte, count int, output []uint32) []uint32 {
	if len(output) < count {
		output = make([]uint32, count)
	}
	sz := svb_decode(input, count, unsafe.SliceData(output))
	return output[:sz]
}

func (e stdEncoding) EncodeDelta(input []uint32, output []byte, prev uint32) []byte {
	panic("implement me")
}

func (e stdEncoding) DecodeDelta(input []byte, count int, output []uint32, prev uint32) []uint32 {
	sz := svb_delta_decode_vector(output, input, prev)
	return output[:sz]
}
