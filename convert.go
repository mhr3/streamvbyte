package streamvbyte

import "unsafe"

type zigZag struct{}

var ZigZag = zigZag{}

func zigzag_encode32(val int32) uint32 {
	return uint32(val+val) ^ (uint32)(val>>31)
}

func zigzag_decode32(val uint32) int32 {
	return int32((val >> 1) ^ (0 - (val & 1)))
}

func convertSliceTo[T, U uint32 | int32](input []U) []T {
	sd := unsafe.SliceData(input)

	return unsafe.Slice((*T)(unsafe.Pointer(sd)), len(input))
}

// Encode encodes the input slice of int32 values into the output slice of uint32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) Encode(input []int32, output []uint32) []uint32 {
	l := len(input)

	if output == nil {
		output = make([]uint32, l)
	}
	svb_zigzag_encode(input, output)

	return output[:l]
}

// EncodeDelta encodes the input slice of int32 values into the output slice of uint32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) EncodeDelta(input []int32, output []uint32, prev int32) []uint32 {
	l := len(input)

	if output == nil {
		output = make([]uint32, l)
	}
	svb_zigzag_delta_encode(input, output, prev)

	return output[:l]
}

// Decode decodes the input slice of uint32 values into the output slice of int32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) Decode(input []uint32, output []int32) []int32 {
	l := len(input)

	if output == nil {
		output = make([]int32, l)
	}
	svb_zigzag_decode(input, output)

	return output[:l]
}

// DecodeDelta decodes the input slice of uint32 values into the output slice of int32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) DecodeDelta(input []uint32, output []int32, prev int32) []int32 {
	l := len(input)

	if output == nil {
		output = make([]int32, l)
	}
	svb_zigzag_delta_decode(input, output, prev)

	return output[:l]
}
