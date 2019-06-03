;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : TGA_TEST.ASM
;쿌uthor       : Cyberfish of The Lost Souls
;쿗ast update  : 05.08.1997
;쿌ction       : Tests the TGA-decoding routine
;�
;쿙otes: No bank-switching or linear framing. Just copying 64k to 0A0000h
;쿔n other words, you'll just see a part of your picture.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model flat
.stack 100h
.code

include w:\general\var.inc
include w:\general\main.inc
include w:\general\general.inc
include w:\general\mem.inc
include w:\gfx\tga.inc


TGAFile         DB 'test.tga', 0

Main          PROC
                call   D32_Setup
                jc     Error

                mov    eax, -1024*1024
                call   MEM_Init
                jc     Error

                mov    edx, OFFSET TGAFile
                xor    esi, esi
                xor    ecx, ecx
                call   TGA_Load

                mov    ah, 4Fh
                mov    al, 02
                mov    bx, 10Dh                 ;320x200x32k
                int    10h

                mov    esi, edi
                mov    edi, 0A0000h
                mov    ecx, 320*200
                rep    movsw

                mov    ah, 01h
                int    21h

                mov    ah, 0                    ;Initialize screenmode
                mov    al, 03h                  ;320x200x256
                int    10h                      ;VGA-services

Error:
                mov    ah, 09h
                mov    edx, SYS_QuitMsg
                int    21h

                mov    ax, 4C00h
                int    21h
Main          ENDP


END Main