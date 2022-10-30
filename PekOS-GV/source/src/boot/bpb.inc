
    ;
    ; OEM PARAMETER BLOCK
    ;
    jmp boot_start
    
    times 3-($-$$) db 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    bpb_disk_info:
        OEMname                 db "PEKOS GV"
        
        bytesPerSector:         dw    512
        sectPerCluster:         db    1
        reservedSectors:        dw    1
        numFAT:                 db    2
        numRootDirEntries:      dw    224
        numSectors:             dw    2880
        mediaType:              db    0xf0
        numFATsectors:          dw    9
        sectorsPerTrack:        dw    18
        numHeads:               dw    2
        numHiddenSectors:       dd    0
        numSectorsHuge:         dd    0
        driveNum:               db    0
        reserved:               db    0
        signature:              db    0x29
        volumeID:               dd    0x2d7e5a1a
        bsVolumeLabel: 	        db    "PEKOS-GV   "
        bsFileSystem: 	        db    "FAT12   "
