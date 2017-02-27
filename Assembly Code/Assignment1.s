! David Sepulveda, 10161931
! Assignment1.s v1

mini:	.asciz "Minimum Value: %d, %d, %d\n"	! Print statement
	.align 4						! Aligns things

.global main						! Globalizes main
main:								! Main label
	save	%sp, -96, %sp				! Saves -96 to the stack pointer?

	mov	-3, %l8		! Sets l8 to -3
	mov	0, %l5		! Sets l5 to 0
loop:					! Loop label
	inc	%l8			! Increments l8

	mov 	%l8, %l1		! Sets l1 to the value in l8
	mov 	%l1, %o0		! Sets o0 to the value in x
	mov 	%l1, %o1		! Sets o1 to the value in x
	call	.mul			! x * x
	nop				! Delay slot

	mov 	%o0, %l2		! l2 = x^2
	mov	%l1, %o1		! l1 = x
	call	.mul			! x^2 * x
	nop				! Delay slot

	mov	%o0, %l3 		! l3 = x^3
	mov 	2, %o1		! Sets o1 to 2.
	call	.mul			! 2 * x^3
	nop				! Delay slot

	mov 	%o0, %l3 		! l3 = 2x^3
	mov 	-18, %o0		! Sets o0 to -18.
	mov 	%l2, %o1		! Sets o1 to x^2.
	call	.mul			! -18 * x^2
	nop				! Delay slot

	mov 	%o0, %l2		! l2 = -18x^2
	mov 	%l1, %o0		! Sets o0 to x.
	mov 	10, %o1		! Sets o1 to 10.
	call	.mul			! 10 * x
	nop				! Delay slot

	mov 	%o0, %l4		! l4 = 10x
	add 	%l3, %l2, %o0	! 2x^3 - 18x^2
	add 	%l4, 39, %o1	! 10x + 39
	add 	%o0, %o1, %o2	! 2x^3 - 18x^2 + 10x + 39 -> %o2
	
	cmp 	%o2, %l0		! Compares the y value to the minimun value.
	ble	minim 		! Branches when the y value is less than the minimum value.
	nop				! Delay slot
back:					! Back label
	set 	mini, %o0		! Puts the print statement into o0.
	mov 	%l1, %o1		! %o0 = mini, %o1 = -2, %o2 = 2x^3 - 18x^2 + 10x + 39
	mov	%l0, %o3		! %o3 = minimum value
	call	printf		! Prints the print statement.
	nop				! Delay slot

	cmp	%l8, 11
	bl	loop			! Branches to loop when l8 is less than 11.
	nop				! Delay slot

	mov	1, %g1		! Sets end to true?
	ta	0			! Something.

minim:				! Checking lowest value.
	mov 	%o2, %l0		! Sets l0 to the y value.
	ba	back			! Always branches to back.
	nop				! Delay slot
