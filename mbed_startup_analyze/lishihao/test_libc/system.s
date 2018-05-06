	.cpu cortex-m3
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"system.c"
	.global	SystemCoreClock
	.data
	.align	2
	.type	SystemCoreClock, %object
	.size	SystemCoreClock, 4
SystemCoreClock:
	.word	72000000
	.global	AHBPrescTable
	.align	2
	.type	AHBPrescTable, %object
	.size	AHBPrescTable, 16
AHBPrescTable:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.section	.text.SystemInit,"ax",%progbits
	.align	1
	.global	SystemInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	SystemInit, %function
SystemInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	ldr	r3, .L2
	ldr	r3, [r3]
	ldr	r2, .L2
	orr	r3, r3, #1
	str	r3, [r2]
	ldr	r3, .L2
	ldr	r2, [r3, #4]
	ldr	r1, .L2
	ldr	r3, .L2+4
	ands	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, .L2
	ldr	r3, [r3]
	ldr	r2, .L2
	bic	r3, r3, #17301504
	bic	r3, r3, #65536
	str	r3, [r2]
	ldr	r3, .L2
	ldr	r3, [r3]
	ldr	r2, .L2
	bic	r3, r3, #262144
	str	r3, [r2]
	ldr	r3, .L2
	ldr	r3, [r3, #4]
	ldr	r2, .L2
	bic	r3, r3, #8323072
	str	r3, [r2, #4]
	ldr	r3, .L2
	mov	r2, #10420224
	str	r2, [r3, #8]
	bl	_ZL11SetSysClockv
	ldr	r3, .L2+8
	mov	r2, #134217728
	str	r2, [r3, #8]
	nop
	pop	{r7, pc}
.L3:
	.align	2
.L2:
	.word	1073876992
	.word	-117506048
	.word	-536810240
	.size	SystemInit, .-SystemInit
	.section	.text.SystemCoreClockUpdate,"ax",%progbits
	.align	1
	.global	SystemCoreClockUpdate
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	SystemCoreClockUpdate, %function
SystemCoreClockUpdate:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #0
	str	r3, [r7, #8]
	movs	r3, #0
	str	r3, [r7, #4]
	ldr	r3, .L14
	ldr	r3, [r3, #4]
	and	r3, r3, #12
	str	r3, [r7, #12]
	ldr	r3, [r7, #12]
	cmp	r3, #4
	beq	.L6
	cmp	r3, #8
	beq	.L7
	cmp	r3, #0
	bne	.L13
	ldr	r3, .L14+4
	ldr	r2, .L14+8
	str	r2, [r3]
	b	.L9
.L6:
	ldr	r3, .L14+4
	ldr	r2, .L14+8
	str	r2, [r3]
	b	.L9
.L7:
	ldr	r3, .L14
	ldr	r3, [r3, #4]
	and	r3, r3, #3932160
	str	r3, [r7, #8]
	ldr	r3, .L14
	ldr	r3, [r3, #4]
	and	r3, r3, #65536
	str	r3, [r7, #4]
	ldr	r3, [r7, #8]
	lsrs	r3, r3, #18
	adds	r3, r3, #2
	str	r3, [r7, #8]
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne	.L10
	ldr	r3, [r7, #8]
	ldr	r2, .L14+12
	mul	r3, r2, r3
	ldr	r2, .L14+4
	str	r3, [r2]
	b	.L9
.L10:
	ldr	r3, .L14
	ldr	r3, [r3, #4]
	and	r3, r3, #131072
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L12
	ldr	r3, [r7, #8]
	ldr	r2, .L14+12
	mul	r3, r2, r3
	ldr	r2, .L14+4
	str	r3, [r2]
	b	.L9
.L12:
	ldr	r3, [r7, #8]
	ldr	r2, .L14+8
	mul	r3, r2, r3
	ldr	r2, .L14+4
	str	r3, [r2]
	b	.L9
.L13:
	ldr	r3, .L14+4
	ldr	r2, .L14+8
	str	r2, [r3]
	nop
.L9:
	ldr	r3, .L14
	ldr	r3, [r3, #4]
	lsrs	r3, r3, #4
	and	r3, r3, #15
	ldr	r2, .L14+16
	ldrb	r3, [r2, r3]
	uxtb	r3, r3
	str	r3, [r7, #12]
	ldr	r3, .L14+4
	ldr	r2, [r3]
	ldr	r3, [r7, #12]
	lsr	r3, r2, r3
	ldr	r2, .L14+4
	str	r3, [r2]
	nop
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L15:
	.align	2
.L14:
	.word	1073876992
	.word	SystemCoreClock
	.word	8000000
	.word	4000000
	.word	AHBPrescTable
	.size	SystemCoreClockUpdate, .-SystemCoreClockUpdate
	.section	.text._ZL11SetSysClockv,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZL11SetSysClockv, %function
_ZL11SetSysClockv:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	bl	_ZL15SetSysClockTo72v
	nop
	pop	{r7, pc}
	.size	_ZL11SetSysClockv, .-_ZL11SetSysClockv
	.section	.text._ZL15SetSysClockTo72v,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZL15SetSysClockTo72v, %function
_ZL15SetSysClockTo72v:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7]
	ldr	r3, .L29
	ldr	r3, [r3]
	ldr	r2, .L29
	orr	r3, r3, #65536
	str	r3, [r2]
.L21:
	ldr	r3, .L29
	ldr	r3, [r3]
	and	r3, r3, #131072
	str	r3, [r7]
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	str	r3, [r7, #4]
	ldr	r3, [r7]
	cmp	r3, #0
	bne	.L18
	ldr	r3, [r7, #4]
	cmp	r3, #1280
	beq	.L18
	movs	r3, #1
	b	.L19
.L18:
	movs	r3, #0
.L19:
	cmp	r3, #0
	beq	.L20
	b	.L21
.L20:
	ldr	r3, .L29
	ldr	r3, [r3]
	and	r3, r3, #131072
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L22
	movs	r3, #1
	str	r3, [r7]
	b	.L23
.L22:
	movs	r3, #0
	str	r3, [r7]
.L23:
	ldr	r3, [r7]
	cmp	r3, #1
	ite	eq
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L28
	ldr	r3, .L29+4
	ldr	r3, [r3]
	ldr	r2, .L29+4
	orr	r3, r3, #16
	str	r3, [r2]
	ldr	r3, .L29+4
	ldr	r3, [r3]
	ldr	r2, .L29+4
	bic	r3, r3, #3
	str	r3, [r2]
	ldr	r3, .L29+4
	ldr	r3, [r3]
	ldr	r2, .L29+4
	orr	r3, r3, #2
	str	r3, [r2]
	ldr	r3, .L29
	ldr	r2, .L29
	ldr	r3, [r3, #4]
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r2, .L29
	ldr	r3, [r3, #4]
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	ldr	r2, .L29
	orr	r3, r3, #1024
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	ldr	r2, .L29
	bic	r3, r3, #4128768
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	ldr	r2, .L29
	orr	r3, r3, #1900544
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r3, [r3]
	ldr	r2, .L29
	orr	r3, r3, #16777216
	str	r3, [r2]
.L26:
	ldr	r3, .L29
	ldr	r3, [r3]
	and	r3, r3, #33554432
	cmp	r3, #0
	ite	eq
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L25
	b	.L26
.L25:
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	ldr	r2, .L29
	bic	r3, r3, #3
	str	r3, [r2, #4]
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	ldr	r2, .L29
	orr	r3, r3, #2
	str	r3, [r2, #4]
.L27:
	ldr	r3, .L29
	ldr	r3, [r3, #4]
	and	r3, r3, #12
	cmp	r3, #8
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L28
	b	.L27
.L28:
	nop
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L30:
	.align	2
.L29:
	.word	1073876992
	.word	1073881088
	.size	_ZL15SetSysClockTo72v, .-_ZL15SetSysClockTo72v
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 7-2017-q4-major) 7.2.1 20170904 (release) [ARM/embedded-7-branch revision 255204]"
