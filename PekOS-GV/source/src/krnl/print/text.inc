    
    ;
    ; TXT STRING (USED BY WRITEF)
    ;

    ACT_BAR:
        db nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl,nl, \
            '       ',218,196,196,196,196,196,196,196,196,180,'   PekOS GV INFO   ',195,196,196,196,196,196,196,196,196,194,196,196,196,196,196,196,196,196,196,196,196,196,196,194,196,196,196,196,196,196,196,196,196,196,196,196,191,nl, \
            '       ',179,' PekOS GV is a version of PekOS built',179,'   UP  : ',24,'   ',179,'[ PekOS GV ]',179,nl,\
            '       ',179,' using 3.x Bootloader, but featuring,',179,' DOWN  : ',25,'   ',179,' [1] Reboot ',179,nl,\
            '       ',179,' a custom kernel & user enviroment.  ',179,' LEFT  : ',27,'   ',179,' [2] Exit   ',179,nl,\
            '       ',179,' Move the cursor using arrow keys &  ',179,' RIGTH : ',26,'   ',179,' [3] pTerm  ',179,nl,\
            '       ',179,' use spacebar to click on stuff.     ',179,'CLICK : SPACE',     179,' [4] Paint  ',179,nl,\
            '       ',192,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,\
            196,196,196,196,196,196,196,196,196,196,196,196,193,196,196,196,196,196,196,196,196,196,196,196,196,196,193,196,196,196,196,196,196,196,196,196,196,196,196,217,nl, 0

    BLANK:    db 0x0D, 0x0A, 0


    ;
    ;   STRINGS USED IN PAINT 1.0
    ;
    p_tbar_txt:
        db ' ESC = Quit ',179,' I = info',nl,0

    p_info_txt:
        db nl, \
            '       ',201,205,205,205,205,205,205,240,'  Paint V1',179,' Info  ',240,205,205,205,205,205,205,187,nl, \
            '       ',186,'                                ',186,nl, \
            '       ',186,'              ABOUT             ',186,nl, \
            '       ',186,'  Paint 1.0 is a painting pro-  ',186,nl, \
            '       ',186,'  gram included in PekOS GV.It  ',186,nl, \
            '       ',186,'  is only in black & white for  ',186,nl, \
            '       ',186,'  now. It also features 4 dif-  ',186,nl, \
            '       ',186,'  ferent transparency levels.   ',186,nl, \
            '       ',186,'                                ',186,nl, \
            '       ',186,'              HELP              ',186,nl, \
            '       ',186,'        Q - Place a block       ',186,nl, \
            '       ',186,'        E - Erase a block       ',186,nl, \
            '       ',186,'       0',26,'3 - Change shade       ',186,nl, \
            '       ',186,'            I - Info            ',186,nl, \
            '       ',200,' Press any key to exit this msg ',188,nl, \
            '       ]', 0


    ;
    ; STRINGS USED IN NOTES 1.0
    ;

    n_tbar_txt:
        db ' ESC = Quit ',179,nl,0

    ;
    ; pTerm(inal)
    ;
    list_str:
        db nl, \
            '       ',' Item List ','   ',' Base App ',nl,  \
            '       ','-----------','---','-----------',nl, \
            '       ',' pTerm 2.4 ','   ','    YES    ',nl, \
            '       ',' Paint 1.0 ','   ','    YES    ',nl, \
            '       ',' Notes 1.0 ','   ','    YES    ',nl, \
            '       ',' GV-Krnl I ','   ','     *     ',nl, \
            '       ','  UsrEnv   ','   ','     *     ',nl, \
            '       ',' BootL-3.x ','   ','     *     ',nl, \
            0
         
    help_str:
        db nl, \
            '       ',' HELP :   ',nl, \
            '       ','========= ',nl, \
            '       ',' ver    - displays pTerm version',nl, \
            '       ',' clr    - clears the screen',nl, \
            '       ',' inf    - displays OS Info',nl, \
            '       ',' help   - displays help screen',nl, \
            '       ',' exit   - returns to desktop',nl, \
            '       ',' list   - lists all items',nl, \
            '       ',' paint  - opens Paint',nl, \
            '       ',' notes  - opens Notes',nl, \
            '       ',' reboot - reboots the sys',nl, \
            '       ',' shutdw - shutsdown the sys',nl, \
            0

    help_infa:
        db nl, \
            '       ','== OS INFO ==',nl, \
            '       ','  VER  : GV  ',nl, \
            '       ','  KRNL : GV-SYS-1',nl, \
            '       ','  USRE : GV-ENV-1',nl, \
            '       ','  BOTL : 3.X',nl, \
            0

    scr_msg:
        db \
            '                       ',15,15,15,' pTerm 2.4.0 | PekOS GV ',15,15,15,nl, 0
