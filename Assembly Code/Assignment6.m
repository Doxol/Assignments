/* 
David Sepulveda
10161931
Assignment 6
*/
include(./macro_defs.m)
	.section	".data"
	.align	8		! aligns 8
c:	.double	0r1.e-10	! compare value
one:	.double	0r1.0		! 1.0
negO:	.double	0r-1.0	! -1.0
zero:	.double	0r0.0		! 0.0
temps:.double	0r0.0		! temporary float		
tmps:	.double	0r0.0		! temporary float
	.section	".text"
table:	.asciz "\tx\t| e^x\t\t\t| e^-x\n"	! print statement for table
input:	.asciz " %.9f"				! print statement for the input
ex:		.asciz "\t| %.9f   \t| %.9f\n"	! print statement for the output
error:	.asciz "Input is incorrect.\n"	! print statement for errors
		.align 4					! aligns 4
local_var		
var(buf, 1, 1*8)
.global main 						! makes main global
main:	save	%sp, (-92 + last_sym) & -8, %sp	! allocates space on the stack
	cmp	%i0, 2					! makes sure there are only two command line argumentss, including the file name
	bne	errors					! if there are more or less than two arguments, there is an error
	nop							! nop
	set	table, %o0					! sets %o0 as table print statement
	call	printf 					! calls printf
	nop							! nop
	ld	[%i1 + 4], %o0				! loads the input file name into %o0
	clr	%o1						! clears %o1
	clr	%o2						! clears %o2
	mov	5, %g1					! sets a trap to open the file specified
	ta	0						! trap
	bcc	open						! branches
	nop							! nop
	ba	end						! branches to end always
	nop							! nop
open:	mov	%o0, %l0					! moves %o0 to %l0
	mov	0, %l2					! moves 0 to %l2
read:	mov	%l0, %o0					! moves %l0 to %o0
	add	%fp, buf, %o1				! adds buf to frame pointer to get the reference for buf on the stack and stores it in %o1
	mov	8, %o2					! moves 8 to %o2, buffer size
	mov	3, %g1					! reads the file
	ta	0						! trap
	cmp	%o0, 8					! compares the input read to the buffer size
	bne	end						! branches to end if not equal
	nop							! nop
	set	input, %o0					! sets %o0 as input print statement
	ld	[%fp + buf], %o1				! loads the high bits of the buffer into %o1
	ld	[%fp + buf + 4], %o2			! loads the low bits of the buffer int %o2
	call	printf 					! calls printf
	nop							! nop
	ld	[%fp + buf], %o0				! reloads the input
	ld	[%fp + buf + 4], %o1			! reloads the input
	call	exp						! calls exponential subroutine
	nop							! nop
	set	temps, %l3					! sets %l3 as temps float
	std	%f0, [%l3]					! stores the double %f0 into %l3
	set	tmps, %l4					! sets %l4 as tmps
	std	%f2, [%l4]					! stores the double %f2 into %l4
	set	ex, %o0					! sets %o0 as the output print statement
	ld	[%l4], %o1					! loads the high bits of %l4 into %o1
	ld	[%l4 + 4], %o2				! loads the low bits of %l4 into %o2
	ld	[%l3], %o3					! loads the high bits of %l3 into %o3
	ld	[%l3 + 4], %o4				! loads the low bits of %l3 into %o4
	call	printf 					! calls printf
	nop							! nop
here:	ba	read						! loops to read
	nop							! nop
	mov	%l0, %o0					! moves %l0 to %o0
	mov	6, %g1					! closes the file
	ta	0						! trap
end:	mov	1, %g1					! closes the program
	ta	0						! trap
local_var
var(temp, 1, 1*8)
.global exp							! makes exp global
exp:	save	%sp, (-92 + last_sym) & -8, %sp	! shifts the window
	mov	%i0, %l0					! moves the first argument into %l0
	mov	%i1, %l1					! moves the second argument into %l1
	std	%l0, [%fp + temp]				! stores the double passed in into a temporary place on the stack
	ldd	[%fp + temp], %f0				! loads the double in the temporary space on the stack into %f0
	set	c, %o0					! sets %o0 as the compare value
	ldd	[%o0], %f2					! loads the double compare value into %f2
	set	one, %o0					! sets %o0 as 1.0
	ldd	[%o0], %f4					! loads 1.0 into %f4
	ldd	[%o0], %f6					! loads 1.0 into %f6
	ldd	[%o0], %f8					! loads 1.0 into %f8
	ldd	[%o0], %f10					! loads 1.0 into %f10
	ldd	[%o0], %f14					! loads 1.0 into %f14
	ldd	[%o0], %f20					! loads 1.0 into %f20
	set	zero, %o0					! sets %o0 as 0.0
	ldd	[%o0], %f12					! loads 0.0 into %f12
	set	negO, %o0					! sets %o0 as -1.0
	ldd	[%o0], %f16					! loads -1.0 into %f16
	ldd	[%o0], %f18					! loads -1.0 into %f18
loop:	faddd	%f14, %f12, %f14				! loop for calculating e^x and e^-x, adds the current term to our e^x register
	fmuld	%f18, %f16, %f18				! (-1)^x
	fmuld	%f12, %f18, %f12				! (-1)^x * current term
	faddd	%f20, %f12, %f20				! adds current term to e^-x
	fmuld	%f12, %f18, %f12				! (-1)^x * current term (to restore the sign value)
	fmuld	%f6, %f4, %f6				! factorial calculation, multiplies our factorial register by the current count
	fmuld	%f10, %f0, %f10				! exponential calculator, multiplies our x^n register by x
	fdivd	%f10, %f6, %f12				! divides x^n by our facotiral, x^n/n!
	faddd	%f4, %f8, %f4				! increments count
	fcmpd	%f12, %f2					! compares current term to compare value
	nop							! nop to prevent problems with branching
	fbge	loop						! branches back to loop while current term is greater than or equal to current value
	nop							! nop
	std	%f20, [%fp + temp]			! stores e^-x into a temporary space on the stack
	ldd	[%fp + temp], %f0				! loads e^-x into %f0
	std	%f14, [%fp + temp]			! stores e^x into a temporary space on the stack
	ldd	[%fp + temp], %f2				! loads e^x into %f2
expE:	ret							! shifts the window back
	restore						! restores control to the program
errors:set	error, %o0					! error code, sets %o0 as error
	call	printf 					! calls printf
	nop							! nop
	ba	end						! branches always to end
	nop							! nop
