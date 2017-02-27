/*
David Sepulveda
10161931
Assignment 4
*/

define(MAXYEAR, 2015)

.align 4		! aligns
.global month	! makes month global
month:	.word jan_m, feb_m, mar_m, apr_m, may_m, jun_m, jul_m, aug_m, sep_m, oct_m, nov_m, dec_m	! array of months
jan_m:	.asciz	"January"			! January
feb_m:	.asciz	"February"			! February
mar_m:	.asciz	"March"			! March
apr_m:	.asciz	"April"			! April
may_m:	.asciz	"May"				! May
jun_m:	.asciz	"June"			! June
jul_m:	.asciz	"July"			! July
aug_m:	.asciz	"August"			! August
sep_m:	.asciz	"September"			! September
oct_m:	.asciz	"October"			! October
nov_m:	.asciz	"November"			! November
dec_m:	.asciz	"December"			! December
wrong:	.asciz "usage mm dd yyyy\n"		! print statement for usage of the program
wrong1:	.asciz "Input out of range.\n"	! print statement for input out of range
fmtst:	.asciz "%s %dst, %d AD\n"		! print statement for days suffixed with st
fmtnd:	.asciz "%s %dnd, %d AD\n"		! print statement for days suffixed with nd
fmtrd:	.asciz "%s %drd, %d AD\n"		! print statement for days suffixed with rd
fmtth:	.asciz "%s %dth, %d AD\n"		! print statement for days suffixed with th
		.align 4					! aligns
.global main 						! makes main global
main:	save	%sp, -96, %sp				! resers space on the stack
	cmp	%i0, 4					! makes sure the number of command line arguments in 3
	bne	here						! branches to here if the number of command line arguments is not 3
	nop							! nop
	ld	[%i1 + 4], %o0				! loads the first argument
	call	atoi						! calls atoi
	nop							! nop
	cmp	%o0, 1					! compares month to 0
	bl	there						! branches to there when less than 0
	cmp	%o0, 12					! compares month to 12
	bg	there						! branches to there when months greater than 12
	nop							! nop
	! month
	mov	%o0, %l0					! moves %o0 to %l0
	sub	%l0, 1, %l1					! subtracts one from the month
	sll	%l1, 2, %l1					! multiples by 4
	set	month, %o0					! sets %o0 as month
	ld	[%o0 + %l1], %l7				! loads month[arg[1]]
	! day
	ld	[%i1 + 8], %o0				! loads the second argument
	call	atoi						! calls atoi
	nop							! nop
	mov	%o0, %l6					! moves %o0 to %l6
	! year
	ld	[%i1 + 12], %o0				! loads the third argument
	call	atoi						! calls atoi
	nop							! nop
	mov	%o0, %l5					! moves %o0 to %l5
	! range checking
	cmp	%l5, 0					! compares the year to 0
	bl	there						! branches to there when year < 0
	cmp	%l5, MAXYEAR				! compares the year to MAXYEAR
	bg	there						! branches to there when year > MAXYEAR
	cmp	%l6, 1					! compares day to 1
	bl	there						! branches to there when day < 1
	cmp	%l0, 7					! compares month to 7
	bg	high						! branches to high when month > 7
	nop
	and	%l0, 0x1, %o1				! %o1 = month mod 2
	cmp	%o1, 1					! compares %o1 to 1					
	be	large						! branches to large when %o1 = 1
	cmp	%l0, 2					! checks if month is february
	bne	small						! branches to small when month is not february
	nop							! nop
	and	%l5, 0x3, %o0				! %o0 = %l5 mod 4
	cmp	%o0, 0					! compares %o0 to 0
	bne	nLeap						! branches to nLeap if %o0 = 0
	nop							! nop
/*	and	%l5, 0x63, %o0		! This code worked under the assumption that x mod y was equivalent to "and x, y-1, z"
	cmp	%o0, 0			! unfortunately this only works for y's that are powers of 2.
	bne	leap				! The point of this code was to range check for leap years.
	nop					! Use your imagination. :D
hund:	and	%l5, 0x18F, %o0
	cmp	%o0, 0
	bne	nLeap
	nop
*/
leap:	cmp	%l6, 29					! leap year block, compares the day to 29
	bg	there						! branches to there on day > 29 (for february on leap years)
	nop							! nop
	ba	inRange					! branches always to inRange
	nop							! nop
nLeap:cmp	%l6, 28					! non-leap year block, comapres the day to 28
	bg	there						! branches to there on day > 28 (for february on non-leap years)
	nop							! nop
	ba	inRange					! branches always to inRange
	nop							! nop
high:	and	%l0, 0x1, %o1				! high block (months 8 - 12), %o1 = month mod 2
	cmp	%o1, 0					! compares %o1 to 0
	be	large						! branches to large when %o1 = 0
	nop							! nop
	ba	small						! branches always to small
	nop							! nop
large:cmp	%l6, 31					! 31 day months block, compares day to 31
	bg	there						! branches to there on day > 31
	nop							! nop
	ba	inRange					! branches always to inRange
	nop							! nop
small:cmp	%l6, 30					! 30 days month block, compares day to 30
	bg	there						! branches to there on day > 30
	nop							! nop
inRange:cmp	%l6, 1					! values in range block, compares day to 1
	be	st						! branches to st on day = 1
	cmp	%l6, 21					! compares day to 21
	be	st						! branches to st on day = 21
	cmp	%l6, 31					! compares day to 31
	be	st						! branches to st on day = 31
	cmp	%l6, 2					! compares day to 2
	be	nd						! branches to nd on day = 2
	cmp	%l6, 22					! comapres day to 22
	be	nd						! branches to nd on day = 22
	cmp	%l6, 3					! compares day to 3
	be	rd						! branches to rd on day = 3
	cmp	%l6, 23					! compares day to 23
	be	rd						! branches to rd on day = 23
	nop							! nop
	ba	th						! always branches to th
	nop							! nop
st:	set	fmtst, %o0					! block for days suffixed with st, sets %o0 as fmtst
	ba	last						! always branches to last
	nop							! nop
nd:	set	fmtnd, %o0					! block for days suffixed with nd, sets %o0 as fmtnd
	ba	last						! branches always to last
	nop							! nop
rd:	set	fmtrd, %o0					! block for days suffixed with rd, sets %o0 as fmtrd
	ba	last						! branches always to last
	nop							! nop
th:	set	fmtth, %o0					! block for days suffixed with th, sets %o0 as fmtth
last:	mov	%l7, %o1					! block for printing out the date, moves %l7 to %o1
	mov	%l6, %o2					! moves %l6 to %o2
	call	printf 					! calls printf
	mov	%l5, %o3					! moves %l5 to %o3
end:	mov	1, %g1					! block for closing the program, moves 1 to %g1
	ta	0						! traps 0
here:	set	wrong, %o0					! block for wrong number of arguments, sets %o0 as wrong
	call	printf 					! calls printf
	nop							! nop
	ba	end						! branches always to end
	nop							! nop
there:set	wrong1, %o0					! block for inputs out of range, sets %o0 as wrong1
	call	printf 					! calls printf
	nop							! nop
	ba	end						! branches always to end
	nop							! end