/* David Sepulveda
    Assignment 4
    10161931
*/
include(./macro_defs.m)

define(FALSE, 0)									! makes FALSE 0
define(TRUE, 1)									! makes TRUE 1

begin_struct(point)									! begins struct point
field(x, 4)										! specifies that we need 4 bytes for x
field(y, 4)										! specifies that we need 4 bytes for y
end_struct(point)									! end struct point

begin_struct(dimension)								! begins struct dimension
field(width, 4)										! specifies tht we need 4 bytes for width
field(height, 4)										! specifies that we need 4 bytes for height
end_struct(dimension)									! end struct dimension

begin_struct(box)									! begin struct box
field(origin, align_of_point, size_of_point)						! nested struct of type point
field(size, align_of_dimension, size_of_dimension)					! nested struct of type dimension
field(area, 4)										! specifies that we need 4 bytes for area
end_struct(box)									! end struct box

local_var										! instantiates last sym
var(first, align_of_box, size_of_box)							! instantiates first box
var(second, align_of_box, size_of_box)						! instantiates second box

initialBox:	.asciz	"Initial box values: \n"						! print statement for inital box values
changeBox:	.asciz	"\nChanged box values: \n"					! print statement for changed box values
firstP:		.asciz	"first"								! print statement for first
secondP:	.asciz	"second"							! print statement for second
boxP1:		.asciz	"Box %s origin = (%d, %d)\twidth = %d\theight = %d\t"	! print statement for printBox part 1
boxP2:		.asciz	"area = %d\n"							! print statement for printBox part 2
		.align 4							! aligns
.global main 								! globalizes main
main:	save	%sp, (-92 + last_sym) & -8, %sp			! allocates memory

	set	initialBox, %o0						! sets %o0 as initialBox
	call	printf 							! calls printf

	add	%fp, first, %l0						! adds %fp and first and stores the value in %l0
	call	newBox						! calls newBox
	st	%l0, [%sp + 64]					! stores %l0 for later use
	.word size_of_box						! makes sure there is enough space to store values
anb1:
	add	%fp, second, %l1					! adds %fp and second and stores the value in %l1
	call	newBox						! calls newBox
	st	%l1, [%sp + 64]					! stores %l1 for later use
	.word size_of_box						! makes sure there is enough space to store values
anb2:
	set	firstP, %o0						! sets %o0 as firstP
	call	printBox						! calls printBox
	mov	%l0, %o1						! moves %l0 to %o1

	set	secondP, %o0						! sets %o0 as secondP
	call	printBox						! calls printBox
	mov	%l1, %o1						! moves %l1 to %o1

	mov	%l0, %o0						! moves %l0 to %o0
	call	equal							! calls equal
	mov	%l1, %o1						! moves %l1 to %o1

	cmp	%o0, 1							! checks whether true or false
	bne	notE							! branches to notE when first is not equal to second
	mov	%l0, %o0						! moves %l0 to %o0
bmov:
	mov	-5, %o1						! moves -5 to %o1
	call	move							! calls move
	mov	7, %o2							! moves 7 to %o2
amov:
	mov	%l1, %o0						! moves %l1 to %o0
	call	expand						! calls expand
	mov	3, %o1							! moves 3 to %o1
aex:
notE:	set	changeBox, %o0					! sets %o0 as changeBox
	call	printf 							! calls printf
	nop								! nop

	set	firstP, %o0						! sets %o0 as firstP
	call	printBox						! calls printBox
	mov	%l0, %o1						! moves %l0 to %o1

	set	secondP, %o0						! sets %o0 as secondP
	call	printBox						! calls printBox
	mov	%l1, %o1						! moves %l1 to %o1
end:
	mov	1, %g1							! moves 1 to %g1
	ta	0							! trap

local_var								! instantiates last sym
var(b, align_of_box, size_of_box)					! instantiates box b

newBox:save	%sp, (-92 + last_sym) & -8, %sp 			! shifts the window

	ld	[%i7 + 8], %o1						! security check
	cmp	%o1, size_of_box					! ensures that there is enough space to store the struct
	bne	return							! if there isnt enough space, branch to return
	mov	1, %l0							! moves 1 to %l0

	ld	[%fp + 64], %o0					! loads %l0 from main
	st	%g0, [%fp + b + box_origin + point_x]			! stores 0 in the space reserved for b.origin.x
	st	%g0, [%fp + b + box_origin + point_y]			! stores 0 in the space reserved for b.origin.y
	st	%l0, [%fp + b + box_size + dimension_width]		! stores 1 in the space reserved for b.size.width
	st	%l0, [%fp + b + box_size + dimension_height]	! stores 1 in the space reserved for b.size.height
	ld	[%fp + b + box_size + dimension_width], %l6		! loads the value b.size.width
	ld	[%fp + b + box_size + dimension_height], %l7	! loads the value b.size.height
	smul	%l6, %l7, %o1						! multiplies the width by the height
	st	%o1, [%fp + b + box_area]				! stores the area in b.area

	ld	[%fp + b + box_origin + point_x], %o1			! loads b.origin.x
	st	%o1, [%o0 + box_origin + point_x]			! returns b.origin.x

	ld	[%fp + b + box_origin + point_y], %o1			! loads b.origin.y
	st	%o1, [%o0 + box_origin + point_y]			! returns b.origin.y

	ld	[%fp + b + box_size + dimension_width], %o1		! loads b.size.width
	st	%o1, [%o0 + box_size + dimension_width]		! returns b.size.width

	ld	[%fp + b + box_size + dimension_height], %o1	! loads b.size.height
	st	%o1, [%o0 + box_size + dimension_height]		! returns b.size.height

	ld	[%fp + b + box_area], %o1				! loads b.area
	st	%o1, [%o0 + box_area]				! returns b.area

return:	jmpl	%i7 + 12, %g0						! gives control back to main
	restore								! shifts the window back

move:	save	%sp, (-92) & -8, %sp 					! shifts the window

	ld	[%i0 + box_origin + point_x], %o1			! loads b.origin.x
	add	%o1, %i1,%o1						! adds %o1 and %i1 and stores the value in %o1
	st	%o1, [%i0 + box_origin + point_x]			! stores %o1 in b.origin.x

	ld	[%i0 + box_origin + point_y], %o0			! loads b.origin.y
	add	%o0, %i2,%o1						! adds %o0 and %i2 and stores the value in %o1
	st	%o1, [%i0 + box_origin + point_y]			! stores %o1 in b.origin.y

	ret								! gives control back to main
	restore								! shifts the window back	

expand:save	%sp, (-92) & -8, %sp 					! shifts the window

	ld	[%i0 + box_size + dimension_width], %o1		! loads b.size.width into %o1
	smul	%i1, %o1, %o1					! multiplies %o1 by %i1 and stores the result in %o1
	st	%o1, [%i0 + box_size + dimension_width]		! stores %o1 in b.size.width

	ld	[%i0 + box_size + dimension_height], %o1		! loads b.size.height into %o1
	smul	%i1, %o1, %o1					! multiplies %o1 by %i1 and stores the result in %o1 
	st	%o1, [%i0 + box_size + dimension_height]		! stores %o1 in b.size.height

	ld	[%i0 + box_size + dimension_width], %o0		! loads b.size.width into %o0
	ld	[%i0 + box_size + dimension_height], %o1		! loads b.size.height into %o1
	smul	%o0, %o1, %o0					! multiplies %o0 by %o1 and stores the result in %o0
	st	%o0, [%i0 + box_area]				! stores %o0 in b.area

	ret								! gives control back to main
	restore								! shifts the window back

printBox:save	%sp, (-92) & -8, %sp					! shifts the window

	set	boxP1, %o0						! sets %o0 as boxP1
	ld	[%i1 + box_origin + point_x], %o2			! loads b.origin.x into %o2
	ld	[%i1 + box_origin + point_y], %o3			! loads b.origin.y into %o3
	ld	[%i1 + box_size + dimension_width], %o4		! loads b.size.width into %o4
	ld	[%i1 + box_size + dimension_height], %o5		! loads b.size.height into %o5
	call	printf 							! calls printf
	mov	%i0, %o1						! moves %i0 to %o1

	set	boxP2, %o0						! sets %o0 as boxP2
	call	printf 							! calls printf
	ld	[%i1 + box_area], %o1				! loads b.area into %o1

	ret								! gives control back to main
	restore								! shifts the window back

equal:	save	%sp, (-92) & -8, %sp					! shifts the window

	mov	FALSE, %l0						! moves 0 (FALSE) into %l0
	ld	[%i0 + box_origin + point_x], %o0			! loads b1.origin.x into %o0
	ld	[%i1 + box_origin + point_x], %o1			! loads b2.origin.x into %o1

	cmp	%o0, %o1						! compares b1.origin.x to b2.origin.y
	bne,a	returnE							! branches if they are not equal
	mov	%l0, %i0						! moves %l0 to %i0
	ld	[%i0 + box_origin + point_y], %o0			! loads b1.origin.y into %o0
	ld	[%i1 + box_origin + point_y], %o1			! loads b2.origin.y into %o1

	cmp	%o0, %o1						! compares b1.origin.y to b2.origin.y
	bne,a	returnE							! branches if they are not equal
	mov	%l0, %i0						! moves %l0 to %i0
	ld	[%i0 + box_size + dimension_width], %o0		! loads b1.size.width into %o0
	ld	[%i1 + box_size + dimension_width], %o1		! loads b2.size.width into %o1

	cmp	%o0, %o1						! compares b1.size.width to b2.size.width
	bne,a	returnE							! branches if they are not equal
	mov	%l0, %i0						! moves %l0 to %i0
	ld	[%i0 + box_size + dimension_height], %o0		! loads b1.size.height into %o0
	ld	[%i1 + box_size + dimension_height], %o1		! loads b2.size.height into %o1

	cmp	%o0, %o1						! compares b1.size.height to b2.size.height
	bne,a	returnE							! branches if not equal
	mov	%l0, %i0						! moves %l0 to %i0
	mov	TRUE, %i0						! moves 1 (TRUE) to %i0

returnE:ret								! gives control back to main
	restore								! shifts window back
