
	%include	"src/krnl/apps/paint.lib"

	_paint:
			call	clear
			call	_paint_scr
			
		_paintm:
		
			call	_def_curs
		
			ret
