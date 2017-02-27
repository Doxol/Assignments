/*
Assignment 3
Name:	David Sepulveda
ID:	10161931
Version: 	V4
*/

define(local_var, `define(last_sym, 0)')
define(`var',  `define(`last_sym', eval((last_sym - ifelse($3,,$2,$3)) & -$2)) $1 = last_sym')	! calculating the block size of the array
local_var						
var(arr_s, 4, 40*4)					! allocates space for our array

define(i_r, l0)					! l0 will be our i counter
define(j_r, l4)					! l4 will be our j counter
define(max_r, l1)					! l1 will be our max register
define(temp, l2)					! l2 will be our temp register

/*
loopr = loop random
loops = loop sorted
loopi = loop inner
loope = loop sorted end
loopp = loop print
*/
array:	.asciz	"v[%-d]:\t%d\n"			! print statement for each array index
sort:	.asciz	"\nSorted Array: \n"			! print statement for my sorted array
	.align	4				! aligns
.global main 					! globalizes main
main:	save	%sp, (-92 + last_sym) & -8, %sp		! initializes stack
	clr	%i_r				! clears the i counter register
loopr:	call	rand				! calls random
	mov	39, %max_r			! moves 39 into the maximum register
	and	%o0, 0xFF, %o2			! applies mod 256 to the random value
	sll	%i_r, 2, %o3			! multiplies i by 4 and stores the result in %o3
	add	%o3, %fp, %o3			! adds 4i to the frame pointer and stores the result in %o3
	st	%o2, [%o3  + arr_s]			! stores the random value in the stack
	set	array, %o0				! sets the array print statement to %o0
	call	printf				! calls printf
	mov	%i_r, %o1				! movs the i counter value into %o1
	cmp	%i_r, %max_r			! compares the i counter to the maximum register
	bl	loopr				! branches up to the loop for the random array
	inc 	%i_r				! increments the i counter
	set	sort, %o0				! sets the sort print statement into %o0
	call	printf 				! calls printf
	mov	1, %i_r				! sets the i counter to 1
loops:	sll	%i_r, 2, %l7			! 4 * i into l7
	add	%fp, %l7, %l7			! adds %l7 to the frame pointer and stores it in %l7
	ld	[%l7  + arr_s], %temp			! gets the value on the stack at position %fp + (4 * i) and puts the value in %temp
	mov	%i_r, %j_r				! j = i
loopi:	sll	%j_r, 2, %l7			! multiplies j by 4
	cmp	%j_r, 0				! for j > 0, given my implementation, it looks like it is not branching on the complement of >, but it is.
	bl	loope				! branches past the loop
	add	%fp, %l7, %l7			! adds %l7 to the frame pointer
	sub	%j_r, 1, %l6			! %l6 = j - 1
	sll	%l6, 2, %l6				! multiplies %o0 by 4
	add	%fp, %l6, %l6			! adds %o0 to the frame pointer
	ld	[%l6  + arr_s], %l5			! loads v[j-1]
	cmp	%temp, %l5			! if %temp < v[j-1]
	bge	loope				! branches past the loop
	dec	%j_r				! decrements j
	ba	loopi				! branches back to loopi
	st	%l5, [%l7 + arr_s]			! stores v[j-1] in v[j]
loope:	st	%temp, [%l7  + arr_s]			! stores temp into v[j]
	cmp	%i_r, %max_r			! compares the i counter and the max
	bl,a	loops				! branches to loops if the i counter has yet to reach the max
	inc 	%i_r				! increments the i counter, unless we are done looping 
	clr	%i_r				! clears the i register
loopp:	sll	%i_r, 2, %l3			! calculates the offset and stores it in %l3
	add	%fp, %l3, %l5			! adds the offset to the frame pointer and stores the result in %o0
	ld	[%l5 + arr_s], %o2			! loads the value at that position, stores it in %o2
	set	array, %o0 			! sets the array print statment to %o0
	call	printf 				! calls printf
	mov	%i_r, %o1				! moves the i counter into %o1
	cmp	%i_r, %max_r			! compares the i counter to the size of the array
	bl	loopp				! branches to loopp if the counter is less than the size of the array
	inc 	%i_r				! increments the counter
end:	mov	1, %g1				! things
	ta	0				! and more things
