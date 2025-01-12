package streamvbyte

import "unsafe"

type Scheme int

const (
	Scheme1234 Scheme = iota
	Scheme0124
)

type EncodeOptions struct {
	Scheme  Scheme
	Output  []byte
	Initial int32 // Initial value for delta encoding
}

func Encode[T uint32 | int32](input []T, opts *EncodeOptions) []byte {
	if opts == nil {
		opts = &EncodeOptions{}
	}

	if len(input) == 0 {
		return nil
	}

	ifc := any((*T)(nil))
	switch ifc.(type) {
	case *uint32:
		inputUint32 := forceSliceType[uint32](input)
		switch opts.Scheme {
		case Scheme0124:
			return AltEncoding.Encode(inputUint32, opts.Output)
		default:
			return StdEncoding.Encode(inputUint32, opts.Output)
		}
	case *int32:
		inputInt32 := forceSliceType[int32](input)
		switch opts.Scheme {
		case Scheme0124:
			// FIXME: ZigZagAlt
			return ZigZagEncoding.Encode(inputInt32, opts.Output)
		default:
			return ZigZagEncoding.Encode(inputInt32, opts.Output)
		}
	}

	return nil
}

type DecodeOptions[T uint32 | int32] struct {
	Scheme  Scheme
	Output  []T
	Initial int32 // Initial value for delta decoding
}

func Decode[T uint32 | int32](input []byte, count int, opts *DecodeOptions[T]) []T {
	if opts == nil {
		opts = &DecodeOptions[T]{}
	}

	if len(input) == 0 {
		return nil
	}

	ifc := any((*T)(nil))
	switch ifc.(type) {
	case *uint32:
		outputUint32 := forceSliceType[uint32](opts.Output)
		var res []uint32
		switch opts.Scheme {
		case Scheme0124:
			res = AltEncoding.Decode(input, count, outputUint32)
		default:
			res = StdEncoding.Decode(input, count, outputUint32)
		}
		return forceSliceType[T](res)
	case *int32:
		outputInt32 := forceSliceType[int32](opts.Output)
		var res []int32
		switch opts.Scheme {
		case Scheme0124:
			// FIXME: ZigZagAlt
			res = ZigZagEncoding.Decode(input, count, outputInt32)
		default:
			res = ZigZagEncoding.Decode(input, count, outputInt32)
		}
		return forceSliceType[T](res)
	}

	return nil
}

func EncodeDelta[T uint32 | int32](input []T, opts *EncodeOptions) []byte {
	if opts == nil {
		opts = &EncodeOptions{}
	}

	if len(input) == 0 {
		return nil
	}

	ifc := any((*T)(nil))
	switch ifc.(type) {
	case *uint32:
		inputUint32 := forceSliceType[uint32](input)
		switch opts.Scheme {
		case Scheme0124:
			return AltEncoding.EncodeDelta(inputUint32, opts.Output, uint32(opts.Initial))
		default:
			return StdEncoding.EncodeDelta(inputUint32, opts.Output, uint32(opts.Initial))
		}
	case *int32:
		inputInt32 := forceSliceType[int32](input)
		switch opts.Scheme {
		case Scheme0124:
			// FIXME: ZigZagAlt
			return ZigZagEncoding.EncodeDelta(inputInt32, opts.Output, int32(opts.Initial))
		default:
			return ZigZagEncoding.EncodeDelta(inputInt32, opts.Output, int32(opts.Initial))
		}
	}

	return nil
}

func DecodeDelta[T uint32 | int32](input []byte, count int, opts *DecodeOptions[T]) []T {
	if opts == nil {
		opts = &DecodeOptions[T]{}
	}

	if len(input) == 0 {
		return nil
	}

	ifc := any((*T)(nil))
	switch ifc.(type) {
	case *uint32:
		outputUint32 := forceSliceType[uint32](opts.Output)
		var res []uint32
		switch opts.Scheme {
		case Scheme0124:
			res = AltEncoding.DecodeDelta(input, count, outputUint32, uint32(opts.Initial))
		default:
			res = StdEncoding.DecodeDelta(input, count, outputUint32, uint32(opts.Initial))
		}
		return forceSliceType[T](res)
	case *int32:
		outputInt32 := forceSliceType[int32](opts.Output)
		var res []int32
		switch opts.Scheme {
		case Scheme0124:
			// FIXME: ZigZagAlt
			res = ZigZagEncoding.DecodeDelta(input, count, outputInt32, int32(opts.Initial))
		default:
			res = ZigZagEncoding.DecodeDelta(input, count, outputInt32, int32(opts.Initial))
		}
		return forceSliceType[T](res)
	}

	return nil
}

func forceSliceType[T, U uint32 | int32](input []U) []T {
	sd := unsafe.SliceData(input)

	return unsafe.Slice((*T)(unsafe.Pointer(sd)), len(input))
}
