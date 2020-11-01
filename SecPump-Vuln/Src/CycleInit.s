	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"tst.c"
	.text
	.comm	m_nStart,4,4
	.comm	m_nStop,4,4
	.align	1
	.global	InitCycleCnt
	.arch armv7e-m
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	InitCycleCnt, %function
InitCycleCnt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	add	r7, sp, #0
	ldr	r3, .L2
	ldr	r3, [r3]
	ldr	r2, .L2
	orr	r3, r3, #16777216
	str	r3, [r2]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #131072
	str	r3, [r2]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #262144
	str	r3, [r2]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #524288
	str	r3, [r2]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #1048576
	str	r3, [r2]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #2097152
	str	r3, [r2]
	ldr	r3, .L2+8
	ldr	r3, [r3]
	ldr	r2, .L2+8
	bic	r3, r3, #255
	str	r3, [r2]
	ldr	r3, .L2+12
	ldr	r3, [r3]
	ldr	r2, .L2+12
	bic	r3, r3, #255
	str	r3, [r2]
	ldr	r3, .L2+16
	ldr	r3, [r3]
	ldr	r2, .L2+16
	bic	r3, r3, #255
	str	r3, [r2]
	ldr	r3, .L2+20
	ldr	r3, [r3]
	ldr	r2, .L2+20
	bic	r3, r3, #255
	str	r3, [r2]
	ldr	r3, .L2+24
	ldr	r3, [r3]
	ldr	r2, .L2+24
	bic	r3, r3, #255
	str	r3, [r2]
	ldr	r3, .L2+28
	movs	r2, #0
	str	r2, [r3]
	ldr	r3, .L2+4
	ldr	r3, [r3]
	ldr	r2, .L2+4
	orr	r3, r3, #1
	str	r3, [r2]
	nop
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
.L3:
	.align	2
.L2:
	.word	-536809988
	.word	-536866816
	.word	-536866808
	.word	-536866804
	.word	-536866800
	.word	-536866796
	.word	-536866792
	.word	-536866812
	.size	InitCycleCnt, .-InitCycleCnt
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 8-2018-q4-major) 8.2.1 20181213 (release) [gcc-8-branch revision 267074]"
