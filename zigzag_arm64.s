//go:build !noasm && arm64
// Code generated by gocc rev-4e20446 -- DO NOT EDIT.
//
// Source file         : zigzag.c
// Clang version       : Apple clang version 16.0.0 (clang-1600.0.26.4)
// Target architecture : arm64
// Compiler options    : [none]

#include "textflag.h"

TEXT ·zigzag_encode(SB), NOSPLIT, $0-24
	MOVD in+0(FP), R0
	MOVD out+8(FP), R1
	MOVD N+16(FP), R2
	CBZ  R2, LBB0_9    // <--                                  // cbz	x2, .LBB0_9
	NOP                // (skipped)                            // stp	x29, x30, [sp, #-16]!
	MOVD ZR, R8        // <--                                  // mov	x8, xzr
	CMP  $8, R2        // <--                                  // cmp	x2, #8
	NOP                // (skipped)                            // mov	x29, sp
	BCC  LBB0_6        // <--                                  // b.lo	.LBB0_6
	SUB  R0, R1, R9    // <--                                  // sub	x9, x1, x0
	CMP  $32, R9       // <--                                  // cmp	x9, #32
	BCC  LBB0_6        // <--                                  // b.lo	.LBB0_6
	AND  $-8, R2, R8   // <--                                  // and	x8, x2, #0xfffffffffffffff8
	ADD  $16, R0, R9   // <--                                  // add	x9, x0, #16
	ADD  $16, R1, R10  // <--                                  // add	x10, x1, #16
	MOVD R8, R11       // <--                                  // mov	x11, x8

LBB0_4:
	WORD $0xad7f8520            // FLDPQ -16(R9), (F0, F1)              // ldp	q0, q1, [x9, #-16]
	ADD  $32, R9, R9            // <--                                  // add	x9, x9, #32
	SUBS $8, R11, R11           // <--                                  // subs	x11, x11, #8
	VADD V0.S4, V0.S4, V2.S4    // <--                                  // add	v2.4s, v0.4s, v0.4s
	WORD $0x4ea0a800            // VCMLT $0, V0.S4, V0.S4               // cmlt	v0.4s, v0.4s, #0
	VADD V1.S4, V1.S4, V3.S4    // <--                                  // add	v3.4s, v1.4s, v1.4s
	WORD $0x4ea0a821            // VCMLT $0, V1.S4, V1.S4               // cmlt	v1.4s, v1.4s, #0
	VEOR V0.B16, V2.B16, V0.B16 // <--                                  // eor	v0.16b, v2.16b, v0.16b
	VEOR V1.B16, V3.B16, V1.B16 // <--                                  // eor	v1.16b, v3.16b, v1.16b
	WORD $0xad3f8540            // FSTPQ (F0, F1), -16(R10)             // stp	q0, q1, [x10, #-16]
	ADD  $32, R10, R10          // <--                                  // add	x10, x10, #32
	BNE  LBB0_4                 // <--                                  // b.ne	.LBB0_4
	CMP  R2, R8                 // <--                                  // cmp	x8, x2
	BEQ  LBB0_8                 // <--                                  // b.eq	.LBB0_8

LBB0_6:
	LSL $2, R8, R10  // <--                                  // lsl	x10, x8, #2
	SUB R8, R2, R9   // <--                                  // sub	x9, x2, x8
	ADD R10, R1, R8  // <--                                  // add	x8, x1, x10
	ADD R10, R0, R10 // <--                                  // add	x10, x0, x10

LBB0_7:
	WORD $0xb840454b       // MOVWU.P 4(R10), R11                  // ldr	w11, [x10], #4
	SUBS $1, R9, R9        // <--                                  // subs	x9, x9, #1
	LSLW $1, R11, R12      // <--                                  // lsl	w12, w11, #1
	EORW R11->31, R12, R11 // <--                                  // eor	w11, w12, w11, asr #31
	WORD $0xb800450b       // MOVW.P R11, 4(R8)                    // str	w11, [x8], #4
	BNE  LBB0_7            // <--                                  // b.ne	.LBB0_7

LBB0_8:
	NOP // (skipped)                            // ldp	x29, x30, [sp], #16

LBB0_9:
	RET // <--                                  // ret

TEXT ·zigzag_delta_encode(SB), NOSPLIT, $0-28
	MOVD in+0(FP), R0
	MOVD out+8(FP), R1
	MOVD N+16(FP), R2
	MOVW prev+24(FP), R3
	CBZ  R2, LBB1_4      // <--                                  // cbz	x2, .LBB1_4
	NOP                  // (skipped)                            // stp	x29, x30, [sp, #-16]!
	NOP                  // (skipped)                            // mov	x29, sp

LBB1_2:
	WORD $0xb9400008    // MOVWU (R0), R8                       // ldr	w8, [x0]
	SUBS $1, R2, R2     // <--                                  // subs	x2, x2, #1
	SUBW R3, R8, R8     // <--                                  // sub	w8, w8, w3
	LSLW $1, R8, R9     // <--                                  // lsl	w9, w8, #1
	EORW R8->31, R9, R8 // <--                                  // eor	w8, w9, w8, asr #31
	WORD $0xb8004428    // MOVW.P R8, 4(R1)                     // str	w8, [x1], #4
	WORD $0xb8404403    // MOVWU.P 4(R0), R3                    // ldr	w3, [x0], #4
	BNE  LBB1_2         // <--                                  // b.ne	.LBB1_2
	NOP                 // (skipped)                            // ldp	x29, x30, [sp], #16

LBB1_4:
	RET // <--                                  // ret

TEXT ·zigzag_decode(SB), NOSPLIT, $0-24
	MOVD in+0(FP), R0
	MOVD out+8(FP), R1
	MOVD N+16(FP), R2
	CBZ  R2, LBB2_9    // <--                                  // cbz	x2, .LBB2_9
	NOP                // (skipped)                            // stp	x29, x30, [sp, #-16]!
	MOVD ZR, R8        // <--                                  // mov	x8, xzr
	CMP  $8, R2        // <--                                  // cmp	x2, #8
	NOP                // (skipped)                            // mov	x29, sp
	BCC  LBB2_6        // <--                                  // b.lo	.LBB2_6
	SUB  R0, R1, R9    // <--                                  // sub	x9, x1, x0
	CMP  $32, R9       // <--                                  // cmp	x9, #32
	BCC  LBB2_6        // <--                                  // b.lo	.LBB2_6
	AND  $-8, R2, R8   // <--                                  // and	x8, x2, #0xfffffffffffffff8
	ADD  $16, R0, R9   // <--                                  // add	x9, x0, #16
	WORD $0x4f000420   // VMOVI $1, V0.S4                      // movi	v0.4s, #1
	ADD  $16, R1, R10  // <--                                  // add	x10, x1, #16
	MOVD R8, R11       // <--                                  // mov	x11, x8

LBB2_4:
	WORD $0xad7f8921            // FLDPQ -16(R9), (F1, F2)              // ldp	q1, q2, [x9, #-16]
	ADD  $32, R9, R9            // <--                                  // add	x9, x9, #32
	SUBS $8, R11, R11           // <--                                  // subs	x11, x11, #8
	WORD $0x6f3f0423            // VUSHR $1, V1.S4, V3.S4               // ushr	v3.4s, v1.4s, #1
	VAND V0.B16, V1.B16, V1.B16 // <--                                  // and	v1.16b, v1.16b, v0.16b
	WORD $0x6f3f0444            // VUSHR $1, V2.S4, V4.S4               // ushr	v4.4s, v2.4s, #1
	VAND V0.B16, V2.B16, V2.B16 // <--                                  // and	v2.16b, v2.16b, v0.16b
	WORD $0x6ea0b821            // VNEG V1.S4, V1.S4                    // neg	v1.4s, v1.4s
	WORD $0x6ea0b842            // VNEG V2.S4, V2.S4                    // neg	v2.4s, v2.4s
	VEOR V1.B16, V3.B16, V1.B16 // <--                                  // eor	v1.16b, v3.16b, v1.16b
	VEOR V2.B16, V4.B16, V2.B16 // <--                                  // eor	v2.16b, v4.16b, v2.16b
	WORD $0xad3f8941            // FSTPQ (F1, F2), -16(R10)             // stp	q1, q2, [x10, #-16]
	ADD  $32, R10, R10          // <--                                  // add	x10, x10, #32
	BNE  LBB2_4                 // <--                                  // b.ne	.LBB2_4
	CMP  R2, R8                 // <--                                  // cmp	x8, x2
	BEQ  LBB2_8                 // <--                                  // b.eq	.LBB2_8

LBB2_6:
	LSL $2, R8, R10  // <--                                  // lsl	x10, x8, #2
	SUB R8, R2, R9   // <--                                  // sub	x9, x2, x8
	ADD R10, R1, R8  // <--                                  // add	x8, x1, x10
	ADD R10, R0, R10 // <--                                  // add	x10, x0, x10

LBB2_7:
	WORD $0xb840454b      // MOVWU.P 4(R10), R11                  // ldr	w11, [x10], #4
	SUBS $1, R9, R9       // <--                                  // subs	x9, x9, #1
	ANDW $1, R11, R12     // <--                                  // and	w12, w11, #0x1
	NEGW R12, R12         // <--                                  // neg	w12, w12
	EORW R11>>1, R12, R11 // <--                                  // eor	w11, w12, w11, lsr #1
	WORD $0xb800450b      // MOVW.P R11, 4(R8)                    // str	w11, [x8], #4
	BNE  LBB2_7           // <--                                  // b.ne	.LBB2_7

LBB2_8:
	NOP // (skipped)                            // ldp	x29, x30, [sp], #16

LBB2_9:
	RET // <--                                  // ret

TEXT ·zigzag_delta_decode(SB), NOSPLIT, $0-28
	MOVD in+0(FP), R0
	MOVD out+8(FP), R1
	MOVD N+16(FP), R2
	MOVW prev+24(FP), R3
	CBZ  R2, LBB3_4      // <--                                  // cbz	x2, .LBB3_4
	NOP                  // (skipped)                            // stp	x29, x30, [sp, #-16]!
	NOP                  // (skipped)                            // mov	x29, sp

LBB3_2:
	WORD $0xb8404408   // MOVWU.P 4(R0), R8                    // ldr	w8, [x0], #4
	SUBS $1, R2, R2    // <--                                  // subs	x2, x2, #1
	ANDW $1, R8, R9    // <--                                  // and	w9, w8, #0x1
	NEGW R9, R9        // <--                                  // neg	w9, w9
	EORW R8>>1, R9, R8 // <--                                  // eor	w8, w9, w8, lsr #1
	ADDW R3, R8, R3    // <--                                  // add	w3, w8, w3
	WORD $0xb8004423   // MOVW.P R3, 4(R1)                     // str	w3, [x1], #4
	BNE  LBB3_2        // <--                                  // b.ne	.LBB3_2
	NOP                // (skipped)                            // ldp	x29, x30, [sp], #16

LBB3_4:
	RET // <--                                  // ret
