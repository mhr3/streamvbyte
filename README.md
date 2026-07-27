# streamvbyte

StreamVByte is a high-performance integer compression library for encoding and decoding streams of 32-bit integers. It supports multiple architectures and implements two encoding schemes: the standard 1234 scheme (using between 1-4 bytes to represent the integers) suitable for most data, and the alternative 0124 scheme, which is optimized for data with lots of zeroes.

This library is based on two repositories:
- [lemire/streamvbyte](https://github.com/lemire/streamvbyte)
- [bmkessler/streamvbyte](https://github.com/bmkessler/streamvbyte)

The C code from lemire's codebase has been adjusted and transpiled to Go using [gocc](https://github.com/mhr3/gocc). Architectures other than amd64 and arm64 use a pure Go implementation based on bmkessler's code.

## Features

- High-speed encoding and decoding of integer streams
- Support for both signed and unsigned 32-bit integers (signed using zigzag encoding)
- SIMD support for amd64 (SSE4.1) and arm64 (NEON)
- Optimized encoding schemes for different data patterns
- Delta encoding for efficient compression of sequences

## Installation

To install the library, use `go get`:

```sh
go get github.com/mhr3/streamvbyte
```

## Usage

### Basic Encoding and Decoding

The library provides two encoding schemes via the `Scheme` type:
- `Scheme1234`: Standard scheme using 1-4 bytes per value
- `Scheme0124`: Alternative scheme optimized for data with many zeros

```go
package main

import (
    "github.com/mhr3/streamvbyte"
)

func main() {
    // Unsigned integers
    input := []uint32{1, 2, 3, 4, 5}
    
    // Basic encoding with default options
    encoded := streamvbyte.EncodeUint32(input, nil)
    decoded := streamvbyte.DecodeUint32(encoded, len(input), nil)

    // Encoding with specific scheme
    encoded = streamvbyte.EncodeUint32(input, &streamvbyte.EncodeOptions[uint32]{
        Scheme: streamvbyte.Scheme0124,  // Use alternative scheme
    })

    // Signed integers
    signedInput := []int32{-1, -2, -3, -4, -5}

    encoded = streamvbyte.EncodeInt32(signedInput, nil)
    decodedSigned := streamvbyte.DecodeInt32(encoded, len(signedInput), nil)
}
```

### Delta Encoding

Delta encoding is useful for compressing sequences of integers with small differences:

```go
package main

import (
    "github.com/mhr3/streamvbyte"
)

func main() {
    // Unsigned delta encoding
    input := []uint32{100, 101, 102, 103, 104}
    
    encoded := streamvbyte.DeltaEncodeUint32(input, nil)    
    decoded := streamvbyte.DeltaDecodeUint32(encoded, len(input), nil)

    // Signed delta encoding
    signedInput := []int32{-100, -98, -96, -94, -92}
    
    encoded = streamvbyte.DeltaEncodeInt32(signedInput, nil)
    decodedSigned := streamvbyte.DeltaDecodeInt32(encoded, len(signedInput), nil)
}
```

### Buffer Reuse

For better performance, you can reuse buffers across encoding/decoding operations:

```go
func processData(data []uint32) {
    var encBuf []byte
    var decBuf []uint32
    
    for {
        // Reuse the same buffers
        encBuf = streamvbyte.EncodeUint32(data, &streamvbyte.EncodeOptions[uint32]{
            Buffer: encBuf,
        })
        
        decBuf = streamvbyte.DecodeUint32(encBuf, len(data), &streamvbyte.DecodeOptions[uint32]{
            Buffer: decBuf,
        })
    }
}
```

## Benchmarks

The following table shows the benchmark results for different encoding and decoding operations on two different architectures: ARM64 (Graviton 2) and AMD64 (Intel Xeon & AMD EPYC). The results include both accelerated and non-accelerated (`noasm` tag) versions (median of 3 runs).

| Operation              | CPU        | Pure Go (MB/s) | SIMD (MB/s) | Speedup |
|------------------------|------------|----------------|-------------|---------|
| Encode/uint32/std      | Graviton 2 | 494.3          | 5397.3      | 10.9x   |
| Encode/uint32/alt      | Graviton 2 | 510.6          | 6263.2      | 12.3x   |
| Encode/int32/std       | Graviton 2 | 476.8          | 4129.7      | 8.7x    |
| Encode/int32/alt       | Graviton 2 | 470.4          | 4998.4      | 10.6x   |
| EncodeDelta/uint32/std | Graviton 2 | 607.7          | 4312.3      | 7.1x    |
| EncodeDelta/uint32/alt | Graviton 2 | 756.8          | 5142.9      | 6.8x    |
| EncodeDelta/int32/std  | Graviton 2 | 518.0          | 3365.1      | 6.5x    |
| EncodeDelta/int32/alt  | Graviton 2 | 565.4          | 3987.4      | 7.1x    |
| Decode/uint32/std      | Graviton 2 | 545.6          | 10459.1     | 19.2x   |
| Decode/uint32/alt      | Graviton 2 | 554.4          | 10334.0     | 18.6x   |
| Decode/int32/std       | Graviton 2 | 503.7          | 7223.9      | 14.3x   |
| Decode/int32/alt       | Graviton 2 | 549.2          | 7144.9      | 13.0x   |
| DecodeDelta/uint32/std | Graviton 2 | 1623.8         | 6889.6      | 4.2x    |
| DecodeDelta/uint32/alt | Graviton 2 | 1792.6         | 7248.1      | 4.0x    |
| DecodeDelta/int32/std  | Graviton 2 | 1400.9         | 4700.5      | 3.4x    |
| DecodeDelta/int32/alt  | Graviton 2 | 1529.9         | 4889.2      | 3.2x    |
| Encode/uint32/std      | Xeon 8375C | 565.0          | 13640.0     | 24.1x   |
| Encode/uint32/alt      | Xeon 8375C | 581.6          | 10116.5     | 17.4x   |
| Encode/int32/std       | Xeon 8375C | 541.8          | 10606.0     | 19.6x   |
| Encode/int32/alt       | Xeon 8375C | 554.3          | 8079.9      | 14.6x   |
| EncodeDelta/uint32/std | Xeon 8375C | 739.3          | 10812.9     | 14.6x   |
| EncodeDelta/uint32/alt | Xeon 8375C | 923.6          | 8255.8      | 8.9x    |
| EncodeDelta/int32/std  | Xeon 8375C | 619.6          | 8885.0      | 14.3x   |
| EncodeDelta/int32/alt  | Xeon 8375C | 719.7          | 6920.4      | 9.6x    |
| Decode/uint32/std      | Xeon 8375C | 590.4          | 19564.2     | 33.1x   |
| Decode/uint32/alt      | Xeon 8375C | 578.6          | 18347.3     | 31.7x   |
| Decode/int32/std       | Xeon 8375C | 577.0          | 13161.2     | 22.8x   |
| Decode/int32/alt       | Xeon 8375C | 577.5          | 13335.3     | 23.1x   |
| DecodeDelta/uint32/std | Xeon 8375C | 3580.7         | 23219.0     | 6.5x    |
| DecodeDelta/uint32/alt | Xeon 8375C | 3073.3         | 14771.3     | 4.8x    |
| DecodeDelta/int32/std  | Xeon 8375C | 2913.7         | 10991.0     | 3.8x    |
| DecodeDelta/int32/alt  | Xeon 8375C | 2826.0         | 11146.1     | 3.9x    |
| Encode/uint32/std      | EPYC 7R13  | 672.8          | 10561.5     | 15.7x   |
| Encode/uint32/alt      | EPYC 7R13  | 680.1          | 8225.2      | 12.1x   |
| Encode/int32/std       | EPYC 7R13  | 631.5          | 10292.8     | 16.3x   |
| Encode/int32/alt       | EPYC 7R13  | 638.6          | 7733.3      | 12.1x   |
| EncodeDelta/uint32/std | EPYC 7R13  | 859.0          | 10674.7     | 12.4x   |
| EncodeDelta/uint32/alt | EPYC 7R13  | 1050.5         | 7664.5      | 7.3x    |
| EncodeDelta/int32/std  | EPYC 7R13  | 717.0          | 10156.8     | 14.2x   |
| EncodeDelta/int32/alt  | EPYC 7R13  | 763.0          | 7329.1      | 9.6x    |
| Decode/uint32/std      | EPYC 7R13  | 697.7          | 21223.5     | 30.4x   |
| Decode/uint32/alt      | EPYC 7R13  | 755.1          | 22265.5     | 29.5x   |
| Decode/int32/std       | EPYC 7R13  | 685.9          | 17496.9     | 25.5x   |
| Decode/int32/alt       | EPYC 7R13  | 738.9          | 17809.0     | 24.1x   |
| DecodeDelta/uint32/std | EPYC 7R13  | 3125.0         | 27192.9     | 8.7x    |
| DecodeDelta/uint32/alt | EPYC 7R13  | 4031.6         | 18448.5     | 4.6x    |
| DecodeDelta/int32/std  | EPYC 7R13  | 2927.0         | 14262.0     | 4.9x    |
| DecodeDelta/int32/alt  | EPYC 7R13  | 3561.2         | 14648.7     | 4.1x    |
