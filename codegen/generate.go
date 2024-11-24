//go:generate go run github.com/mhr3/gocc/cmd/gocc@v0.9.2 c/zigzag.c -l -p streamvbyte -o ../ -a amd64 -O3 --suffix _amd64_sse --function-suffix _sse
//go:generate go run github.com/mhr3/gocc/cmd/gocc@v0.9.2 c/zigzag.c -l -p streamvbyte -o ../ -a avx2 -O3 --suffix _amd64_avx2 --function-suffix _avx2
//go:generate go run github.com/mhr3/gocc/cmd/gocc@v0.9.2 c/zigzag.c -l -p streamvbyte -o ../ -a arm64 -O3 --suffix _arm64
//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.12.0 c/svb_decode_neon.c -l -p streamvbyte -o ../ -a arm64 -O3 --suffix _arm64

package codegen
