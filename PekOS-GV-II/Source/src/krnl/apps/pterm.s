
	%include	"src/krnl/apps/pterm.lib"

	_pterm_M:
			call	_p_scrn

			_cmd:
				call	_scroll

				mov	si,	buff

				mov	si,	_p_prmpt
				call	print

				mov	di,	buff
				call	inpt

				mov	si,	buff
				cmp	byte	[si], 0
				je	_cmd

				mov	si,	buff
				mov	di,	empt
				call	strcmp
				jc		.empty

				mov	si,	buff
				mov	di,	ver
				call	strcmp
				jc		.ver

				mov	si,	buff
				mov	di,	clr
				call	strcmp
				jc		.clr

				mov	si,	buff
				mov	di,	hlp
				call	strcmp
				jc		.help


				mov	si,	buff
				mov	di,	ext
				call	strcmp
				jc		.exit

				mov	si,	buff
				mov	di,	pnt
				call	strcmp
				jc		.paint
				
				mov	si,	buff
				mov	di,	line
				call	strcmp
				jc		.edl
				
				mov	si,	buff
				mov	di,	rbt
				call	strcmp
				jc		.rebt
				
				mov	si,	buff
				mov	di,	sht
				call	strcmp
				jc		.shutd

				mov	si,	err
				call	print
				jmp		_cmd

			jmp		_cmd

			.empty:
				call	_scroll
				jmp		_cmd

			.ver:
				call	_scroll
				mov	si,	vers
				call	print

				jmp		_cmd

			.clr:
				call	clear
				call	_p_scrn

				jmp		_cmd

			.help:
				mov	si,	helps
				call	print

				jmp		_cmd

			.exit:
				jmp		_kmain

			.paint
				call	_paint
				ret
				
			.edl:
				call	_edline
				ret

			.rebt:
				jmp		_kmain
				ret
				
			.shutd:
				mov	ax,	0x1000
				mov	ax,	ss
				mov	sp,	0xf000
				mov	ax,	0x5307
				mov	bx,	0x0001
				mov	cx,	0x0003
				int		0x15
				ret
