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
	.file	"main.c"
	.global	a2
	.bss
	.align	2
	.type	a2, %object
	.size	a2, 16
a2:
	.space	16
	.section	.text.main,"ax",%progbits
	.align	1
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #8
	add	r7, sp, #0
	ldr	r0, .L7
	bl	_ZN5IOPin4initEv
.L6:
	ldr	r3, .L7+4
	str	r3, [r7, #4]
.L3:
	ldr	r3, [r7, #4]
	subs	r3, r3, #1
	str	r3, [r7, #4]
	ldr	r3, [r7, #4]
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L2
	b	.L3
.L2:
	ldr	r0, .L7
	bl	_ZN5IOPin11read_outputEv
	mov	r3, r0
	cmp	r3, #0
	ite	eq
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L4
	movs	r1, #1
	ldr	r0, .L7
	bl	_ZN5IOPin5writeEi
	b	.L6
.L4:
	movs	r1, #0
	ldr	r0, .L7
	bl	_ZN5IOPin5writeEi
	b	.L6
.L8:
	.align	2
.L7:
	.word	a2
	.word	655350
	.size	main, .-main
	.section	.text._Z41__static_initialization_and_destruction_0ii,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_Z41__static_initialization_and_destruction_0ii, %function
_Z41__static_initialization_and_destruction_0ii:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #16
	add	r7, sp, #8
	str	r0, [r7, #4]
	str	r1, [r7]
	ldr	r3, [r7, #4]
	cmp	r3, #1
	bne	.L11
	ldr	r3, [r7]
	movw	r2, #65535
	cmp	r3, r2
	bne	.L11
	movs	r3, #4
	str	r3, [sp]
	movs	r3, #1
	movs	r2, #13
	movs	r1, #2
	ldr	r0, .L12
	bl	_ZN5IOPinC1Eiiii
	ldr	r2, .L12+4
	ldr	r1, .L12+8
	ldr	r0, .L12
	bl	__aeabi_atexit
.L11:
	nop
	adds	r7, r7, #8
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
.L13:
	.align	2
.L12:
	.word	a2
	.word	__dso_handle
	.word	_ZN5IOPinD1Ev
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.section	.text._GLOBAL__sub_I_a2,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_GLOBAL__sub_I_a2, %function
_GLOBAL__sub_I_a2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	add	r7, sp, #0
	movw	r1, #65535
	movs	r0, #1
	bl	_Z41__static_initialization_and_destruction_0ii
	pop	{r7, pc}
	.size	_GLOBAL__sub_I_a2, .-_GLOBAL__sub_I_a2
	.section	.init_array,"aw",%init_array
	.align	2
	.word	_GLOBAL__sub_I_a2(target1)
	.hidden	__dso_handle
	.ident	"GCC: (GNU Tools for Arm Embedded Processors 7-2017-q4-major) 7.2.1 20170904 (release) [ARM/embedded-7-branch revision 255204]"
