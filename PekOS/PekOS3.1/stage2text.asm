.message:
    
    osvermsg db '=PekOS V3.1=',13,10,'',0
    text_string db '<Functions>',13,10,'1) Reboot',13,10,'2) Shutdown',13,10,'3) About & Credits',13,10,'4) pTerm',13,10,'5) System Halt',13,10,'6) Other',13,10,'7) Instructions',13,10,'8) Contact me / Help',0
    about_string db '<About & Credits>',13,10,'PekOS is a small OS made in assembly.I develop PekOS in my free time so updates might be slow',13,10,'  ',13,10,'There is no copyright you are free to do what you want with this OS',13,10,'  ',13,10,'Main Programer: StjepanBM1',13,10,'  ',13,10,'Graphichs: Currently no one',13,10,'  ',13,10,'Press any key to exit this page',0
    pterm_str db '<pTerm>',10,13,'Welcome to pTerm V1.7.6 | still in development |',13,10,'You cant exit this app cuz i fucked up the code',10,13,'',0
    cr_str db '<List of other applications> ',13,10,'1) Notepad',13,10,'2) Calculator',13,10,'3) Settings',' ',13,10,'Press any key to exit this page',0
    inst db '<Instructions>',13,10,' To open apps simply press the keyboard number equivilent or the OS will bug out',10,13,' ',13,10,'Press any key to exit this page',0
    help db '<Help & Contacts>',13,10,'If you are expiriencing an issue or a bug contact me on github',10,13,'Github Username > StjepanBM1',13,10,'Press any key to exit this page',0
