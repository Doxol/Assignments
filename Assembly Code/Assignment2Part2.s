/*	Name: David Sepulveda
	ID:	10161931
*/

result:		.asciz "Result: %08x and %08x\n\n"	! result print statement 
multiplicand:	.asciz "Multiplicand: %08x\n"		! multiplicand print statement
multiplier: 	.asciz "Multiplier: %08x\n"		! multiplier print statement
			.align 4			! aligns

.global main					! globalizes main
main:	save 	%sp, -96, %sp			! sets the stack pointer

	set 	0x04ee67b7, %l0 			! sets first input multiplicand to l0
	set 	0x072e8b8c, %l1			! sets first input multiplier to l1
	set	0x80000000, %l6			! all bits 0 execept for the 31st bit
	clr	%l4					! input counter

start:clr 	%l7 					! loop counter
	clr 	%l2					! clears product register
	and	%l1, %l6, %l5			! checks if the multiplier is negative
	set	multiplicand, %o0			! sets the multiplicand print statement to o0
	call	printf				! prints the multiplican
	mov	%l0, %o1				! moves the multiplicand to o1
	set	multiplier, %o0			! moves the multiplier print statement to o0
	call	printf				! prints the multiplier
	mov	%l1, %o1				! moves the multiplier to o1

loop:	btst	1, %l1 				! checks if l1 is 1
	be 	down					! if 0 branch
	inc	%l7					! increments the loop counter
	add	%l0, %l2, %l2			! adds the multiplicand to the product

down:	srl 	%l1, 1, %l1				! shifts the multiplier to the right once
	btst	1, %l2				! checks the first bit in the product
	be	here					! branches on 0
	sra	%l2, 1, %l2				! shifts the product right arithmetically
	or	%l1, %l6, %l1			! sets the 31st bit to 1

here:	cmp	%l7, 32				! checks the loop counter
	bl	loop					! branches when iterations are less than 32
	andcc	%l5, %l6, %g0			! checks if the negative boolean was toggled
	be 	jump					! branches to jump if positive
	mov 	%l1, %o2				! sets o2 to the multiplier
	sub	%l2, %l0, %l2			! subtracts the multiplicand from the finished product

jump:	set 	result, %o0				! sets the result print statement to o0
	mov 	%l2, %o1				! moves the product to the o1 register
	call	printf				! prints the result
	inc	%l4					! increments the input counter
	cmp	%l4, 1				! compares the input counter to 1
	set 	0xf8d17474, %l1			! sets second multiplier
	be 	start					! branches on input counter equal to 1
	nop						! nop
	cmp	%l4, 2				! compares the input counter to 2
	set 	0xfb119849, %l0			! sets the third multiplicand
	be 	start					! branches when the input counter is 2
	nop						! nop

	mov	1, %g1				! things
	ta 	0 					! also the things
