//go:build !noasm && (amd64 || arm64)

package streamvbyte

func svb_zigzag_encode(in []int32, out []uint32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}
	if len(in) == 0 {
		return
	}

	zigzag_encode(&in[0], &out[0], len(in))
}

func svb_zigzag_delta_encode(in []int32, out []uint32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}
	if len(in) == 0 {
		return
	}

	zigzag_delta_encode(&in[0], &out[0], len(in), prev)
}

func svb_zigzag_decode(in []uint32, out []int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}
	if len(in) == 0 {
		return
	}

	zigzag_decode(&in[0], &out[0], len(in))
}

func svb_zigzag_delta_decode(in []uint32, out []int32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}
	if len(in) == 0 {
		return
	}

	zigzag_delta_decode(&in[0], &out[0], len(in), prev)
}
