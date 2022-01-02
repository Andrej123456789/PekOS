.message:
    
    osvermsg db '  PekOS V4.3',13,10,'|------------|',13,10,'',0
    warn db ' DONT PRESS NUMBERS ON NUMPAD | Check Instructions for more info',13,10,'|-----------------------------------------------------------------|',13,10,'',0
    text_string db '<Functions>',13,10,'',13,10,'(1) Reboot',13,10,'',13,10,'(2) Shutdown',13,10,'',13,10,'(3) About & Credits',13,10,'',13,10,'(4) pTerm',13,10,'',13,10,'(5) System Halt',13,10,'',13,10,'(6) Instructions',13,10,'|> ',0
    about_string db '|<About & Credits>|',13,10,'(PDM) 2021 No rights reserved',13,10,'',13,10,'PekOS is a small OS made in assembly.I develop PekOS in my free time so updates might be slow',13,10,'  ',13,10,'Thank you OSdev wiki for terminal code',13,10,'  ',13,10,'Main Programer: StjepanBM1',13,10,'  ',13,10,'Graphichs: no one',13,10,'  ',13,10,'Press any key to exit this page',0
    pterm_str db '',10,13,'Welcome to pTerm V1.8.0',13,10,'Type help to see all commands',10,13,'Type exit to exit',13,10,'',0
    inst db '|<Instructions>|',13,10,'To open apps simply press the keyboard number equivilent or the OS will bug out',10,13,'',13,10,'If the OS does bug out just restart the System',13,10,'Press any key to exit this page',0
    help db '|<Help & Contacts>|',13,10,'If you are expiriencing an issue or a bug contact me on github',10,13,'Github Username > StjepanBM1',13,10,'Github page> https://github.com/StjepanBM1/PekOS',13,10,'Press any key to exit this page',0
    osinf db 'OS Ver >> PekOS V4.4 `Blue Rock` ',13,10,'',10,13,'Press any key to exit this page',0