
    scr_test:

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x0F
		int		0x10
        mov si, blck
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x1F
		int		0x10
        mov si, blue
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x2F
		int		0x10
        mov si, gren
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x3F
		int		0x10
        mov si, cyan
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x4F
		int		0x10
        mov si, red
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x5F
		int		0x10
        mov si, mage
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x6F
		int		0x10
        mov si, brown
        call    print

        mov ah, 0x00
        int     0x16

        call clear
        mov	ah,	09h
		mov	cx,	2000d
		mov	al,	20h
		mov	bl,	0x7F
		int		0x10
        mov si, l_gray
        call    print

        mov ah, 0x00
        int     0x16

        call    clear

        mov si, done
        call    print

        mov ah, 0x00
        int     0x16

        jmp _kmain

        blck:
            db  "Is your screen displaying the color BLACK? | Press any key to continue...", 0

        blue:
            db  "Is your screen displaying the color BLUE? | Press any key to continue...", 0

        gren:
            db  "Is your screen displaying the color GREEN? |Press any key to continue...", 0

        cyan:
            db  "Is your screen displaying the color CYAN? | Press any key to continue...", 0

        red:
            db  "Is your screen displaying the color RED? | Press any key to continue...", 0

        mage:
            db  "Is your screen displaying the color MAGENTA? | Press any key to continue...", 0

        brown:
            db  "Is your screen displaying the color BROWN? | Press any key to continue...", 0

        l_gray:
            db  "Is your screen displaying the color (LIGHT) GRAY? | Press any key to continue...", 0

        done:
            db  "Screen color test has finished.", 13,10, \
            "If you couldn't see any of the colors shown try configuring your monitor to ", 13,10, "a different setting.", 13, 10, \
            "If that fails go to https://github.com/StjepanBM1/PekOS and submit an issue.", 13,10, \
            "Press any key to exit...", 0
