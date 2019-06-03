;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : GIF_TEST.ASM
;쿌uthor       : Cyberfish of The Lost Souls
;쿗ast update  : 29.05.1997
;쿌ction       : Tests the GIF87/89-decoding routine
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

.386
.Model  FLAT
.Stack  100h
.code

EXTRN         debug:PROC
EXTRN         debugrun:PROC

include       w:\general\var.inc
include       w:\general\general.inc
include       w:\general\main.inc
include       w:\gfx\gif.inc
include       w:\general\mem.inc

InitStr       DB '컴컴컴컴컴컴� GIF decoder 컴컴컴컴컴컴', 0Dh, 0Ah
              DB 'Loading and initializing...', 0Dh, 0Ah, 0Dh, 0Ah, '$'
File          DB 'Object file : ','$'
GIFFile       DB 'test.gif', 0, 0Dh, 0Ah, '$'

Picture       DD ?



Main          PROC
                PS_PString(InitStr)
                call   D32_Setup
                jc     Failed

                mov    eax, -1024*1024          ;Reserve 1 meg
                call   MEM_Init
                jc     Failed

                PS_PString(File)
                PS_PString(GIFFile)

                mov    ah, 01                   ;Wait for keypress
                int    21h

                mov    edx, OFFSET GIFFile      ;Setting up for decoder
                xor    ecx, ecx                 ;Read entire file
                xor    esi, esi                 ;...from start
                call   GIF_Load
                jc     Failed
                mov    Picture, edi

                mov    ah, 0                    ;Initialize screenmode
                mov    al, 13h                  ;320x200x256
                int    10h                      ;VGA-services

                mov    dx,3C8h                  ;Port to send out startingcolor
                mov    al, 0                    ;Starting color = 0
                out    dx, al                   ;Send it!
                inc    dx                       ;Next port = RGB values
                mov    ecx, 768
                mov    esi, OFFSET GIF_Palette  ;Point to palette
                rep    outsb                    ;Set palette

                cld
                mov    esi, Picture
                mov    edi, 0A0000h             ;VGA
                mov    ecx, 16000
                rep    movsd                    ;View picture

                mov    ah, 01                   ;Wait for keypress
                int    21h

                mov    ah, 0                    ;Initialize screenmode
                mov    al, 03h                  ;320x200x256
                int    10h                      ;VGA-services


Failed:
                mov    edx, SYS_QuitMsg
                mov    ax, 0900h
                int    21h
                mov    ax, 4C00h
                int    21h
Main          ENDP

END           Main




