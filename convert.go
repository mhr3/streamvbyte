package streamvbyte

type zigZag struct{}

var ZizZag = zigZag{}

func (z zigZag) Encode(input []int32) []uint32 {
	output := make([]uint32, len(input))
	z.EncodeTo(input, output)

	return output
}

func (z zigZag) EncodeTo(input []int32, output []uint32) {
	svb_zigzag_encode(input, output)
}

func (z zigZag) EncodeDelta(input []int32, prev int32) []uint32 {
	output := make([]uint32, len(input))
	z.EncodeDeltaTo(input, output, prev)

	return output
}

func (z zigZag) EncodeDeltaTo(input []int32, output []uint32, prev int32) {
	svb_zigzag_delta_encode(input, output, prev)
}

func (z zigZag) Decode(input []uint32) []int32 {
	output := make([]int32, len(input))
	z.DecodeTo(input, output)

	return output
}

func (z zigZag) DecodeTo(input []uint32, output []int32) {
	svb_zigzag_decode(input, output)
}

func (z zigZag) DecodeDelta(input []uint32, prev int32) []int32 {
	output := make([]int32, len(input))
	z.DecodeDeltaTo(input, output, prev)

	return output
}

func (z zigZag) DecodeDeltaTo(input []uint32, output []int32, prev int32) {
	svb_zigzag_delta_decode(input, output, prev)
}
