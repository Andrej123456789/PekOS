
	%DEFINE	nl	0x0D, 0x0A


	print:
		lodsb

		or	al,	al
		jz	.done
		
		mov	ah,	0x0E
		mov	bh,	0
		int		0x10

		jmp	print

	.done:
		ret

	;	TEXT
	start:	; 1 LINE
		db	'  |',4,'| PekOS GV-II  ',16, nl, 0

	tasks:	;	10 LINES
		db	'  (1) Reboot    ', nl, \
			'  (2) Shutdown  ', nl, \
			' -------------- ', nl, \
			'  (3) pTerm 2.6 ', nl, \
			'  (4) Paint 2.0 ', nl, \
			'  (5) Ed-Line   ', nl, \
			' -------------- ', nl, \
			'  (6) Scr. Test ', nl, \
			' -------------- ', nl, \
		 	' [X] ', \
			0
	
	bott:	; 1 LINE
		db	" ",SYS_VER," | ", RELEASE," | ", BUILD, 0

	info:	; 7 LINES
		db 	\
			218,196,196,196,196,196,196,196,196,196,196,196,196,196,191,nl, \
			179,'   UP  : ',24,'   ',179, nl, \
			179,' DOWN  : ',25,'   ',179, nl, \
			179,' RIGHT : ',26,'   ',179, nl, \
			179,' LEFT  : ',27,'   ',179, nl, \
			179,' MENU  :SPACE',179,nl, \
			192,196,196,196,196,196,196,196,196,196,196,196,196,196,217, \
			0
