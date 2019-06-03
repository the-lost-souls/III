;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER INCLUDEFILE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : GENERAL.INC
;³Author       : Cyberfish/TLS & Toe/DU
;³Last update  : 28.09.97
;³Action       : Div general procedures.
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

extrn  D32_Setup	    :PROC        ;Sets up several "SYS"-variables
extrn  PD_PDec		    :PROC        ;Prints an usigned decimal
extrn  PC_PCount            :PROC        ;Prints a string ecx bytes long
extrn  PZ_PZero             :PROC        ;Prints a 0-terminated string
extrn  PH_PHex              :PROC        ;Prints a number in hex
extrn  CN_8_16              :PROC        ;Converts an 8-bit picture to 16-bit
extrn  CN_8_15              :PROC        ;Converts an 8-bit picture to 15-bit
extrn  DOS_Int              :PROC        ;Simulates a realmode int-call
extrn  DOS_es               :WORD        ;/es and ds must be passed here
extrn  DOS_ds		    :WORD        ;/
extrn  M44_Mul              :PROC        ;Multiplies to 4x4 matrices




;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : PS_PString
;³ID           : PS
;³Type         : macro
;³Last update  : 20.05.1997
;³Action       : Prints a string, terminated by '$'
;³Optimized    : -
;³
;³Input variables : String
;³
;³Registers changed : eax, edx
;³
;³Notes : Resolution has to be given in _BYTES_, not pixels.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

PS_PString    MACRO String
                mov    ax, 0900h
                mov    edx, OFFSET String
                int    21h
              ENDM
