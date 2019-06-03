;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : PCX.ASM
;쿌uthor       : Cyberfish of The Lost Souls
;쿗ast update  : 30.08.1997
;쿌ction       : Tests the PCX-decoding routine
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model  FLAT
.Stack  100h
.code

include       w:\general\var.inc
include       w:\general\main.inc
include       w:\general\general.inc
include       w:\gfx\pcx.inc
include       w:\general\mem.inc

InitStr       DB '컴컴컴컴컴컴� PCX decoder 컴컴컴컴컴컴', 0Dh, 0Ah
              DB 'Loading and initializing...', 0Dh, 0Ah, 0Dh, 0Ah, '$'
File          DB 'Object file : ','$'
PCXFile       DB 'test.pcx', 0, 0Dh, 0Ah, '$'


Main          PROC
                PS_PString(InitStr)

                call   D32_Setup
                jc     Failed

                mov    eax, 0
                call   MEM_Init

                PS_PString(File)
                PS_PString(PCXFile)

                mov    ah, 01                   ;Wait for keypress
                int    21h

                mov    edx, OFFSET PCXFile      ;Setting up for decoder
                xor    esi, esi                 ;Read from end of file
                xor    ecx, ecx                 ;Read to end of file
                call   PCX_Load
                jc     Failed

                mov    ah, 0                    ;Initialize screenmode
                mov    al, 13h                  ;320x200x256
                int    10h                      ;VGA-services

                mov    dx,3C8h                  ;Port to send out startingcolor
                mov    al, 0                    ;Starting color = 0
                out    dx, al                   ;Send it!
                inc    dx                       ;Next port = RGB values
                mov    ecx, 768
                mov    esi, OFFSET PCX_Palette  ;Point to palette
                rep    outsb                    ;Set palette

                cld
                mov    esi, edi
                mov    edi, 0A0000h
                mov    ecx, 16000
                rep    movsd                    ;View picture

                mov    ah, 01                   ;Wait for keypress
                int    21h

                mov    ah, 0                    ;Initialize screenmode
                mov    al, 3h                  ;320x200x256
                int    10h                      ;VGA-services

Failed:
                mov    edx, SYS_QuitMsg
                mov    ax, 0900h
                int    21h

                mov    ax, 4C00h
                int    21h
Main          ENDP

END           Main




