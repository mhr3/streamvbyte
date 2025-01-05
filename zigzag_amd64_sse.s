//go:build !noasm && amd64
// Code generated by gocc rev-4e20446 -- DO NOT EDIT.
//
// Source file         : zigzag.c
// Clang version       : Apple clang version 16.0.0 (clang-1600.0.26.6)
// Target architecture : amd64
// Compiler options    : [none]

#include "textflag.h"

TEXT ·zigzag_encode_sse(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	WORD $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB0_11             // <--                                  // je	.LBB0_11
	NOP                      // (skipped)                            // push	rbp
	NOP                      // (skipped)                            // mov	rbp, rsp
	NOP                      // (skipped)                            // and	rsp, -8
	XORL AX, AX              // <--                                  // xor	eax, eax
	CMPQ DX, $0x8            // <--                                  // cmp	rdx, 8
	JB   LBB0_6              // <--                                  // jb	.LBB0_6
	MOVQ SI, CX              // <--                                  // mov	rcx, rsi
	SUBQ DI, CX              // <--                                  // sub	rcx, rdi
	CMPQ CX, $0x20           // <--                                  // cmp	rcx, 32
	JB   LBB0_6              // <--                                  // jb	.LBB0_6
	MOVQ DX, AX              // <--                                  // mov	rax, rdx
	ANDQ $-0x8, AX           // <--                                  // and	rax, -8
	XORL CX, CX              // <--                                  // xor	ecx, ecx

LBB0_4:
	LONG $0x046f0ff3; BYTE $0x8f   // MOVDQU 0(DI)(CX*4), X0               // movdqu	xmm0, xmmword ptr [rdi + 4*rcx]
	LONG $0x4c6f0ff3; WORD $0x108f // MOVDQU 0x10(DI)(CX*4), X1            // movdqu	xmm1, xmmword ptr [rdi + 4*rcx + 16]
	LONG $0xd06f0f66               // MOVDQA X0, X2                        // movdqa	xmm2, xmm0
	LONG $0xd0fe0f66               // PADDD X0, X2                         // paddd	xmm2, xmm0
	LONG $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	LONG $0xd9fe0f66               // PADDD X1, X3                         // paddd	xmm3, xmm1
	LONG $0xe0720f66; BYTE $0x1f   // PSRAD $0x1f, X0                      // psrad	xmm0, 31
	LONG $0xc2ef0f66               // PXOR X2, X0                          // pxor	xmm0, xmm2
	LONG $0xe1720f66; BYTE $0x1f   // PSRAD $0x1f, X1                      // psrad	xmm1, 31
	LONG $0xcbef0f66               // PXOR X3, X1                          // pxor	xmm1, xmm3
	LONG $0x047f0ff3; BYTE $0x8e   // MOVDQU X0, 0(SI)(CX*4)               // movdqu	xmmword ptr [rsi + 4*rcx], xmm0
	LONG $0x4c7f0ff3; WORD $0x108e // MOVDQU X1, 0x10(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 16], xmm1
	ADDQ $0x8, CX                  // <--                                  // add	rcx, 8
	CMPQ AX, CX                    // <--                                  // cmp	rax, rcx
	JNE  LBB0_4                    // <--                                  // jne	.LBB0_4
	CMPQ AX, DX                    // <--                                  // cmp	rax, rdx
	JE   LBB0_10                   // <--                                  // je	.LBB0_10

LBB0_6:
	MOVQ AX, CX              // <--                                  // mov	rcx, rax
	NOTQ CX                  // <--                                  // not	rcx
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB0_8              // <--                                  // je	.LBB0_8
	LONG $0x87048b44         // MOVL 0(DI)(AX*4), R8                 // mov	r8d, dword ptr [rdi + 4*rax]
	LONG $0x000c8d47         // LEAL 0(R8)(R8*1), R9                 // lea	r9d, [r8 + r8]
	LONG $0x1ff8c141         // SARL $0x1f, R8                       // sar	r8d, 31
	XORL R9, R8              // <--                                  // xor	r8d, r9d
	LONG $0x86048944         // MOVL R8, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r8d
	ORQ  $0x1, AX            // <--                                  // or	rax, 1

LBB0_8:
	ADDQ DX, CX  // <--                                  // add	rcx, rdx
	JE   LBB0_10 // <--                                  // je	.LBB0_10

LBB0_9:
	WORD $0x0c8b; BYTE $0x87 // MOVL 0(DI)(AX*4), CX                 // mov	ecx, dword ptr [rdi + 4*rax]
	LONG $0x09048d44         // LEAL 0(CX)(CX*1), R8                 // lea	r8d, [rcx + rcx]
	WORD $0xf9c1; BYTE $0x1f // SARL $0x1f, CX                       // sar	ecx, 31
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	WORD $0x0c89; BYTE $0x86 // MOVL CX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	LONG $0x09048d44         // LEAL 0(CX)(CX*1), R8                 // lea	r8d, [rcx + rcx]
	WORD $0xf9c1; BYTE $0x1f // SARL $0x1f, CX                       // sar	ecx, 31
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ DX, AX              // <--                                  // cmp	rdx, rax
	JNE  LBB0_9              // <--                                  // jne	.LBB0_9

LBB0_10:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp

LBB0_11:
	RET // <--                                  // ret

TEXT ·zigzag_delta_encode_sse(SB), NOSPLIT, $0-28
	MOVQ    in+0(FP), DI
	MOVQ    out+8(FP), SI
	MOVQ    N+16(FP), DX
	MOVLQZX prev+24(FP), CX
	WORD    $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE      LBB1_6              // <--                                  // je	.LBB1_6
	NOP                         // (skipped)                            // push	rbp
	NOP                         // (skipped)                            // mov	rbp, rsp
	NOP                         // (skipped)                            // and	rsp, -8
	CMPQ    DX, $0x1            // <--                                  // cmp	rdx, 1
	JNE     LBB1_7              // <--                                  // jne	.LBB1_7
	XORL    AX, AX              // <--                                  // xor	eax, eax
	JMP     LBB1_3              // <--                                  // jmp	.LBB1_3

LBB1_7:
	MOVQ DX, R8    // <--                                  // mov	r8, rdx
	ANDQ $-0x2, R8 // <--                                  // and	r8, -2
	XORL AX, AX    // <--                                  // xor	eax, eax

LBB1_8:
	LONG $0x870c8b44         // MOVL 0(DI)(AX*4), R9                 // mov	r9d, dword ptr [rdi + 4*rax]
	WORD $0x2941; BYTE $0xc9 // SUBL CX, R9                          // sub	r9d, ecx
	LONG $0x090c8d43         // LEAL 0(R9)(R9*1), CX                 // lea	ecx, [r9 + r9]
	LONG $0x1ff9c141         // SARL $0x1f, R9                       // sar	r9d, 31
	XORL CX, R9              // <--                                  // xor	r9d, ecx
	LONG $0x860c8944         // MOVL R9, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r9d
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x0c2b; BYTE $0x87 // SUBL 0(DI)(AX*4), CX                 // sub	ecx, dword ptr [rdi + 4*rax]
	LONG $0x090c8d44         // LEAL 0(CX)(CX*1), R9                 // lea	r9d, [rcx + rcx]
	WORD $0xf9c1; BYTE $0x1f // SARL $0x1f, CX                       // sar	ecx, 31
	XORL R9, CX              // <--                                  // xor	ecx, r9d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ R8, AX              // <--                                  // cmp	r8, rax
	JNE  LBB1_8              // <--                                  // jne	.LBB1_8

LBB1_3:
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB1_5              // <--                                  // je	.LBB1_5
	WORD $0x148b; BYTE $0x87 // MOVL 0(DI)(AX*4), DX                 // mov	edx, dword ptr [rdi + 4*rax]
	WORD $0xca29             // SUBL CX, DX                          // sub	edx, ecx
	WORD $0x0c8d; BYTE $0x12 // LEAL 0(DX)(DX*1), CX                 // lea	ecx, [rdx + rdx]
	WORD $0xfac1; BYTE $0x1f // SARL $0x1f, DX                       // sar	edx, 31
	XORL CX, DX              // <--                                  // xor	edx, ecx
	WORD $0x1489; BYTE $0x86 // MOVL DX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], edx

LBB1_5:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp

LBB1_6:
	RET // <--                                  // ret

DATA LCPI2_0<>+0x00(SB)/4, $0x00000001
DATA LCPI2_0<>+0x04(SB)/4, $0x00000001
DATA LCPI2_0<>+0x08(SB)/4, $0x00000001
DATA LCPI2_0<>+0x0c(SB)/4, $0x00000001
GLOBL LCPI2_0<>(SB), (RODATA|NOPTR), $16

TEXT ·zigzag_decode_sse(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	WORD $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB2_11             // <--                                  // je	.LBB2_11
	NOP                      // (skipped)                            // push	rbp
	NOP                      // (skipped)                            // mov	rbp, rsp
	NOP                      // (skipped)                            // and	rsp, -8
	XORL AX, AX              // <--                                  // xor	eax, eax
	CMPQ DX, $0x8            // <--                                  // cmp	rdx, 8
	JB   LBB2_6              // <--                                  // jb	.LBB2_6
	MOVQ SI, CX              // <--                                  // mov	rcx, rsi
	SUBQ DI, CX              // <--                                  // sub	rcx, rdi
	CMPQ CX, $0x20           // <--                                  // cmp	rcx, 32
	JB   LBB2_6              // <--                                  // jb	.LBB2_6
	MOVQ DX, AX              // <--                                  // mov	rax, rdx
	ANDQ $-0x8, AX           // <--                                  // and	rax, -8
	XORL CX, CX              // <--                                  // xor	ecx, ecx
	MOVO LCPI2_0<>(SB), X0   // <--                                  // movdqa	xmm0, xmmword ptr [rip + .LCPI2_0]

LBB2_4:
	LONG $0x0c6f0ff3; BYTE $0x8f   // MOVDQU 0(DI)(CX*4), X1               // movdqu	xmm1, xmmword ptr [rdi + 4*rcx]
	LONG $0x546f0ff3; WORD $0x108f // MOVDQU 0x10(DI)(CX*4), X2            // movdqu	xmm2, xmmword ptr [rdi + 4*rcx + 16]
	LONG $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	LONG $0xd3720f66; BYTE $0x01   // PSRLD $0x1, X3                       // psrld	xmm3, 1
	LONG $0xe26f0f66               // MOVDQA X2, X4                        // movdqa	xmm4, xmm2
	LONG $0xd4720f66; BYTE $0x01   // PSRLD $0x1, X4                       // psrld	xmm4, 1
	LONG $0xc8db0f66               // PAND X0, X1                          // pand	xmm1, xmm0
	LONG $0xd0db0f66               // PAND X0, X2                          // pand	xmm2, xmm0
	LONG $0xedef0f66               // PXOR X5, X5                          // pxor	xmm5, xmm5
	LONG $0xe9fa0f66               // PSUBD X1, X5                         // psubd	xmm5, xmm1
	LONG $0xebef0f66               // PXOR X3, X5                          // pxor	xmm5, xmm3
	LONG $0xc9ef0f66               // PXOR X1, X1                          // pxor	xmm1, xmm1
	LONG $0xcafa0f66               // PSUBD X2, X1                         // psubd	xmm1, xmm2
	LONG $0xccef0f66               // PXOR X4, X1                          // pxor	xmm1, xmm4
	LONG $0x2c7f0ff3; BYTE $0x8e   // MOVDQU X5, 0(SI)(CX*4)               // movdqu	xmmword ptr [rsi + 4*rcx], xmm5
	LONG $0x4c7f0ff3; WORD $0x108e // MOVDQU X1, 0x10(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 16], xmm1
	ADDQ $0x8, CX                  // <--                                  // add	rcx, 8
	CMPQ AX, CX                    // <--                                  // cmp	rax, rcx
	JNE  LBB2_4                    // <--                                  // jne	.LBB2_4
	CMPQ AX, DX                    // <--                                  // cmp	rax, rdx
	JE   LBB2_10                   // <--                                  // je	.LBB2_10

LBB2_6:
	MOVQ AX, CX              // <--                                  // mov	rcx, rax
	NOTQ CX                  // <--                                  // not	rcx
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB2_8              // <--                                  // je	.LBB2_8
	LONG $0x87048b44         // MOVL 0(DI)(AX*4), R8                 // mov	r8d, dword ptr [rdi + 4*rax]
	WORD $0x8945; BYTE $0xc1 // MOVL R8, R9                          // mov	r9d, r8d
	WORD $0xd141; BYTE $0xe9 // SHRL $0x1, R9                        // shr	r9d
	LONG $0x01e08341         // ANDL $0x1, R8                        // and	r8d, 1
	WORD $0xf741; BYTE $0xd8 // NEGL R8                              // neg	r8d
	XORL R9, R8              // <--                                  // xor	r8d, r9d
	LONG $0x86048944         // MOVL R8, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r8d
	ORQ  $0x1, AX            // <--                                  // or	rax, 1

LBB2_8:
	ADDQ DX, CX  // <--                                  // add	rcx, rdx
	JE   LBB2_10 // <--                                  // je	.LBB2_10

LBB2_9:
	WORD $0x0c8b; BYTE $0x87 // MOVL 0(DI)(AX*4), CX                 // mov	ecx, dword ptr [rdi + 4*rax]
	WORD $0x8941; BYTE $0xc8 // MOVL CX, R8                          // mov	r8d, ecx
	WORD $0xd141; BYTE $0xe8 // SHRL $0x1, R8                        // shr	r8d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	WORD $0xd9f7             // NEGL CX                              // neg	ecx
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	WORD $0x0c89; BYTE $0x86 // MOVL CX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x8941; BYTE $0xc8 // MOVL CX, R8                          // mov	r8d, ecx
	WORD $0xd141; BYTE $0xe8 // SHRL $0x1, R8                        // shr	r8d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	WORD $0xd9f7             // NEGL CX                              // neg	ecx
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ DX, AX              // <--                                  // cmp	rdx, rax
	JNE  LBB2_9              // <--                                  // jne	.LBB2_9

LBB2_10:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp

LBB2_11:
	RET // <--                                  // ret

TEXT ·zigzag_delta_decode_sse(SB), NOSPLIT, $0-28
	MOVQ    in+0(FP), DI
	MOVQ    out+8(FP), SI
	MOVQ    N+16(FP), DX
	MOVLQZX prev+24(FP), CX
	WORD    $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE      LBB3_6              // <--                                  // je	.LBB3_6
	NOP                         // (skipped)                            // push	rbp
	NOP                         // (skipped)                            // mov	rbp, rsp
	NOP                         // (skipped)                            // and	rsp, -8
	CMPQ    DX, $0x1            // <--                                  // cmp	rdx, 1
	JNE     LBB3_7              // <--                                  // jne	.LBB3_7
	XORL    AX, AX              // <--                                  // xor	eax, eax
	JMP     LBB3_3              // <--                                  // jmp	.LBB3_3

LBB3_7:
	MOVQ DX, R8    // <--                                  // mov	r8, rdx
	ANDQ $-0x2, R8 // <--                                  // and	r8, -2
	XORL AX, AX    // <--                                  // xor	eax, eax

LBB3_8:
	LONG $0x870c8b44         // MOVL 0(DI)(AX*4), R9                 // mov	r9d, dword ptr [rdi + 4*rax]
	WORD $0x8945; BYTE $0xca // MOVL R9, R10                         // mov	r10d, r9d
	WORD $0xd141; BYTE $0xea // SHRL $0x1, R10                       // shr	r10d
	LONG $0x01e18341         // ANDL $0x1, R9                        // and	r9d, 1
	WORD $0xf741; BYTE $0xd9 // NEGL R9                              // neg	r9d
	XORL R10, R9             // <--                                  // xor	r9d, r10d
	WORD $0x0141; BYTE $0xc9 // ADDL CX, R9                          // add	r9d, ecx
	LONG $0x860c8944         // MOVL R9, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r9d
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x8941; BYTE $0xca // MOVL CX, R10                         // mov	r10d, ecx
	WORD $0xd141; BYTE $0xea // SHRL $0x1, R10                       // shr	r10d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	WORD $0xd9f7             // NEGL CX                              // neg	ecx
	XORL R10, CX             // <--                                  // xor	ecx, r10d
	WORD $0x0144; BYTE $0xc9 // ADDL R9, CX                          // add	ecx, r9d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ R8, AX              // <--                                  // cmp	r8, rax
	JNE  LBB3_8              // <--                                  // jne	.LBB3_8

LBB3_3:
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB3_5              // <--                                  // je	.LBB3_5
	WORD $0x148b; BYTE $0x87 // MOVL 0(DI)(AX*4), DX                 // mov	edx, dword ptr [rdi + 4*rax]
	WORD $0xd789             // MOVL DX, DI                          // mov	edi, edx
	WORD $0xefd1             // SHRL $0x1, DI                        // shr	edi
	WORD $0xe283; BYTE $0x01 // ANDL $0x1, DX                        // and	edx, 1
	WORD $0xdaf7             // NEGL DX                              // neg	edx
	XORL DI, DX              // <--                                  // xor	edx, edi
	WORD $0xca01             // ADDL CX, DX                          // add	edx, ecx
	WORD $0x1489; BYTE $0x86 // MOVL DX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], edx

LBB3_5:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp

LBB3_6:
	RET // <--                                  // ret
