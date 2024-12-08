# streamvbyte

StreamVByte is a high-performance integer compression library for encoding and decoding streams of 32bit integers. It supports multiple architectures and implements two encoding schemes: the standard 1234 scheme (using between 1-4 bytes to represent the integers) suitable for most data, and the alternative 0124 scheme, which is suitable for data with lots of zeroes.

This library is based on two repositories:

- [lemire/streamvbyte](https://github.com/lemire/streamvbyte)
- [bmkessler/streamvbyte](https://github.com/bmkessler/streamvbyte)

The C code from lemire's codebase has been adjusted and transpiled to Go using [gocc](https://github.com/mhr3/gocc). Architectures other than amd64 and arm64 use a pure Go implementation based on bmkessler's code.

## Features

- High-speed encoding and decoding of integer streams.
- Support for multiple architectures including x86 (SSE4.1) and ARM (NEON).
- Optimized encoding schemes for different data patterns.

## Installation

To install the library, use `go get`:

```sh
go get github.com/mhr3/streamvbyte
```

## Usage

### Encoding and Decoding

The library provides two main encoding schemes: `StdEncoding` (uses 1234 scheme) and `AltEncoding` (0124 scheme).

```go
package main

import (
	"github.com/mhr3/streamvbyte"
)

func main() {
	input := []uint32{1, 2, 3, 4, 5}

	encoded := streamvbyte.StdEncoding.Encode(input, nil)
	decoded := streamvbyte.StdEncoding.Decode(encoded, len(input), nil)
}
```

### Delta Encoding

Delta encoding is also supported for compressing sequences of integers with small differences.

```go
package main

import (
	"github.com/mhr3/streamvbyte"
)

func main() {
	input := []uint32{100, 101, 102, 103, 104}

	encoded := streamvbyte.StdEncoding.EncodeDelta(input, nil, 0)
	decoded := streamvbyte.StdEncoding.DecodeDelta(encoded, len(input), nil, 0)
}
```

### Working with signed integers

The encoding methods only support slices of `uint32`, but you can use ZigZag encoding to convert int32 to uint32 slice suitable for encoding.

```go
package main

import (
	"github.com/mhr3/streamvbyte"
)

func main() {
	input := []int32{-1, -2, -3, -4, -5}

	zzInput := streamvbyte.ZigZag.Encode(input, nil)
	encoded := streamvbyte.StdEncoding.Encode(zzInput, nil)

	decoded := streamvbyte.StdEncoding.Decode(encoded, len(input), nil)
	output := streamvbyte.ZigZag.Decode(decoded, nil)
}
```

## Benchmarks

The following table shows the benchmark results for different encoding and decoding operations on two different architectures: AWS Graviton 2 (ARM64) and Intel Xeon Platinum 8375C (AMD64). The results include both accelerated and non-accelerated (noasm tag) versions.

| Benchmark          | CPU        | Accelerated (MB/s) | Pure Go (MB/s) | Speedup |
| ------------------ | ---------- | ------------------ | -------------- | ------- |
| Encode/std         | Graviton 2 | 5488.56            | 523.83         | 10.48x  |
| Encode/alt         | Graviton 2 | 5731.03            | 509.97         | 11.24x  |
| EncodeDelta/std    | Graviton 2 | 4305.64            | 1577.83        | 2.73x   |
| EncodeDelta/alt    | Graviton 2 | 5024.83            | 1439.92        | 3.49x   |
| Decode/std         | Graviton 2 | 10387.50           | 548.61         | 18.93x  |
| Decode/alt         | Graviton 2 | 9942.57            | 561.46         | 17.71x  |
| DecodeDelta/std    | Graviton 2 | 6157.40            | 1680.90        | 3.66x   |
| DecodeDelta/alt    | Graviton 2 | 6196.00            | 1855.46        | 3.34x   |
| ZigZag/Encode      | Graviton 2 | 18823.22           | 4173.77        | 4.51x   |
| ZigZag/Decode      | Graviton 2 | 15200.00           | 3279.09        | 4.63x   |
| ZigZagDelta/Encode | Graviton 2 | 3651.04            | 3306.59        | 1.10x   |
| ZigZagDelta/Decode | Graviton 2 | 3375.70            | 2960.41        | 1.14x   |
| Encode/std         | Xeon 8375C | 13097.79           | 557.21         | 23.52x  |
| Encode/alt         | Xeon 8375C | 9988.15            | 574.44         | 17.39x  |
| EncodeDelta/std    | Xeon 8375C | 10778.95           | 2777.15        | 3.88x   |
| EncodeDelta/alt    | Xeon 8375C | 8244.99            | 2139.46        | 3.85x   |
| Decode/std         | Xeon 8375C | 20482.06           | 556.66         | 36.79x  |
| Decode/alt         | Xeon 8375C | 23190.89           | 601.42         | 38.56x  |
| DecodeDelta/std    | Xeon 8375C | 21624.97           | 2786.24        | 7.76x   |
| DecodeDelta/alt    | Xeon 8375C | 15166.37           | 2993.66        | 5.06x   |
| ZigZag/Encode      | Xeon 8375C | 21579.60           | 6440.37        | 3.35x   |
| ZigZag/Decode      | Xeon 8375C | 15592.41           | 6917.33        | 2.25x   |
| ZigZagDelta/Encode | Xeon 8375C | 6525.11            | 5472.73        | 1.19x   |
| ZigZagDelta/Decode | Xeon 8375C | 6940.30            | 5082.88        | 1.36x   |
