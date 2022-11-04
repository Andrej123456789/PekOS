
	%DEFINE	X	0
	%DEFINE	Y	0

	mouse:
		mov	ah,	01h
		mov	cx,	04h
		int	0x10

		mov	dl,	0d
		mov	dh,	0d

		jmp	_mouse

		_mouse:
			mov	ah,	02h
			mov	dl,	bl
			mov	dh,	cl
			int	0x10

			mov	ah,	00h
			int		16h

			cmp	ah,	0x48
			je		_up
			
			cmp	ah,	0x50
			je		_dw

			cmp	ah,	0x4B
			je		_lt

			cmp	ah,	0x4D
			je		_rt

			cmp	al,	0x0D		; CLICK
			je		_cl		; DOES NOTHING

			;	TASKBAR
			;	|> SPACE BAR
			cmp	al,	0x20
			je		_tsk

			_up:
				cmp	cl,	0h
				je	_mouse

				sub	cl,	1
				jmp	_mouse
			
			_dw:
				cmp	cl,	24d
				je	_mouse

				add	cl,	1
				jmp	_mouse

			
			_lt:
				cmp	bl,	0h
				je	_mouse

				sub	bl,	1
				jmp	_mouse
			
			_rt:
				cmp	bl,	79d
				je	_mouse

				add	bl,	1
				jmp	_mouse
			
			_cl:
				jmp	_mouse

			_tsk:
				call	taskb
				jmp	_mouse

			msg:
				db	"Press any key...", 13, 0

