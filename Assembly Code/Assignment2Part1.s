/*	Name: David Sepulveda
	ID:	10161931
*/

data:		.asciz "Input: %x\n"		! data print statement
checksum:	.asciz "\nChecksum: %x\n\n"	! checksum print statement
		.align 4				! aligns

.global main					! gloablizes main
main:	save 	%sp, -96, %sp			! sets the stack pointer

	set 	0x0000ffff, %l0		! sets l0 to 0x0000ffff
	set 	0x00000001, %l2		! sets l2 to 0x00000001
	set	0x80000000, %l3		! sets l3 to 0x80000000
	set 	0xaaaa8c01, %l4		! moves the l4 input into l4
	set 	0xff001234, %l5		! moves the l5 input into l5
	set 	0x13579bdf, %l6		! moves the l6 input into l6
	set 	0xc8b4ae32, %l7		! moves the l7 input into l7
	clr	%l8				! sets l8 to 0 (loop counter)

	set	checksum, %o0		! puts checksum print statement into o0
	call	printf			! prints initial checksum
	mov	%l0, %o1			! moves l0 into %o1

	set	data, %o0			! puts data print statement into o0
	call	printf			! prints l4 input
	mov	%l4, %o1			! puts l4 into o1

	set	data, %o0			! puts data print statement into o0
	call	printf			! prints l5 input
	mov	%l5, %o1			! puts l5 into o1

	set	data, %o0			! puts data print statement into o0
	call	printf			! prints l6 input
	mov	%l6, %o1			! puts l6 into o1

	set	data, %o0			! puts data print statement into o0
	call	printf			! prints the l7 input
	mov	%l7, %o1			! puts l7 into o1

loop:	sll	%l0, 1, %l0 		! shifts l0 left
	andcc	%l4, %l3, %g0		! checks the last bit in l4
	be	first				! branches if 0
	sll	%l4, 1, %l4			! shifts l4 left
	or 	%l0, %l2, %l0 		! sets l0 to l0 +1

first:andcc	%l5, %l3, %g0		! checks the last bit in l5
	be	jump				! branches if 0
	sll	%l5, 1, %l5			! shifts l5 left
	or 	%l4, %l2, %l4 		! sets l4 to l4 +1

jump:	andcc	%l6, %l3, %g0		! checks the last bit in l6
	be	whee				! branches if 0
	sll	%l6, 1, %l6			! shifts l6 left
	or 	%l5, %l2, %l5 		! sets l5 to l5 +1

whee: andcc	%l7, %l3, %g0		! checks the last bit in l7
	be	last				! branches if 0
	sll	%l7, 1, %l7			! shifts l7 left
	or 	%l6, %l2, %l6 		! sets l6 to l6 +1

last:	set 	0x00001021, %l1		! sets l1 to the bitmask for the xor gate
	set	0x00010000, %g2		! sets g2 to 0x00010000
	andcc	%l0, %g2, %g0		! checks the last bit in l0 
	be	end				! branches if 0
	cmp	%l8, 126			! checks the l8 loop counter
	xor	%l0, %l1, %l0 		! sets off all xor gates

end:	ble	loop				! branches when the loop has looped less than or equal to 126 times
	inc	%l8				! increments the loop counter
	set 	0x0000ffff, %g3		! bit mask
	and 	%l0, %g3, %l0		! throws out the part of the l0 register that is not the checksum
	set	checksum, %o0		! sets the checksum print statement to o0
	call	printf			! prints the final checksum
	mov	%l0, %o1			! moves the checksum to the o1 register

	mov	1, %g1			! things
	ta 	0 				! also the things
