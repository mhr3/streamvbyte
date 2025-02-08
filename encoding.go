package streamvbyte

type Scheme byte

const (
	// Scheme1234 is the standard streamvbyte encoding, which encodes the data using 1, 2, 3 or 4 bytes
	// (plus 2 control bits for each number).
	Scheme1234 Scheme = 0

	// Scheme0124 optimizes for data containing lots of zeros, encoding the data using 0, 1, 2 or 4 bytes.
	// This means zeroes are only represented in the control bits, and do not add to the data bytes.
	// As an example, a slice of 1000 zeroes would be encoded in 1250 bytes with standard encoding, but only
	// 250 bytes with this scheme. However, this optimization comes at the cost of using 4 bytes
	// even for numbers that would fit into 3 bytes.
	Scheme0124 Scheme = 1
)

type EncodeOptions[T uint32 | int32] struct {
	Scheme  Scheme
	Buffer  []byte
	Initial T // Initial value for delta encoding
}

type DecodeOptions[T uint32 | int32] struct {
	Scheme  Scheme
	Buffer  []T
	Initial T // Initial value for delta decoding
}

func (s Scheme) String() string {
	switch s {
	case Scheme1234:
		return "std"
	case Scheme0124:
		return "alt"
	default:
		return "unknown"
	}
}

// MaxEncodedLen returns the maximum number of bytes required to encode n uint32 values.
func MaxEncodedLen(n int) int {
	numControlBytes := (n + 3) / 4
	maxNumDataBytes := 4 * n
	return numControlBytes + maxNumDataBytes
}

func EncodeInt32(input []int32, opts *EncodeOptions[int32]) []byte {
	if opts == nil {
		opts = &EncodeOptions[int32]{}
	}

	return intEncoding{}.Encode(input, opts.Buffer, opts.Scheme)
}

func DecodeInt32(input []byte, count int, opts *DecodeOptions[int32]) []int32 {
	if opts == nil {
		opts = &DecodeOptions[int32]{}
	}

	return intEncoding{}.Decode(input, count, opts.Buffer, opts.Scheme)
}

func DeltaEncodeInt32(input []int32, opts *EncodeOptions[int32]) []byte {
	if opts == nil {
		opts = &EncodeOptions[int32]{}
	}

	return intEncoding{}.EncodeDelta(input, opts.Buffer, opts.Initial, opts.Scheme)
}

func DeltaDecodeInt32(input []byte, count int, opts *DecodeOptions[int32]) []int32 {
	if opts == nil {
		opts = &DecodeOptions[int32]{}
	}

	return intEncoding{}.DecodeDelta(input, count, opts.Buffer, opts.Initial, opts.Scheme)
}

func EncodeUint32(input []uint32, opts *EncodeOptions[uint32]) []byte {
	if opts == nil {
		opts = &EncodeOptions[uint32]{}
	}

	return uintEncoding{}.Encode(input, opts.Buffer, opts.Scheme)
}

func DecodeUint32(input []byte, count int, opts *DecodeOptions[uint32]) []uint32 {
	if opts == nil {
		opts = &DecodeOptions[uint32]{}
	}

	return uintEncoding{}.Decode(input, count, opts.Buffer, opts.Scheme)
}

func DeltaEncodeUint32(input []uint32, opts *EncodeOptions[uint32]) []byte {
	if opts == nil {
		opts = &EncodeOptions[uint32]{}
	}

	return uintEncoding{}.EncodeDelta(input, opts.Buffer, opts.Initial, opts.Scheme)
}

func DeltaDecodeUint32(input []byte, count int, opts *DecodeOptions[uint32]) []uint32 {
	if opts == nil {
		opts = &DecodeOptions[uint32]{}
	}

	return uintEncoding{}.DecodeDelta(input, count, opts.Buffer, opts.Initial, opts.Scheme)
}
