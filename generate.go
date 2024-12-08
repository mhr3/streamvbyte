//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.15.0 csrc/zigzag.c -l -p streamvbyte -o ./ -a amd64 -O3 --suffix _amd64_sse --function-suffix _sse
//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.15.0 csrc/zigzag.c -l -p streamvbyte -o ./ -a avx2 -O3 --suffix _amd64_avx2 --function-suffix _avx2
//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.15.0 csrc/zigzag.c -l -p streamvbyte -o ./ -a arm64 -O3 --suffix _arm64
//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.15.0 csrc/svb_sse41.c -l -p streamvbyte -o ./ -a amd64 -m sse4.1 -O3 --suffix _amd64
//go:generate go run github.com/mhr3/goruntool@v0.1.1 github.com/mhr3/gocc/cmd/gocc@v0.15.0 csrc/svb_neon.c -l -p streamvbyte -o ./ -a arm64 -O3 --suffix _arm64

package streamvbyte
