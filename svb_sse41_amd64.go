//go:build !noasm && amd64
// Code generated by gocc -- DO NOT EDIT.

package streamvbyte


//go:noescape
func svb_encode(in []uint32, out *byte) uint64

//go:noescape
func svb_encode_alt(in []uint32, out *byte) uint64

//go:noescape
func svb_decode(in []byte, count int, out *uint32) uint64

//go:noescape
func svb_decode_alt(in []byte, count int, out *uint32) uint64

//go:noescape
func svb_delta_encode(in []uint32, prev uint32, out *byte) uint64

//go:noescape
func svb_delta_encode_alt(in []uint32, prev uint32, out *byte) uint64

//go:noescape
func svb_delta_decode(in []byte, count int, prev uint32, out *uint32) uint64
