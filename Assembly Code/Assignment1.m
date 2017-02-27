! David Sepulveda, 10161931
! Assignment1.m v2

define(mini_r, l0)		! Register for the minimum value.
define(x_r, l1)			! Register for the x value.
define(x2_r, l2)			! Register for the x^2 value.
define(y_r, l3)			! Register for the y value.

define(c1, -18)			! Constant: -18
define(c2, 10)			! Constant:  10
define(c3, 39)			! Constant:  39

define(iv, -2)			! Start value: -2
define(max, 11)			! Start value: 11

mini:	.asciz "Minimum Value: %d, %d, %d\n"	! Print statement
	.align 4						! Aligns things

.global main 			! Globalizes main
main: save	%sp, -96, %sp	! Saves -96 to the stack pointer?
	mov	iv, %x_r		! Initial x value -2
	mov   1000, %mini_r	! Sets minimum to a value unlikely to be encountered

loop:	mov 	%x_r, %o0		! o0 = x
	call	.mul			! x * x
	mov 	%x_r, %o1		! o1 = x, o0 = x^2
	
	mov 	%o0, %x2_r		! x2_r = x^2
	call	.mul			! x^2 * x
	mov	%x_r, %o1		! o0 = x^3

	add	%o0, c3, %y_r	! y_r = x^3 + 39
	add   %o0, %y_r, %y_r	! y_r = 2x^3 + 39
	
	mov 	c1, %o0		! o0 = -18
	call	.mul			! -18 * x^2
	mov 	%x2_r, %o1		! o0 = -18x^2
	
	add 	%o0, %y_r, %y_r	! y_r = 2x^3 - 18x^2 + 39
	
	mov 	%x_r, %o0		! o0 = x
	call	.mul			! 10 * x
	mov 	c2, %o1		! o0 = 10x
	
	add 	%o0, %y_r, %o2	! o2 = 2x^3 - 18x^2 + 10x + 39
	
	cmp 	%o2, %mini_r	! Checks whether or not the current y is less than the minimum.
	bge	jump			! Branches to jump if current y is greater than or equal to the minimum.
	
	mov 	%x_r, %o1		! %o1 = x
	mov 	%o2, %mini_r	! %mini_r = 2x^3 - 18x^2 + 10x + 39

jump: set 	mini, %o0		! %o0 = mini
	call	printf 		! prints: mini x, y, minimum
	mov	%mini_r, %o3	! o3 = minimum
	cmp	%x_r, max		! Checks if x is less than 11 before iterating again.
	bl	loop			! Loops if x is less than 11.
	inc	%x_r			! Increments x.
	
	mov	1, %g1		! Sets end to true?
	ta	0			! Something.
