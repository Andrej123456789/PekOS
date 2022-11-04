
	clear:
		pusha
		
		mov	ah,	0x00
		mov	al,	0x03
		int		0x10

		popa
		ret

	screen:
		xor	al,	al
		
		mov	ah,	09h
		mov	cx,	2000d
		mov	al,	32d
		mov	bl,	0x0C
		int		0x10
		
		;
		;	BACKG.
		;
		mov	ah,	09h
		mov	cx,	1957d
		mov	al,	176d
		mov	bl,	0x4F
		int		0x10

		;
		;	TASKB. 1
		;
		mov	ah,	09h
		mov	cx,	20d
		mov	al,	20h
		mov	bl,	0x7F
		int		0x10

		;
		;	TASKB.	2
		;
		;mov	ah,	09h
		;mov	cx,	13d
		;mov	al,	20h
		;mov	bl,	0x4F
		;int		0x10
		
		mov ah, 0x02
		mov bh, 0
		mov dh, 24
		mov dl, 37
		int 	0x10
		
		mov	si,	bott
		call	print
		
		mov ah, 0x02
		mov bh, 0
		mov dh, 18
		mov dl, 0
		int 	0x10
		
		mov	si,	info
		call	print
		
		mov ah, 0x02
		mov bh, 0
		mov dh, 0
		mov dl, 0
		int 	0x10
		
		ret

	_string:
		;	String
		mov	si,	start
		mov	bx,	0
		mov	cx,	0
		mov	dx,	0
		call		print

		ret

	taskb:
		call	clear
		call	screen
		call	_string
	
		cli
		xor	al,	al
	
		mov	si,	tasks	
		call		print

		mov	ah,	0x00
		int		0x16
		
		jmp	_kmain

		cmp	al,	0x31
		je		.reboot

		cmp	al,	0x32
		je		.shutd

		cmp	al,	0x33
		je		.pterm

		cmp	al,	0x34
		je		.paint2

		cmp	al,	0x35
		je		.edline

		cmp	al,	0x36
		je		.scrt

		ret

	.scrt:
		call	scr_test
		ret

	.reboot:
		mov	ax,	0x0000
		jmp	ax

	.shutd:
		mov	ax,	0x1000
		mov	ax,	ss
		mov	sp,	0xf000
		mov	ax,	0x5307
		mov	bx,	0x0001
		mov	cx,	0x0003
		int		0x15
		ret

	.pterm:
		call	_pterm_M
		ret

	.paint2:
		cli
		call	_paint
		ret

	.edline:
		cli
		call	_edline
		ret

