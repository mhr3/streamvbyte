package streamvbyte

import "encoding/binary"

func encodeDeltaScalar(encoded []byte, data []uint32, previous uint32, scheme Scheme) int {
	// index of the control bytes
	ci := 0
	// index of the data bytes
	di := (len(data) + 3) >> 2

	controlByte := byte(0)
	switch scheme {
	case Scheme1234:
		for i, v := range data {
			tmp := v
			v -= previous
			previous = tmp
			controlByte >>= 2
			switch {
			case v < 1<<8:
				encoded[di] = byte(v)
				di++
			case v < 1<<16:
				binary.LittleEndian.PutUint16(encoded[di:], uint16(v))
				di += 2
				controlByte ^= 0b_01_00_00_00
			case v < 1<<24:
				encoded[di] = byte(v)
				encoded[di+1] = byte(v >> 8)
				encoded[di+2] = byte(v >> 16)
				di += 3
				controlByte ^= 0b_10_00_00_00
			default:
				binary.LittleEndian.PutUint32(encoded[di:], uint32(v))
				di += 4
				controlByte ^= 0b_11_00_00_00
			}
			if (i+1)&3 == 0 {
				encoded[ci] = controlByte
				controlByte = 0
				ci++
			}
		}
	case Scheme0124:
		for i, v := range data {
			tmp := v
			v -= previous
			previous = tmp
			controlByte >>= 2
			switch {
			case v == 0:
				// nothing to do
			case v < 1<<8:
				encoded[di] = byte(v)
				di++
				controlByte ^= 0b_01_00_00_00
			case v < 1<<16:
				binary.LittleEndian.PutUint16(encoded[di:], uint16(v))
				di += 2
				controlByte ^= 0b_10_00_00_00
			default:
				binary.LittleEndian.PutUint32(encoded[di:], uint32(v))
				di += 4
				controlByte ^= 0b_11_00_00_00
			}
			if (i+1)&3 == 0 {
				encoded[ci] = controlByte
				controlByte = 0
				ci++
			}
		}
	}
	// Check if the last block was complete or the control byte
	// needs to be shifted and written.
	if rem := len(data) & 3; rem != 0 {
		shift := uint(4-rem) * 2
		encoded[ci] = controlByte >> shift
	}
	return di
}

func decodeDeltaScalar(data []uint32, encoded []byte, previous uint32, scheme Scheme) {
	// index of the control bytes
	ci := 0
	// index of the data bytes
	di := (len(data) + 3) >> 2

	var controlByte byte
	switch scheme {
	case Scheme1234:
		for i := range data {
			if i&3 == 0 {
				controlByte = encoded[ci]
				ci++
			}
			var delta uint32
			switch controlByte & 3 {
			case 0:
				delta = uint32(encoded[di])
				di++
			case 1:
				delta = uint32(binary.LittleEndian.Uint16(encoded[di:]))
				di += 2
			case 2:
				delta = uint32(encoded[di+2])<<16 | uint32(encoded[di+1])<<8 | uint32(encoded[di])
				di += 3
			default:
				delta = binary.LittleEndian.Uint32(encoded[di:])
				di += 4
			}
			previous += delta
			data[i] = previous
			controlByte >>= 2
		}
	case Scheme0124:
		for i := range data {
			if i&3 == 0 {
				controlByte = encoded[ci]
				ci++
			}
			var delta uint32
			switch controlByte & 3 {
			case 0:
				delta = 0
			case 1:
				delta = uint32(encoded[di])
				di++
			case 2:
				delta = uint32(binary.LittleEndian.Uint16(encoded[di:]))
				di += 2
			default:
				delta = binary.LittleEndian.Uint32(encoded[di:])
				di += 4
			}
			previous += delta
			data[i] = previous
			controlByte >>= 2
		}
	}
}

func encodeDeltaScalarZigzag(encoded []byte, data []int32, previous int32, scheme Scheme) int {
	// index of the control bytes
	ci := 0
	// index of the data bytes
	di := (len(data) + 3) >> 2

	controlByte := byte(0)
	switch scheme {
	case Scheme1234:
		for i, sv := range data {
			tmp := sv
			sv -= previous
			previous = tmp

			controlByte >>= 2
			v := zigzag_encode32(sv)
			switch {
			case v < 1<<8:
				encoded[di] = byte(v)
				di++
			case v < 1<<16:
				binary.LittleEndian.PutUint16(encoded[di:], uint16(v))
				di += 2
				controlByte ^= 0b_01_00_00_00
			case v < 1<<24:
				encoded[di] = byte(v)
				encoded[di+1] = byte(v >> 8)
				encoded[di+2] = byte(v >> 16)
				di += 3
				controlByte ^= 0b_10_00_00_00
			default:
				binary.LittleEndian.PutUint32(encoded[di:], uint32(v))
				di += 4
				controlByte ^= 0b_11_00_00_00
			}
			if (i+1)&3 == 0 {
				encoded[ci] = controlByte
				controlByte = 0
				ci++
			}
		}
	case Scheme0124:
		for i, sv := range data {
			tmp := sv
			sv -= previous
			previous = tmp

			controlByte >>= 2
			v := zigzag_encode32(sv)
			switch {
			case v == 0:
				// nothing to do
			case v < 1<<8:
				encoded[di] = byte(v)
				di++
				controlByte ^= 0b_01_00_00_00
			case v < 1<<16:
				binary.LittleEndian.PutUint16(encoded[di:], uint16(v))
				di += 2
				controlByte ^= 0b_10_00_00_00
			default:
				binary.LittleEndian.PutUint32(encoded[di:], uint32(v))
				di += 4
				controlByte ^= 0b_11_00_00_00
			}
			if (i+1)&3 == 0 {
				encoded[ci] = controlByte
				controlByte = 0
				ci++
			}
		}
	}
	// Check if the last block was complete or the control byte
	// needs to be shifted and written.
	if rem := len(data) & 3; rem != 0 {
		shift := uint(4-rem) * 2
		encoded[ci] = controlByte >> shift
	}
	return di
}

func decodeDeltaScalarZigzag(data []int32, encoded []byte, previous int32, scheme Scheme) {
	// index of the control bytes
	ci := 0
	// index of the data bytes
	di := (len(data) + 3) >> 2

	var controlByte byte
	switch scheme {
	case Scheme1234:
		for i := range data {
			if i&3 == 0 {
				controlByte = encoded[ci]
				ci++
			}
			var delta uint32
			switch controlByte & 3 {
			case 0:
				delta = uint32(encoded[di])
				di++
			case 1:
				delta = uint32(binary.LittleEndian.Uint16(encoded[di:]))
				di += 2
			case 2:
				delta = uint32(encoded[di+2])<<16 | uint32(encoded[di+1])<<8 | uint32(encoded[di])
				di += 3
			default:
				delta = binary.LittleEndian.Uint32(encoded[di:])
				di += 4
			}
			previous += zigzag_decode32(delta)
			data[i] = previous
			controlByte >>= 2
		}
	case Scheme0124:
		for i := range data {
			if i&3 == 0 {
				controlByte = encoded[ci]
				ci++
			}
			var delta uint32
			switch controlByte & 3 {
			case 0:
				delta = 0
			case 1:
				delta = uint32(encoded[di])
				di++
			case 2:
				delta = uint32(binary.LittleEndian.Uint16(encoded[di:]))
				di += 2
			default:
				delta = binary.LittleEndian.Uint32(encoded[di:])
				di += 4
			}
			previous += zigzag_decode32(delta)
			data[i] = previous
			controlByte >>= 2
		}
	}
}
