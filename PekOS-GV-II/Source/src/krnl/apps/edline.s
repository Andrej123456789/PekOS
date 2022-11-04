
	%include	"src/krnl/apps/edline.lib"

	_edline:
		call	_edl_scr
		
		_edline_m:
				call	_ed_inpt
				
			jmp	_edline_m
			
		;ret
