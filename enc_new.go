package streamvbyte

type EncodeOptionsNew[T uint32 | int32] struct {
	Scheme  Scheme
	Buffer  []byte
	Initial T // Initial value for delta encoding
}

type DecodeOptionsNew[T uint32 | int32] struct {
	Scheme  Scheme
	Buffer  []T
	Initial T // Initial value for delta decoding
}

func EncodeInt32(input []int32, opts *EncodeOptionsNew[int32]) []byte {
	if opts == nil {
		opts = &EncodeOptionsNew[int32]{}
	}

	// TODO: missing alt
	return IntEncoding.Encode(input, opts.Buffer, opts.Scheme)
}

func DecodeInt32(input []byte, count int, opts *DecodeOptionsNew[int32]) []int32 {
	if opts == nil {
		opts = &DecodeOptionsNew[int32]{}
	}

	return IntEncoding.Decode(input, count, opts.Buffer, opts.Scheme)
}

func DeltaEncodeInt32(input []int32, opts *EncodeOptionsNew[int32]) []byte {
	if opts == nil {
		opts = &EncodeOptionsNew[int32]{}
	}

	return IntEncoding.EncodeDelta(input, opts.Buffer, opts.Initial, opts.Scheme)
}

func DeltaDecodeInt32(input []byte, count int, opts *DecodeOptionsNew[int32]) []int32 {
	if opts == nil {
		opts = &DecodeOptionsNew[int32]{}
	}

	return IntEncoding.DecodeDelta(input, count, opts.Buffer, opts.Initial, opts.Scheme)
}

func EncodeUint32(input []uint32, opts *EncodeOptionsNew[uint32]) []byte {
	if opts == nil {
		opts = &EncodeOptionsNew[uint32]{}
	}

	return UintEncoding.Encode(input, opts.Buffer, opts.Scheme)
}

func DecodeUint32(input []byte, count int, opts *DecodeOptionsNew[uint32]) []uint32 {
	if opts == nil {
		opts = &DecodeOptionsNew[uint32]{}
	}

	return UintEncoding.Decode(input, count, opts.Buffer, opts.Scheme)
}

func DeltaEncodeUint32(input []uint32, opts *EncodeOptionsNew[uint32]) []byte {
	if opts == nil {
		opts = &EncodeOptionsNew[uint32]{}
	}

	return UintEncoding.EncodeDelta(input, opts.Buffer, opts.Initial, opts.Scheme)
}

func DeltaDecodeUint32(input []byte, count int, opts *DecodeOptionsNew[uint32]) []uint32 {
	if opts == nil {
		opts = &DecodeOptionsNew[uint32]{}
	}

	return UintEncoding.DecodeDelta(input, count, opts.Buffer, opts.Initial, opts.Scheme)
}
