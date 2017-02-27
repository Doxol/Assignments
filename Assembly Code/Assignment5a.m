/*
David Sepulveda
10161931
Assignment 5
*/

define(TRUE, 1)		! replaces all TRUE's with 1
define(FALSE, 0)		! replaces all FALSE's with 0
define(STACKSIZE, 5)	! replaces all STACKSIZE's with 5

overflow:	.asciz "\nStack overflow! Cannot push value onto stack.\n"	! stack overflow print statement
underflow:	.asciz "\nStack underflow! Cannot pop an empty stack.\n"	! stack underflow print statement
empty:	.asciz "\nEmpty stack\n"						! empty stack print statement
stackCur:	.asciz "\nCurrent stack contents:\n"				! current contents of the stack print statement
stackTop:	.asciz " <-- top of stack"						! top of stack print statement
stackE:	.asciz "  %d"								! stack element print statement
new:		.asciz "\n"									! newline print statement
		.align 4									! aligns

.data							! makes this section data so it is readable and writable
.global top 					! makes top global
top:		.word	-1 				! top of the stack

.global stack 					! makes stack global
stack:	.skip	5 * 4				! stack
.text							! makes this section text so it is readable

.global push 					! makes push global
push:							! function for push a value onto the stack
	save	%sp, (-92) & -8, %sp		! shifts the window
	call	stackFull				! calls stackFull
	nop						! nop
	cmp	%o0, FALSE				! compares the result of stackFull with FALSE
	be	notFull				! branches to notFull if stackFull is false
	nop						! nop
	set	overflow, %o0			! sets %o0 as overflow
	call	printf 				! calls printf
	nop						! nop
	ba	pushE					! branches always to pushE
	nop						! nop
notFull:
	sethi	%hi(top), %o0			! sets %o0 as the high bits of top
	ld	[%o0 + %lo(top)], %l0		! sets %l0 as the low bits of top
	inc	%l0					! increments top
	set 	top, %o0				! sets %o0 as top
	st 	%l0, [%o0]				! updates the valueof top
	sll 	%l0, 2, %l1				! multiplies the value of top by 4
	set	stack, %l2				! sets %l2 as stack
	st 	%i0, [%l2 + %l1]			! stores the value stored in %i0 into the corresponding space in the stack
pushE:
	ret						! returns control
	restore					! restores the window

.global pop						! makes pop global
pop:							! function for popping a value off the stack
	save	%sp, (-92) & -8, %sp		! shifts the window
	call	stackEmpty				! call stackEmpty
	nop						! nop
	cmp	%o0, FALSE				! compares the output of stackEmpty to FALSE
	be	notEmpty				! branches to notEmpty if stackEmpty returns FALSE
	nop						! nop
	set	underflow, %o0			! set %o0 as underflow
	call	printf 				! call printf
	nop						! nop
	mov	-1, %i0				! returns -1
	ba	popE					! branches always to popE
	nop						! nop
notEmpty:
	sethi	%hi(top), %o0			! sets %o0 as the high bits of top
	ld	[%o0 + %lo(top)], %l0		! loads top into %l0
	sll 	%l0, 2, %o2				! multiples top by 4
	set	stack, %o1				! sets %o1 as stack
	ld 	[%o1 + %o2], %o3			! loads the value at stack[top]
	dec	%l0					! decrements top
	set 	top, %o0				! sets %o0 as top
	st 	%l0, [%o0]				! updates the value of top
	mov	%o3, %i0				! returns the value at stack[top]
popE:
	ret						! returns control
	restore					! restores the window

.global stackFull					! makes stackFull global
stackFull:						! function to check if the stack is full
	save	%sp, (-92) & -8, %sp		! shifts the window
	mov	STACKSIZE, %o1			! moves STACKSIZE to %o1
	sub	%o1, 1, %o0				! %o0 = STACKSIZE - 1
	sethi	%hi(top), %o3			! sets %o3 as the high bits of top
	ld	[%o3 + %lo(top)], %o2		! loads top into %o2
	cmp	%o2, %o0				! compares top and STACKSIZE - 1
	bne	stackFE				! branches when top != STACKSIZE - 1 to stackFE
	mov	FALSE, %i0				! returns FALSE
	mov	TRUE, %i0				! returns TRUE
stackFE:
	ret						! returns control
	restore					! restores the window

.global stackEmpty				! makes stackEmpty global
stackEmpty:
	save	%sp, (-92) & -8, %sp		! shifts the window
	sethi	%hi(top), %o0			! sets %o0 as the high bits of top
	ld	[%o0 + %lo(top)], %l0		! loads top into %l0
	cmp	%l0, -1				! compares top to -1
	bne 	stackEnd				! branches when top != -1 to stackEnd
	mov	FALSE, %i0				! returns FALSE
	mov	TRUE, %i0				! returns TRUE
stackEnd:
	ret						! returns control
	restore					! restores the window

.global display					! makes display global
display:						! function for printing the contents of the stack
	save	%sp, (-92) & -8, %sp		! shifts the window
	call	stackEmpty				! calls stackEmpty
	nop						! nop
	cmp	%o0, FALSE				! compares the result of stackEmpty to FALSE
	be	this					! branches to this if stackEmpty returns FALSE
	nop						! nop
	set 	empty, %o0				! sets %o0 as empty
	call	printf 				! calls printf
	nop						! nop
	ba	after					! branches always to after
	nop						! nop
this:
	set 	top, %o0				! sets %o0 as top
	ld	[%o0], %l0				! dereferences top and stores it in %l0
	set 	stackCur, %o0			! sets %o0 as stackCur
	call	printf 				! calls printf
	mov	%l0, %l1				! moves top to %l1
loop:
	cmp	%l1,0					! compares %l1 to 0
	bl	after					! branches when %l1 < 0
	nop						! nop
	set	stackE, %o0				! sets %o0 as stackE
	sll 	%l1, 2, %o1				! multiplies %l1 by 4
	set 	stack, %o3				! sets %o3 as stack
	ld	[%o3 + %o1], %o2			! loads stack[%l0] into %o2
	call	printf 				! calls printf
	mov	%o2, %o1				! moves stack[%l0] into %o1
	cmp	%l0, %l1				! compares %l1 to top
	bne	hey					! branches when the current element is not the top element
	nop						! nop
	set 	stackTop, %o0			! sets %o0 as stackTop
	call	printf 				! calls printf
	nop						! nop
hey:
	set 	new, %o0				! sets %o0 as new
	call	printf 				! calls printf
	nop						! nop
	ba	loop					! branches always to loop
	dec	%l1					! decrements %l1
after:
	ret						! returns control
	restore					! restores the window