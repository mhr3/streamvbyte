package streamvbyte

type zigZag struct{}

var ZizZag = zigZag{}

// Encode encodes the input slice of int32 values into the output slice of uint32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) Encode(input []int32, output []uint32) []uint32 {
	if output == nil {
		output = make([]uint32, len(input))
	}
	svb_zigzag_encode(input, output)

	return output
}

// EncodeDelta encodes the input slice of int32 values into the output slice of uint32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) EncodeDelta(input []int32, output []uint32, prev int32) []uint32 {
	if output == nil {
		output = make([]uint32, len(input))
	}
	svb_zigzag_delta_encode(input, output, prev)

	return output
}

// Decode decodes the input slice of uint32 values into the output slice of int32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) Decode(input []uint32, output []int32) []int32 {
	if output == nil {
		output = make([]int32, len(input))
	}
	svb_zigzag_decode(input, output)

	return output
}

// DecodeDelta decodes the input slice of uint32 values into the output slice of int32 values.
// If the output slice is nil, a new slice will be allocated and returned.
func (z zigZag) DecodeDelta(input []uint32, output []int32, prev int32) []int32 {
	if output == nil {
		output = make([]int32, len(input))
	}
	svb_zigzag_delta_decode(input, output, prev)

	return output
}
