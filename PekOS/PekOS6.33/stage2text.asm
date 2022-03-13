.message:
    
    osvermsg db '  PekOS 6.33 ',13,10,'',13,10,'',0
    text_string db '  <Functions>',13,10,'',13,10,'  (1) Shutdown',13,10,'',13,10,'  (2) About',13,10,'',13,10,'  (3) pTerminal',13,10,'',13,10,'  (4) PekOS version information',13,10,'',13,10,'  |> ',0
    about_string db '|<About & Credits>|',13,10,'(PDM) 2022 No rights reserved',13,10,'',13,10,'PekOS is a small OS made in assembly.I develop PekOS in my free time so updates might be slow',13,10,'',13,10,'Thank you OSdev wiki for terminal code',13,10,'',13,10,'Thank you Stack Overflow for helping me fix some errors',13,10,'',13,10,'Main Programer: StjepanBM1',13,10,'  ',13,10,'Graphichs: no one',13,10,'  ',13,10,'Press any key to exit...',0
    pterm_str db '',10,13,'Welcome to pTerm V2.2.1',13,10,'Type "help" to see all commands',10,13,'Type "exit" to exit',13,10,'',0
    inst db '|<Info & Instructions>|',13,10,'',13,10,'Instructions','',13,10,' To open apps simply press the keyboard number equivilent or the OS will bug out',10,13,'',13,10,'If the OS does bug out just restart the System',13,10,'',13,10,'General OS info:',13,10,'',13,10,'OS Ver: PekOS 5.1',13,10,'This is last version of PekOS 100% made in Assembly from this version onward most of the OS will be made in C',13,10,'',13,10,'Press any key to exit...',13,10,'',0
    osinf db 'OS Ver >> PekOS V6.33 ~Black Dusk~',13,10,'',10,13,' ',0
    Firsttimeusertxt db 'OS Ver >> PekOS V6.33 ~Black Dusk~ PRE-RELEASE BUILD 133 ',13,10,'Welcome to PekOS 16-bit OS made in Assembly.',13,10,'',13,10,'PekOS Version 6.33 "Black Dusk is ment to be 16-bit and ',13,10, 'windowless version of PekOS ',13,10,'',10,13,'Press any key to exit this page...',0
    pterinf db 'OS Ver: PekOS 6.33',13,10,'',13,10,'',0