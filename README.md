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

The following table shows the benchmark results for different encoding and decoding operations on two different architectures: AWS Graviton 2 (ARM64) and Intel Xeon Platinum 8375C (AMD64). The results include both accelerated and non-accelerated (noasm tag) versions.

| Operation              | CPU        | Pure Go (MB/s) | SIMD (MB/s) | Speedup |
|------------------------|------------|----------------|-------------|---------|
| Encode/uint32/std      | Graviton 2 | 498.2          | 5429.4      | 10.9x   |
| Encode/uint32/alt	     | Graviton 2 | 491.1          | 6260.5      | 12.7x   |
| Encode/int32/std	     | Graviton 2 | 462.1          | 4156.4      | 9.0x    |
| Encode/int32/alt 	     | Graviton 2 | 491.2          | 5060.6      | 10.3x   |
| EncodeDelta/uint32/std | Graviton 2 | 615.9          | 4325.9      | 7.0x    |
| EncodeDelta/uint32/alt | Graviton 2 | 759.7          | 5188.5      | 6.8x    |
| EncodeDelta/int32/std  | Graviton 2 | 497.7          | 3399.9      | 6.8x    |
| EncodeDelta/int32/alt  | Graviton 2 | 553.8          | 4011.3      | 7.2x    |
| Decode/uint32/std      | Graviton 2 | 539.0          | 10430.7     | 19.4x   |
| Decode/uint32/alt      | Graviton 2 | 560.3          | 10359.0     | 18.5x   |
| Decode/int32/std       | Graviton 2 | 508.5          | 7181.9      | 14.1x   |
| Decode/int32/alt       | Graviton 2 | 537.9          | 7154.2      | 13.3x   |
| DecodeDelta/uint32/std | Graviton 2 | 1619.9         | 6868.5      | 4.2x    |
| DecodeDelta/uint32/alt | Graviton 2 | 1799.6         | 7267.6      | 4.0x    |
| DecodeDelta/int32/std  | Graviton 2 | 1389.3         | 4690.2      | 3.4x    |
| DecodeDelta/int32/alt  | Graviton 2 | 1495.8         | 4896.1      | 3.3x    |
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
