
    ;
    ; @AUTHOR :: StjepanBM1
    ; @ABOUT  :: Basic Painting software
    ;            for PekOS GV
    ;

    %include "src/krnl/inc/paint.inc"

    paint_m:

        ; SETUP THE
        ; DRAWING SURFACE
        ; 79x25
        call canvas

        ; Top-Bar
        call paint_tbar

        ; DRAWING
        ; CURSOR
        ; (BLACK)
        call paint_cursor

        ret
