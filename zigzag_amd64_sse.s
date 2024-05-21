//go:build !noasm && amd64
// Code generated by gocc v0.9.2 -- DO NOT EDIT.
//
// Source file         : zigzag.c
// Clang version       : Apple clang version 15.0.0 (clang-1500.3.9.4)
// Target architecture : amd64
// Compiler options    : [none]

#include "textflag.h"

TEXT ·zigzag_encode_sse(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	NOP                      // (skipped)                            // push	rbp
	NOP                      // (skipped)                            // mov	rbp, rsp
	NOP                      // (skipped)                            // and	rsp, -8
	WORD $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB0_14             // <--                                  // je	.LBB0_14
	XORL AX, AX              // <--                                  // xor	eax, eax
	CMPQ DX, $0x8            // <--                                  // cmp	rdx, 8
	JB   LBB0_10             // <--                                  // jb	.LBB0_10
	MOVQ SI, CX              // <--                                  // mov	rcx, rsi
	SUBQ DI, CX              // <--                                  // sub	rcx, rdi
	CMPQ CX, $0x20           // <--                                  // cmp	rcx, 32
	JB   LBB0_10             // <--                                  // jb	.LBB0_10
	LEAQ -0x8(DX), AX        // <--                                  // lea	rax, [rdx - 8]
	MOVQ AX, R8              // <--                                  // mov	r8, rax
	SHRQ $0x3, R8            // <--                                  // shr	r8, 3
	INCQ R8                  // <--                                  // inc	r8
	CMPQ AX, $0x8            // <--                                  // cmp	rax, 8
	JAE  LBB0_5              // <--                                  // jae	.LBB0_5
	XORL CX, CX              // <--                                  // xor	ecx, ecx
	JMP  LBB0_7              // <--                                  // jmp	.LBB0_7

LBB0_5:
	MOVQ R8, AX    // <--                                  // mov	rax, r8
	ANDQ $-0x2, AX // <--                                  // and	rax, -2
	XORL CX, CX    // <--                                  // xor	ecx, ecx

LBB0_6:
	LONG  $0x046f0ff3; BYTE $0x8f   // MOVDQU 0(DI)(CX*4), X0               // movdqu	xmm0, xmmword ptr [rdi + 4*rcx]
	LONG  $0x4c6f0ff3; WORD $0x108f // MOVDQU 0x10(DI)(CX*4), X1            // movdqu	xmm1, xmmword ptr [rdi + 4*rcx + 16]
	LONG  $0xd06f0f66               // MOVDQA X0, X2                        // movdqa	xmm2, xmm0
	PADDD X0, X2                    // <--                                  // paddd	xmm2, xmm0
	LONG  $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	PADDD X1, X3                    // <--                                  // paddd	xmm3, xmm1
	LONG  $0xe0720f66; BYTE $0x1f   // PSRAD $0x1f, X0                      // psrad	xmm0, 31
	PXOR  X2, X0                    // <--                                  // pxor	xmm0, xmm2
	LONG  $0xe1720f66; BYTE $0x1f   // PSRAD $0x1f, X1                      // psrad	xmm1, 31
	PXOR  X3, X1                    // <--                                  // pxor	xmm1, xmm3
	LONG  $0x047f0ff3; BYTE $0x8e   // MOVDQU X0, 0(SI)(CX*4)               // movdqu	xmmword ptr [rsi + 4*rcx], xmm0
	LONG  $0x4c7f0ff3; WORD $0x108e // MOVDQU X1, 0x10(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 16], xmm1
	LONG  $0x446f0ff3; WORD $0x208f // MOVDQU 0x20(DI)(CX*4), X0            // movdqu	xmm0, xmmword ptr [rdi + 4*rcx + 32]
	LONG  $0x4c6f0ff3; WORD $0x308f // MOVDQU 0x30(DI)(CX*4), X1            // movdqu	xmm1, xmmword ptr [rdi + 4*rcx + 48]
	LONG  $0xd06f0f66               // MOVDQA X0, X2                        // movdqa	xmm2, xmm0
	PADDD X0, X2                    // <--                                  // paddd	xmm2, xmm0
	LONG  $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	PADDD X1, X3                    // <--                                  // paddd	xmm3, xmm1
	LONG  $0xe0720f66; BYTE $0x1f   // PSRAD $0x1f, X0                      // psrad	xmm0, 31
	PXOR  X2, X0                    // <--                                  // pxor	xmm0, xmm2
	LONG  $0xe1720f66; BYTE $0x1f   // PSRAD $0x1f, X1                      // psrad	xmm1, 31
	PXOR  X3, X1                    // <--                                  // pxor	xmm1, xmm3
	LONG  $0x447f0ff3; WORD $0x208e // MOVDQU X0, 0x20(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 32], xmm0
	LONG  $0x4c7f0ff3; WORD $0x308e // MOVDQU X1, 0x30(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 48], xmm1
	ADDQ  $0x10, CX                 // <--                                  // add	rcx, 16
	ADDQ  $-0x2, AX                 // <--                                  // add	rax, -2
	JNE   LBB0_6                    // <--                                  // jne	.LBB0_6

LBB0_7:
	MOVQ  DX, AX                    // <--                                  // mov	rax, rdx
	ANDQ  $-0x8, AX                 // <--                                  // and	rax, -8
	LONG  $0x01c0f641               // TESTL $0x1, R8                       // test	r8b, 1
	JE    LBB0_9                    // <--                                  // je	.LBB0_9
	LONG  $0x046f0ff3; BYTE $0x8f   // MOVDQU 0(DI)(CX*4), X0               // movdqu	xmm0, xmmword ptr [rdi + 4*rcx]
	LONG  $0x4c6f0ff3; WORD $0x108f // MOVDQU 0x10(DI)(CX*4), X1            // movdqu	xmm1, xmmword ptr [rdi + 4*rcx + 16]
	LONG  $0xd06f0f66               // MOVDQA X0, X2                        // movdqa	xmm2, xmm0
	PADDD X0, X2                    // <--                                  // paddd	xmm2, xmm0
	LONG  $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	PADDD X1, X3                    // <--                                  // paddd	xmm3, xmm1
	LONG  $0xe0720f66; BYTE $0x1f   // PSRAD $0x1f, X0                      // psrad	xmm0, 31
	PXOR  X2, X0                    // <--                                  // pxor	xmm0, xmm2
	LONG  $0xe1720f66; BYTE $0x1f   // PSRAD $0x1f, X1                      // psrad	xmm1, 31
	PXOR  X3, X1                    // <--                                  // pxor	xmm1, xmm3
	LONG  $0x047f0ff3; BYTE $0x8e   // MOVDQU X0, 0(SI)(CX*4)               // movdqu	xmmword ptr [rsi + 4*rcx], xmm0
	LONG  $0x4c7f0ff3; WORD $0x108e // MOVDQU X1, 0x10(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 16], xmm1

LBB0_9:
	CMPQ AX, DX  // <--                                  // cmp	rax, rdx
	JE   LBB0_14 // <--                                  // je	.LBB0_14

LBB0_10:
	MOVQ AX, CX              // <--                                  // mov	rcx, rax
	NOTQ CX                  // <--                                  // not	rcx
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB0_12             // <--                                  // je	.LBB0_12
	LONG $0x87048b44         // MOVL 0(DI)(AX*4), R8                 // mov	r8d, dword ptr [rdi + 4*rax]
	LEAL 0(R8)(R8*1), R9     // <--                                  // lea	r9d, [r8 + r8]
	SARL $0x1f, R8           // <--                                  // sar	r8d, 31
	XORL R9, R8              // <--                                  // xor	r8d, r9d
	LONG $0x86048944         // MOVL R8, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r8d
	ORQ  $0x1, AX            // <--                                  // or	rax, 1

LBB0_12:
	ADDQ DX, CX  // <--                                  // add	rcx, rdx
	JE   LBB0_14 // <--                                  // je	.LBB0_14

LBB0_13:
	WORD $0x0c8b; BYTE $0x87 // MOVL 0(DI)(AX*4), CX                 // mov	ecx, dword ptr [rdi + 4*rax]
	LEAL 0(CX)(CX*1), R8     // <--                                  // lea	r8d, [rcx + rcx]
	SARL $0x1f, CX           // <--                                  // sar	ecx, 31
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	WORD $0x0c89; BYTE $0x86 // MOVL CX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	LEAL 0(CX)(CX*1), R8     // <--                                  // lea	r8d, [rcx + rcx]
	SARL $0x1f, CX           // <--                                  // sar	ecx, 31
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ DX, AX              // <--                                  // cmp	rdx, rax
	JNE  LBB0_13             // <--                                  // jne	.LBB0_13

LBB0_14:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp
	RET // <--                                  // ret

TEXT ·zigzag_delta_encode_sse(SB), NOSPLIT, $0-28
	MOVQ    in+0(FP), DI
	MOVQ    out+8(FP), SI
	MOVQ    N+16(FP), DX
	MOVLQZX prev+24(FP), CX
	NOP                         // (skipped)                            // push	rbp
	NOP                         // (skipped)                            // mov	rbp, rsp
	NOP                         // (skipped)                            // and	rsp, -8
	WORD    $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE      LBB1_5              // <--                                  // je	.LBB1_5
	CMPQ    DX, $0x1            // <--                                  // cmp	rdx, 1
	JNE     LBB1_6              // <--                                  // jne	.LBB1_6
	XORL    AX, AX              // <--                                  // xor	eax, eax
	JMP     LBB1_3              // <--                                  // jmp	.LBB1_3

LBB1_6:
	MOVQ DX, R8    // <--                                  // mov	r8, rdx
	ANDQ $-0x2, R8 // <--                                  // and	r8, -2
	XORL AX, AX    // <--                                  // xor	eax, eax

LBB1_7:
	LONG $0x870c8b44         // MOVL 0(DI)(AX*4), R9                 // mov	r9d, dword ptr [rdi + 4*rax]
	WORD $0x2941; BYTE $0xc9 // SUBL CX, R9                          // sub	r9d, ecx
	LEAL 0(R9)(R9*1), CX     // <--                                  // lea	ecx, [r9 + r9]
	SARL $0x1f, R9           // <--                                  // sar	r9d, 31
	XORL CX, R9              // <--                                  // xor	r9d, ecx
	LONG $0x860c8944         // MOVL R9, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r9d
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x0c2b; BYTE $0x87 // SUBL 0(DI)(AX*4), CX                 // sub	ecx, dword ptr [rdi + 4*rax]
	LEAL 0(CX)(CX*1), R9     // <--                                  // lea	r9d, [rcx + rcx]
	SARL $0x1f, CX           // <--                                  // sar	ecx, 31
	XORL R9, CX              // <--                                  // xor	ecx, r9d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ R8, AX              // <--                                  // cmp	r8, rax
	JNE  LBB1_7              // <--                                  // jne	.LBB1_7

LBB1_3:
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB1_5              // <--                                  // je	.LBB1_5
	WORD $0x148b; BYTE $0x87 // MOVL 0(DI)(AX*4), DX                 // mov	edx, dword ptr [rdi + 4*rax]
	WORD $0xca29             // SUBL CX, DX                          // sub	edx, ecx
	LEAL 0(DX)(DX*1), CX     // <--                                  // lea	ecx, [rdx + rdx]
	SARL $0x1f, DX           // <--                                  // sar	edx, 31
	XORL CX, DX              // <--                                  // xor	edx, ecx
	WORD $0x1489; BYTE $0x86 // MOVL DX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], edx

LBB1_5:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp
	RET // <--                                  // ret

LCPI2_0:

DATA LCPI2_0<>+0x00(SB)/4, $0x0001
DATA LCPI2_0<>+0x04(SB)/4, $0x0001
DATA LCPI2_0<>+0x08(SB)/4, $0x0001
DATA LCPI2_0<>+0x0c(SB)/4, $0x0001
GLOBL LCPI2_0<>(SB), (RODATA|NOPTR), $16

TEXT ·zigzag_decode_sse(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	NOP                      // (skipped)                            // push	rbp
	NOP                      // (skipped)                            // mov	rbp, rsp
	NOP                      // (skipped)                            // and	rsp, -8
	WORD $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB2_10             // <--                                  // je	.LBB2_10
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
	LONG  $0x0c6f0ff3; BYTE $0x8f   // MOVDQU 0(DI)(CX*4), X1               // movdqu	xmm1, xmmword ptr [rdi + 4*rcx]
	LONG  $0x546f0ff3; WORD $0x108f // MOVDQU 0x10(DI)(CX*4), X2            // movdqu	xmm2, xmmword ptr [rdi + 4*rcx + 16]
	LONG  $0xd96f0f66               // MOVDQA X1, X3                        // movdqa	xmm3, xmm1
	LONG  $0xd3720f66; BYTE $0x01   // PSRLD $0x1, X3                       // psrld	xmm3, 1
	LONG  $0xe26f0f66               // MOVDQA X2, X4                        // movdqa	xmm4, xmm2
	LONG  $0xd4720f66; BYTE $0x01   // PSRLD $0x1, X4                       // psrld	xmm4, 1
	PAND  X0, X1                    // <--                                  // pand	xmm1, xmm0
	PAND  X0, X2                    // <--                                  // pand	xmm2, xmm0
	PXOR  X5, X5                    // <--                                  // pxor	xmm5, xmm5
	PSUBD X1, X5                    // <--                                  // psubd	xmm5, xmm1
	PXOR  X3, X5                    // <--                                  // pxor	xmm5, xmm3
	PXOR  X1, X1                    // <--                                  // pxor	xmm1, xmm1
	PSUBD X2, X1                    // <--                                  // psubd	xmm1, xmm2
	PXOR  X4, X1                    // <--                                  // pxor	xmm1, xmm4
	LONG  $0x2c7f0ff3; BYTE $0x8e   // MOVDQU X5, 0(SI)(CX*4)               // movdqu	xmmword ptr [rsi + 4*rcx], xmm5
	LONG  $0x4c7f0ff3; WORD $0x108e // MOVDQU X1, 0x10(SI)(CX*4)            // movdqu	xmmword ptr [rsi + 4*rcx + 16], xmm1
	ADDQ  $0x8, CX                  // <--                                  // add	rcx, 8
	CMPQ  AX, CX                    // <--                                  // cmp	rax, rcx
	JNE   LBB2_4                    // <--                                  // jne	.LBB2_4
	CMPQ  AX, DX                    // <--                                  // cmp	rax, rdx
	JE    LBB2_10                   // <--                                  // je	.LBB2_10

LBB2_6:
	MOVQ AX, CX              // <--                                  // mov	rcx, rax
	NOTQ CX                  // <--                                  // not	rcx
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB2_8              // <--                                  // je	.LBB2_8
	LONG $0x87048b44         // MOVL 0(DI)(AX*4), R8                 // mov	r8d, dword ptr [rdi + 4*rax]
	WORD $0x8945; BYTE $0xc1 // MOVL R8, R9                          // mov	r9d, r8d
	SHRL $0x1, R9            // <--                                  // shr	r9d
	LONG $0x01e08341         // ANDL $0x1, R8                        // and	r8d, 1
	NEGL R8                  // <--                                  // neg	r8d
	XORL R9, R8              // <--                                  // xor	r8d, r9d
	LONG $0x86048944         // MOVL R8, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r8d
	ORQ  $0x1, AX            // <--                                  // or	rax, 1

LBB2_8:
	ADDQ DX, CX  // <--                                  // add	rcx, rdx
	JE   LBB2_10 // <--                                  // je	.LBB2_10

LBB2_9:
	WORD $0x0c8b; BYTE $0x87 // MOVL 0(DI)(AX*4), CX                 // mov	ecx, dword ptr [rdi + 4*rax]
	WORD $0x8941; BYTE $0xc8 // MOVL CX, R8                          // mov	r8d, ecx
	SHRL $0x1, R8            // <--                                  // shr	r8d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	NEGL CX                  // <--                                  // neg	ecx
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	WORD $0x0c89; BYTE $0x86 // MOVL CX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], ecx
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x8941; BYTE $0xc8 // MOVL CX, R8                          // mov	r8d, ecx
	SHRL $0x1, R8            // <--                                  // shr	r8d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	NEGL CX                  // <--                                  // neg	ecx
	XORL R8, CX              // <--                                  // xor	ecx, r8d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ DX, AX              // <--                                  // cmp	rdx, rax
	JNE  LBB2_9              // <--                                  // jne	.LBB2_9

LBB2_10:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp
	RET // <--                                  // ret

TEXT ·zigzag_delta_decode_sse(SB), NOSPLIT, $0-28
	MOVQ    in+0(FP), DI
	MOVQ    out+8(FP), SI
	MOVQ    N+16(FP), DX
	MOVLQZX prev+24(FP), CX
	NOP                         // (skipped)                            // push	rbp
	NOP                         // (skipped)                            // mov	rbp, rsp
	NOP                         // (skipped)                            // and	rsp, -8
	WORD    $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE      LBB3_5              // <--                                  // je	.LBB3_5
	CMPQ    DX, $0x1            // <--                                  // cmp	rdx, 1
	JNE     LBB3_6              // <--                                  // jne	.LBB3_6
	XORL    AX, AX              // <--                                  // xor	eax, eax
	JMP     LBB3_3              // <--                                  // jmp	.LBB3_3

LBB3_6:
	MOVQ DX, R8    // <--                                  // mov	r8, rdx
	ANDQ $-0x2, R8 // <--                                  // and	r8, -2
	XORL AX, AX    // <--                                  // xor	eax, eax

LBB3_7:
	LONG $0x870c8b44         // MOVL 0(DI)(AX*4), R9                 // mov	r9d, dword ptr [rdi + 4*rax]
	WORD $0x8945; BYTE $0xca // MOVL R9, R10                         // mov	r10d, r9d
	SHRL $0x1, R10           // <--                                  // shr	r10d
	LONG $0x01e18341         // ANDL $0x1, R9                        // and	r9d, 1
	NEGL R9                  // <--                                  // neg	r9d
	XORL R10, R9             // <--                                  // xor	r9d, r10d
	WORD $0x0141; BYTE $0xc9 // ADDL CX, R9                          // add	r9d, ecx
	LONG $0x860c8944         // MOVL R9, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r9d
	LONG $0x04874c8b         // MOVL 0x4(DI)(AX*4), CX               // mov	ecx, dword ptr [rdi + 4*rax + 4]
	WORD $0x8941; BYTE $0xca // MOVL CX, R10                         // mov	r10d, ecx
	SHRL $0x1, R10           // <--                                  // shr	r10d
	WORD $0xe183; BYTE $0x01 // ANDL $0x1, CX                        // and	ecx, 1
	NEGL CX                  // <--                                  // neg	ecx
	XORL R10, CX             // <--                                  // xor	ecx, r10d
	WORD $0x0144; BYTE $0xc9 // ADDL R9, CX                          // add	ecx, r9d
	LONG $0x04864c89         // MOVL CX, 0x4(SI)(AX*4)               // mov	dword ptr [rsi + 4*rax + 4], ecx
	ADDQ $0x2, AX            // <--                                  // add	rax, 2
	CMPQ R8, AX              // <--                                  // cmp	r8, rax
	JNE  LBB3_7              // <--                                  // jne	.LBB3_7

LBB3_3:
	WORD $0xc2f6; BYTE $0x01 // TESTL $0x1, DL                       // test	dl, 1
	JE   LBB3_5              // <--                                  // je	.LBB3_5
	WORD $0x148b; BYTE $0x87 // MOVL 0(DI)(AX*4), DX                 // mov	edx, dword ptr [rdi + 4*rax]
	WORD $0xd789             // MOVL DX, DI                          // mov	edi, edx
	SHRL $0x1, DI            // <--                                  // shr	edi
	WORD $0xe283; BYTE $0x01 // ANDL $0x1, DX                        // and	edx, 1
	NEGL DX                  // <--                                  // neg	edx
	XORL DI, DX              // <--                                  // xor	edx, edi
	WORD $0xca01             // ADDL CX, DX                          // add	edx, ecx
	WORD $0x1489; BYTE $0x86 // MOVL DX, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], edx

LBB3_5:
	NOP // (skipped)                            // mov	rsp, rbp
	NOP // (skipped)                            // pop	rbp
	RET // <--                                  // ret
