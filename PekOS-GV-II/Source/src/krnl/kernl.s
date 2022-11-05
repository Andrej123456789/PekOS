
	[BITS		16]

	%DEFINE	SYS_VER	'GV-II System'
	%DEFINE RELEASE 'Second Ed. '
	%DEFINE BUILD	'11/5/2022-PM'
	
	JMP	_kmain

	_kmain:

		cli
		mov	ax,	0
		mov	ss,	ax
		mov	sp,	0FFFFh
		sti
	
		cld

		mov	ax,	2000h
		mov	ds,	ax
		mov	es,	ax
		mov	fs,	ax
		mov	gs,	ax

		mov	ax,	1003h
		mov	bx,	0
		int		0x10

		call	clear
		call 	screen
		call	_string
		call	mouse

		jmp	$

	;	INCLUDES
	%include "src/krnl/incs/screen.s"
	%include "src/krnl/txt/puts.s"
	%include "src/krnl/incs/mouse.s"

		;	APPS
	%include "src/krnl/apps/pterm.s"
	%include "src/krnl/apps/edline.s"
	%include "src/krnl/apps/paint.s"
	%include "src/krnl/incs/settings.s"
