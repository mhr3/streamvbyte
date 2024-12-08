//go:build !noasm && amd64

package streamvbyte

import "golang.org/x/sys/cpu"

var (
	useAvx2 = cpu.X86.HasAVX2
)

func zigzag_encode(in *int32, out *uint32, sz int) {
	if useAvx2 {
		zigzag_encode_avx2(in, out, sz)
		return
	}
	zigzag_encode_sse(in, out, sz)
}

func zigzag_delta_encode(in *int32, out *uint32, sz int, prev int32) {
	if useAvx2 {
		zigzag_delta_encode_avx2(in, out, sz, prev)
		return
	}
	zigzag_delta_encode_sse(in, out, sz, prev)
}

func zigzag_decode(in *uint32, out *int32, sz int) {
	if useAvx2 {
		zigzag_decode_avx2(in, out, sz)
		return
	}
	zigzag_decode_sse(in, out, sz)
}

func zigzag_delta_decode(in *uint32, out *int32, sz int, prev int32) {
	if useAvx2 {
		zigzag_delta_decode_avx2(in, out, sz, prev)
		return
	}
	zigzag_delta_decode_sse(in, out, sz, prev)
}
