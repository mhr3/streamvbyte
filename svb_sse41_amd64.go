//go:build !noasm && amd64
// Code generated by gocc -- DO NOT EDIT.

package streamvbyte


//go:noescape
func svb_encode_u32_std(in []uint32, out *byte) uint64

//go:noescape
func svb_encode_u32_alt(in []uint32, out *byte) uint64

//go:noescape
func svb_encode_s32_std(in []int32, out *byte) uint64

//go:noescape
func svb_encode_s32_alt(in []int32, out *byte) uint64

//go:noescape
func svb_decode_u32_std(in []byte, count int, out *uint32) uint64

//go:noescape
func svb_decode_u32_alt(in []byte, count int, out *uint32) uint64

//go:noescape
func svb_decode_s32_std(in []byte, count int, out *int32) uint64

//go:noescape
func svb_decode_s32_alt(in []byte, count int, out *int32) uint64

//go:noescape
func svb_delta_encode_u32_std(in []uint32, prev uint32, out *byte) uint64

//go:noescape
func svb_delta_encode_u32_alt(in []uint32, prev uint32, out *byte) uint64

//go:noescape
func svb_delta_encode_s32_std(in []int32, prev int32, out *byte) uint64

//go:noescape
func svb_delta_encode_s32_alt(in []int32, prev int32, out *byte) uint64

//go:noescape
func svb_delta_decode_u32_std(in []byte, count int, prev uint32, out *uint32) uint64

//go:noescape
func svb_delta_decode_u32_alt(in []byte, count int, prev uint32, out *uint32) uint64

//go:noescape
func svb_delta_decode_s32_std(in []byte, count int, prev int32, out *int32) uint64

//go:noescape
func svb_delta_decode_s32_alt(in []byte, count int, prev int32, out *int32) uint64
