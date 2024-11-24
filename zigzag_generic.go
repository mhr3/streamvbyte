//go:build noasm || (!amd64 && !arm64)

package streamvbyte

func zigzag_encode32(val int32) uint32 {
	return uint32(val+val) ^ (uint32)(val>>31)
}

func svb_zigzag_encode(in []int32, out []uint32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	for i, v := range in {
		out[i] = zigzag_encode32(v)
	}
}

func svb_zigzag_delta_encode(in []int32, out []uint32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	for i, v := range in {
		out[i] = zigzag_encode32(v - prev)
		prev = v
	}
}

func zigzag_decode32(val uint32) int32 {
	return int32((val >> 1) ^ (0 - (val & 1)))
}

func svb_zigzag_decode(in []uint32, out []int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	for i, v := range in {
		out[i] = zigzag_decode32(v)
	}
}

func svb_zigzag_delta_decode(in []uint32, out []int32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	for i, v := range in {
		val := zigzag_decode32(v)
		out[i] = val + prev
		prev += val
	}
}
