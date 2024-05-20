package streamvbyte

import "unsafe"

func svb_zigzag_encode(in []int32, out []uint32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	zigzag_encode(unsafe.SliceData(in), unsafe.SliceData(out), int64(len(in)))
}

func svb_zigzag_delta_encode(in []int32, out []uint32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	zigzag_delta_encode(unsafe.SliceData(in), unsafe.SliceData(out), int64(len(in)), prev)
}

func svb_zigzag_decode(in []uint32, out []int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	zigzag_decode(unsafe.SliceData(in), unsafe.SliceData(out), int64(len(in)))
}

func svb_zigzag_delta_decode(in []uint32, out []int32, prev int32) {
	if len(in) > len(out) {
		panic("output slice is too small")
	}

	zigzag_delta_decode(unsafe.SliceData(in), unsafe.SliceData(out), int64(len(in)), prev)
}
