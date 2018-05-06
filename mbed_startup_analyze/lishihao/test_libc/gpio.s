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
	.file	"gpio.c"
	.global	_ZN5IOPin8pin_usedE
	.bss
	.align	2
	.type	_ZN5IOPin8pin_usedE, %object
	.size	_ZN5IOPin8pin_usedE, 8
_ZN5IOPin8pin_usedE:
	.space	8
	.global	_ZN5IOPin5GPIOxE
	.align	2
	.type	_ZN5IOPin5GPIOxE, %object
	.size	_ZN5IOPin5GPIOxE, 16
_ZN5IOPin5GPIOxE:
	.space	16
	.section	.text._ZN5IOPinC2Eiiii,"ax",%progbits
	.align	1
	.global	_ZN5IOPinC2Eiiii
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPinC2Eiiii, %function
_ZN5IOPinC2Eiiii:
	@ args = 4, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #12]
	str	r1, [r7, #8]
	str	r2, [r7, #4]
	str	r3, [r7]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #8]
	str	r2, [r3]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #4]
	str	r2, [r3, #4]
	ldr	r3, [r7, #12]
	ldr	r2, [r7]
	str	r2, [r3, #8]
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #24]
	str	r2, [r3, #12]
	ldr	r3, [r7, #12]
	mov	r0, r3
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
	.size	_ZN5IOPinC2Eiiii, .-_ZN5IOPinC2Eiiii
	.global	_ZN5IOPinC1Eiiii
	.thumb_set _ZN5IOPinC1Eiiii,_ZN5IOPinC2Eiiii
	.section	.text._ZN5IOPinD2Ev,"ax",%progbits
	.align	1
	.global	_ZN5IOPinD2Ev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPinD2Ev, %function
_ZN5IOPinD2Ev:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L6
	ldrh	r3, [r2, r3, lsl #1]
	sxth	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	movs	r1, #1
	lsl	r3, r1, r3
	sxth	r3, r3
	mvns	r3, r3
	sxth	r3, r3
	ands	r3, r3, r2
	sxth	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	uxth	r1, r2
	ldr	r2, .L6
	strh	r1, [r2, r3, lsl #1]	@ movhi
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L6
	ldrh	r3, [r2, r3, lsl #1]
	cmp	r3, #0
	bne	.L4
	ldr	r3, .L6+4
	ldr	r2, [r3, #24]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	adds	r3, r3, #2
	movs	r1, #1
	lsl	r3, r1, r3
	mvns	r3, r3
	ldr	r1, .L6+4
	ands	r3, r3, r2
	str	r3, [r1, #24]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L6+8
	movs	r1, #0
	str	r1, [r2, r3, lsl #2]
.L4:
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L7:
	.align	2
.L6:
	.word	_ZN5IOPin8pin_usedE
	.word	1073876992
	.word	_ZN5IOPin5GPIOxE
	.size	_ZN5IOPinD2Ev, .-_ZN5IOPinD2Ev
	.global	_ZN5IOPinD1Ev
	.thumb_set _ZN5IOPinD1Ev,_ZN5IOPinD2Ev
	.section	.text._ZN5IOPin4initEv,"ax",%progbits
	.align	1
	.global	_ZN5IOPin4initEv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPin4initEv, %function
_ZN5IOPin4initEv:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #36
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #1
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #28]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #0
	str	r3, [r7, #20]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #0
	blt	.L9
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #3
	bgt	.L9
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #8]
	cmp	r3, #0
	beq	.L10
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #8]
	cmp	r3, #1
	bne	.L9
.L10:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	blt	.L9
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #4
	bgt	.L9
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	blt	.L9
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	cmp	r3, #15
	ble	.L11
.L9:
	movs	r3, #0
	b	.L8
.L11:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L13
	movs	r3, #0
	b	.L8
.L13:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r3, [r2, r3, lsl #2]
	cmp	r3, #0
	bne	.L14
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #0
	bne	.L15
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r1, .L32+8
	str	r1, [r2, r3, lsl #2]
	ldr	r3, .L32+12
	ldr	r3, [r3, #24]
	ldr	r2, .L32+12
	orr	r3, r3, #4
	str	r3, [r2, #24]
	b	.L14
.L15:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L16
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r1, .L32+16
	str	r1, [r2, r3, lsl #2]
	ldr	r3, .L32+12
	ldr	r3, [r3, #24]
	ldr	r2, .L32+12
	orr	r3, r3, #8
	str	r3, [r2, #24]
	b	.L14
.L16:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #2
	bne	.L17
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r1, .L32+20
	str	r1, [r2, r3, lsl #2]
	ldr	r3, .L32+12
	ldr	r3, [r3, #24]
	ldr	r2, .L32+12
	orr	r3, r3, #16
	str	r3, [r2, #24]
	b	.L14
.L17:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	cmp	r3, #3
	bne	.L14
	ldr	r3, .L32+12
	ldr	r3, [r3, #24]
	ldr	r2, .L32+12
	orr	r3, r3, #32
	str	r3, [r2, #24]
.L14:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #8]
	cmp	r3, #0
	bne	.L18
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	bne	.L19
	movs	r3, #72
	str	r3, [r7, #24]
	b	.L20
.L19:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L21
	movs	r3, #40
	str	r3, [r7, #24]
	b	.L20
.L21:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L22
	movs	r3, #0
	str	r3, [r7, #24]
	b	.L20
.L22:
	movs	r3, #0
	b	.L8
.L18:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #3
	bne	.L23
	movs	r3, #20
	str	r3, [r7, #24]
	b	.L24
.L23:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #4
	bne	.L25
	movs	r3, #16
	str	r3, [r7, #24]
	b	.L24
.L25:
	movs	r3, #0
	b	.L8
.L24:
	ldr	r3, [r7, #24]
	adds	r3, r3, #1
	str	r3, [r7, #24]
.L20:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	cmp	r3, #7
	bgt	.L26
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r3, [r3]
	str	r3, [r7, #28]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	str	r3, [r7, #20]
	b	.L27
.L26:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r3, [r3, #4]
	str	r3, [r7, #28]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	subs	r3, r3, #8
	str	r3, [r7, #20]
.L27:
	ldr	r3, [r7, #20]
	lsls	r3, r3, #2
	movs	r2, #15
	lsl	r3, r2, r3
	str	r3, [r7, #12]
	ldr	r3, [r7, #12]
	mvns	r3, r3
	ldr	r2, [r7, #28]
	ands	r3, r3, r2
	str	r3, [r7, #28]
	ldr	r3, [r7, #20]
	lsls	r3, r3, #2
	ldr	r2, [r7, #24]
	lsl	r3, r2, r3
	ldr	r2, [r7, #28]
	orrs	r3, r3, r2
	str	r3, [r7, #28]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #2
	bne	.L28
	ldr	r3, [r7, #4]
	ldr	r2, [r3, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r1, .L32+4
	ldr	r3, [r1, r3, lsl #2]
	movs	r1, #1
	lsl	r2, r1, r2
	str	r2, [r3, #20]
	b	.L29
.L28:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #12]
	cmp	r3, #1
	bne	.L29
	ldr	r3, [r7, #4]
	ldr	r2, [r3, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r1, .L32+4
	ldr	r3, [r1, r3, lsl #2]
	movs	r1, #1
	lsl	r2, r1, r2
	str	r2, [r3, #16]
.L29:
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	cmp	r3, #7
	bgt	.L30
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r2, [r7, #28]
	str	r2, [r3]
	b	.L31
.L30:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r2, [r7, #28]
	str	r2, [r3, #4]
.L31:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L32
	ldrh	r3, [r2, r3, lsl #1]
	sxth	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	movs	r1, #1
	lsl	r3, r1, r3
	sxth	r3, r3
	orrs	r3, r3, r2
	sxth	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	uxth	r1, r2
	ldr	r2, .L32
	strh	r1, [r2, r3, lsl #1]	@ movhi
.L8:
	mov	r0, r3
	adds	r7, r7, #36
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L33:
	.align	2
.L32:
	.word	_ZN5IOPin8pin_usedE
	.word	_ZN5IOPin5GPIOxE
	.word	1073809408
	.word	1073876992
	.word	1073810432
	.word	1073811456
	.size	_ZN5IOPin4initEv, .-_ZN5IOPin4initEv
	.section	.text._ZN5IOPin4readEv,"ax",%progbits
	.align	1
	.global	_ZN5IOPin4readEv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPin4readEv, %function
_ZN5IOPin4readEv:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L37
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L35
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L37+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r2, [r3, #8]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	lsr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L35
	movs	r3, #1
	b	.L36
.L35:
	movs	r3, #0
.L36:
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L38:
	.align	2
.L37:
	.word	_ZN5IOPin8pin_usedE
	.word	_ZN5IOPin5GPIOxE
	.size	_ZN5IOPin4readEv, .-_ZN5IOPin4readEv
	.section	.text._ZN5IOPin5writeEi,"ax",%progbits
	.align	1
	.global	_ZN5IOPin5writeEi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPin5writeEi, %function
_ZN5IOPin5writeEi:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	str	r1, [r7]
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L40
	ldr	r3, [r7]
	cmp	r3, #1
	beq	.L40
	movs	r3, #0
	b	.L41
.L40:
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L45
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L42
	ldr	r3, [r7]
	cmp	r3, #1
	bne	.L43
	ldr	r3, [r7, #4]
	ldr	r2, [r3, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r1, .L45+4
	ldr	r3, [r1, r3, lsl #2]
	movs	r1, #1
	lsl	r2, r1, r2
	str	r2, [r3, #16]
	b	.L44
.L43:
	ldr	r3, [r7, #4]
	ldr	r2, [r3, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r1, .L45+4
	ldr	r3, [r1, r3, lsl #2]
	movs	r1, #1
	lsl	r2, r1, r2
	str	r2, [r3, #20]
.L44:
	movs	r3, #1
	b	.L41
.L42:
	movs	r3, #0
.L41:
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L46:
	.align	2
.L45:
	.word	_ZN5IOPin8pin_usedE
	.word	_ZN5IOPin5GPIOxE
	.size	_ZN5IOPin5writeEi, .-_ZN5IOPin5writeEi
	.section	.text._ZN5IOPin11exti_configEv,"ax",%progbits
	.align	1
	.global	_ZN5IOPin11exti_configEv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPin11exti_configEv, %function
_ZN5IOPin11exti_configEv:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	ldr	r3, .L49
	ldr	r3, [r3, #24]
	ldr	r2, .L49
	orr	r3, r3, #1
	str	r3, [r2, #24]
	movs	r3, #0
	str	r3, [r7, #12]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	and	r3, r3, #3
	lsls	r3, r3, #2
	movs	r2, #15
	lsl	r3, r2, r3
	str	r3, [r7, #12]
	ldr	r2, .L49+4
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asrs	r3, r3, #2
	adds	r3, r3, #2
	ldr	r1, [r2, r3, lsl #2]
	ldr	r3, [r7, #12]
	mvns	r2, r3
	ldr	r0, .L49+4
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asrs	r3, r3, #2
	ands	r2, r2, r1
	adds	r3, r3, #2
	str	r2, [r0, r3, lsl #2]
	ldr	r2, .L49+4
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asrs	r3, r3, #2
	adds	r3, r3, #2
	ldr	r1, [r2, r3, lsl #2]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	mov	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	and	r3, r3, #3
	lsls	r3, r3, #2
	lsls	r2, r2, r3
	ldr	r0, .L49+4
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asrs	r3, r3, #2
	orrs	r2, r2, r1
	adds	r3, r3, #2
	str	r2, [r0, r3, lsl #2]
	movs	r3, #1
	mov	r0, r3
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L50:
	.align	2
.L49:
	.word	1073876992
	.word	1073807360
	.size	_ZN5IOPin11exti_configEv, .-_ZN5IOPin11exti_configEv
	.section	.text._ZN5IOPin11read_outputEv,"ax",%progbits
	.align	1
	.global	_ZN5IOPin11read_outputEv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_ZN5IOPin11read_outputEv, %function
_ZN5IOPin11read_outputEv:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	str	r0, [r7, #4]
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L54
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	asr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L52
	ldr	r3, [r7, #4]
	ldr	r3, [r3]
	ldr	r2, .L54+4
	ldr	r3, [r2, r3, lsl #2]
	ldr	r2, [r3, #12]
	ldr	r3, [r7, #4]
	ldr	r3, [r3, #4]
	lsr	r3, r2, r3
	and	r3, r3, #1
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L52
	movs	r3, #1
	b	.L53
.L52:
	movs	r3, #0
.L53:
	mov	r0, r3
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L55:
	.align	2
.L54:
	.word	_ZN5IOPin8pin_usedE
	.word	_ZN5IOPin5GPIOxE
	.size	_ZN5IOPin11read_outputEv, .-_ZN5IOPin11read_outputEv
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 7-2017-q4-major) 7.2.1 20170904 (release) [ARM/embedded-7-branch revision 255204]"
