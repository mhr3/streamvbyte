//go:build !noasm && (amd64 || arm64)

package streamvbyte

func mustDecodeAll(n, count int) {
	if n != count {
		panic("streamvbyte: truncated input")
	}
}
