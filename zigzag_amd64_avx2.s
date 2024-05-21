//go:build !noasm && amd64
// Code generated by gocc v0.9.2 -- DO NOT EDIT.
//
// Source file         : zigzag.c
// Clang version       : Apple clang version 15.0.0 (clang-1500.3.9.4)
// Target architecture : amd64
// Compiler options    : -mavx2 -mfma

#include "textflag.h"

TEXT ·zigzag_encode_avx2(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	NOP                      // (skipped)                            // push	rbp
	NOP                      // (skipped)                            // mov	rbp, rsp
	NOP                      // (skipped)                            // and	rsp, -8
	WORD $0x8548; BYTE $0xd2 // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB0_10             // <--                                  // je	.LBB0_10
	XORL AX, AX              // <--                                  // xor	eax, eax
	CMPQ DX, $0x20           // <--                                  // cmp	rdx, 32
	JB   LBB0_6              // <--                                  // jb	.LBB0_6
	MOVQ SI, CX              // <--                                  // mov	rcx, rsi
	SUBQ DI, CX              // <--                                  // sub	rcx, rdi
	CMPQ CX, $0x80           // <--                                  // cmp	rcx, 128
	JB   LBB0_6              // <--                                  // jb	.LBB0_6
	MOVQ DX, AX              // <--                                  // mov	rax, rdx
	ANDQ $-0x20, AX          // <--                                  // and	rax, -32
	XORL CX, CX              // <--                                  // xor	ecx, ecx

LBB0_4:
	LONG $0x046ffec5; BYTE $0x8f   // VMOVDQU 0(DI)(CX*4), X0              // vmovdqu	ymm0, ymmword ptr [rdi + 4*rcx]
	LONG $0x4c6ffec5; WORD $0x208f // VMOVDQU 0x20(DI)(CX*4), X1           // vmovdqu	ymm1, ymmword ptr [rdi + 4*rcx + 32]
	LONG $0x546ffec5; WORD $0x408f // VMOVDQU 0x40(DI)(CX*4), X2           // vmovdqu	ymm2, ymmword ptr [rdi + 4*rcx + 64]
	LONG $0x5c6ffec5; WORD $0x608f // VMOVDQU 0x60(DI)(CX*4), X3           // vmovdqu	ymm3, ymmword ptr [rdi + 4*rcx + 96]
	LONG $0xe0fefdc5               // ?                                    // vpaddd	ymm4, ymm0, ymm0
	LONG $0xe9fef5c5               // ?                                    // vpaddd	ymm5, ymm1, ymm1
	LONG $0xf2feedc5               // ?                                    // vpaddd	ymm6, ymm2, ymm2
	LONG $0xfbfee5c5               // ?                                    // vpaddd	ymm7, ymm3, ymm3
	LONG $0xe072fdc5; BYTE $0x1f   // ?                                    // vpsrad	ymm0, ymm0, 31
	LONG $0xc0efddc5               // ?                                    // vpxor	ymm0, ymm4, ymm0
	LONG $0xe172f5c5; BYTE $0x1f   // ?                                    // vpsrad	ymm1, ymm1, 31
	LONG $0xc9efd5c5               // ?                                    // vpxor	ymm1, ymm5, ymm1
	LONG $0xe272edc5; BYTE $0x1f   // ?                                    // vpsrad	ymm2, ymm2, 31
	LONG $0xd2efcdc5               // ?                                    // vpxor	ymm2, ymm6, ymm2
	LONG $0xe372e5c5; BYTE $0x1f   // ?                                    // vpsrad	ymm3, ymm3, 31
	LONG $0xdbefc5c5               // ?                                    // vpxor	ymm3, ymm7, ymm3
	LONG $0x047ffec5; BYTE $0x8e   // VMOVDQU X0, 0(SI)(CX*4)              // vmovdqu	ymmword ptr [rsi + 4*rcx], ymm0
	LONG $0x4c7ffec5; WORD $0x208e // VMOVDQU X1, 0x20(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 32], ymm1
	LONG $0x547ffec5; WORD $0x408e // VMOVDQU X2, 0x40(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 64], ymm2
	LONG $0x5c7ffec5; WORD $0x608e // VMOVDQU X3, 0x60(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 96], ymm3
	ADDQ $0x20, CX                 // <--                                  // add	rcx, 32
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
	LEAL 0(R8)(R8*1), R9     // <--                                  // lea	r9d, [r8 + r8]
	SARL $0x1f, R8           // <--                                  // sar	r8d, 31
	XORL R9, R8              // <--                                  // xor	r8d, r9d
	LONG $0x86048944         // MOVL R8, 0(SI)(AX*4)                 // mov	dword ptr [rsi + 4*rax], r8d
	ORQ  $0x1, AX            // <--                                  // or	rax, 1

LBB0_8:
	ADDQ DX, CX  // <--                                  // add	rcx, rdx
	JE   LBB0_10 // <--                                  // je	.LBB0_10

LBB0_9:
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
	JNE  LBB0_9              // <--                                  // jne	.LBB0_9

LBB0_10:
	NOP        // (skipped)                            // mov	rsp, rbp
	NOP        // (skipped)                            // pop	rbp
	VZEROUPPER // <--                                  // vzeroupper
	RET        // <--                                  // ret

TEXT ·zigzag_delta_encode_avx2(SB), NOSPLIT, $0-28
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
GLOBL LCPI2_0<>(SB), (RODATA|NOPTR), $4

TEXT ·zigzag_decode_avx2(SB), NOSPLIT, $0-24
	MOVQ in+0(FP), DI
	MOVQ out+8(FP), SI
	MOVQ N+16(FP), DX
	NOP                                  // (skipped)                            // push	rbp
	NOP                                  // (skipped)                            // mov	rbp, rsp
	NOP                                  // (skipped)                            // and	rsp, -8
	WORD $0x8548; BYTE $0xd2             // TESTQ DX, DX                         // test	rdx, rdx
	JE   LBB2_10                         // <--                                  // je	.LBB2_10
	XORL AX, AX                          // <--                                  // xor	eax, eax
	CMPQ DX, $0x20                       // <--                                  // cmp	rdx, 32
	JB   LBB2_6                          // <--                                  // jb	.LBB2_6
	MOVQ SI, CX                          // <--                                  // mov	rcx, rsi
	SUBQ DI, CX                          // <--                                  // sub	rcx, rdi
	CMPQ CX, $0x80                       // <--                                  // cmp	rcx, 128
	JB   LBB2_6                          // <--                                  // jb	.LBB2_6
	MOVQ DX, AX                          // <--                                  // mov	rax, rdx
	ANDQ $-0x20, AX                      // <--                                  // and	rax, -32
	XORL CX, CX                          // <--                                  // xor	ecx, ecx
	QUAD $0x00000005587de2c4; BYTE $0x00 // ?                                    // vpbroadcastd	ymm0, dword ptr [rip + .LCPI2_0]
	LONG $0xc9eff1c5                     // ?                                    // vpxor	xmm1, xmm1, xmm1

LBB2_4:
	LONG $0x146ffec5; BYTE $0x8f   // VMOVDQU 0(DI)(CX*4), X2              // vmovdqu	ymm2, ymmword ptr [rdi + 4*rcx]
	LONG $0x5c6ffec5; WORD $0x208f // VMOVDQU 0x20(DI)(CX*4), X3           // vmovdqu	ymm3, ymmword ptr [rdi + 4*rcx + 32]
	LONG $0x646ffec5; WORD $0x408f // VMOVDQU 0x40(DI)(CX*4), X4           // vmovdqu	ymm4, ymmword ptr [rdi + 4*rcx + 64]
	LONG $0x6c6ffec5; WORD $0x608f // VMOVDQU 0x60(DI)(CX*4), X5           // vmovdqu	ymm5, ymmword ptr [rdi + 4*rcx + 96]
	LONG $0xd272cdc5; BYTE $0x01   // ?                                    // vpsrld	ymm6, ymm2, 1
	LONG $0xd372c5c5; BYTE $0x01   // ?                                    // vpsrld	ymm7, ymm3, 1
	LONG $0xd472bdc5; BYTE $0x01   // ?                                    // vpsrld	ymm8, ymm4, 1
	LONG $0xd572b5c5; BYTE $0x01   // ?                                    // vpsrld	ymm9, ymm5, 1
	LONG $0xd0dbedc5               // FCMOVNBE F0, F0                      // vpand	ymm2, ymm2, ymm0
	LONG $0xd8dbe5c5               // FCMOVNU F0, F0                       // vpand	ymm3, ymm3, ymm0
	LONG $0xe0dbddc5               // ?                                    // vpand	ymm4, ymm4, ymm0
	LONG $0xe8dbd5c5               // FUCOMI F0, F0                        // vpand	ymm5, ymm5, ymm0
	LONG $0xd2faf5c5               // ?                                    // vpsubd	ymm2, ymm1, ymm2
	LONG $0xd2efcdc5               // ?                                    // vpxor	ymm2, ymm6, ymm2
	LONG $0xdbfaf5c5               // ?                                    // vpsubd	ymm3, ymm1, ymm3
	LONG $0xdbefc5c5               // ?                                    // vpxor	ymm3, ymm7, ymm3
	LONG $0xe4faf5c5               // ?                                    // vpsubd	ymm4, ymm1, ymm4
	LONG $0xe4efbdc5               // ?                                    // vpxor	ymm4, ymm8, ymm4
	LONG $0xedfaf5c5               // ?                                    // vpsubd	ymm5, ymm1, ymm5
	LONG $0xedefb5c5               // ?                                    // vpxor	ymm5, ymm9, ymm5
	LONG $0x147ffec5; BYTE $0x8e   // VMOVDQU X2, 0(SI)(CX*4)              // vmovdqu	ymmword ptr [rsi + 4*rcx], ymm2
	LONG $0x5c7ffec5; WORD $0x208e // VMOVDQU X3, 0x20(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 32], ymm3
	LONG $0x647ffec5; WORD $0x408e // VMOVDQU X4, 0x40(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 64], ymm4
	LONG $0x6c7ffec5; WORD $0x608e // VMOVDQU X5, 0x60(SI)(CX*4)           // vmovdqu	ymmword ptr [rsi + 4*rcx + 96], ymm5
	ADDQ $0x20, CX                 // <--                                  // add	rcx, 32
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
	NOP        // (skipped)                            // mov	rsp, rbp
	NOP        // (skipped)                            // pop	rbp
	VZEROUPPER // <--                                  // vzeroupper
	RET        // <--                                  // ret

TEXT ·zigzag_delta_decode_avx2(SB), NOSPLIT, $0-28
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
