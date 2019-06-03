;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER INCLUDEFILE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : LIGHTS.INC
;³Author       : Cyberfish/TLS & Toe/DU
;³Last update  : 09.03.1998
;³Action       : External definitions for light-routines/variables.
;³
;³Notes : DO NOT USE ESI, EDI, ECX INSIDE LIGHT-ROUTINES!!!!!!!!
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

extrn LIT_ROut          :DWORD                  ;/
extrn LIT_GOut          :DWORD                  ;/
extrn LIT_BOut          :DWORD                  ;/

extrn LIT_NVX		:DWORD
extrn LIT_NVY		:DWORD
extrn LIT_NVZ		:DWORD

extrn LIT_Init          :DWORD
extrn LIT_InitP         :PROC
extrn L01               :PROC
extrn L02 		:PROC
extrn L03 		:PROC




;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER MACRO ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : LIT_Run
;³ID           : LIT
;³Type         : Macro
;³Last update  : 27.12.97
;³Action       : Macro to run all lights for an object
;³Optimized    : Nope
;³
;³Input variables : All variables above...
;³
;³Registers changed : All!
;³
;³Notes : DO NOT USE ESI, EDI, ECX INSIDE LIGHT-ROUTINES!!!!!!
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

LIT_Run       MACRO
LOCAL LIT_RunLoop, LIT_RunDump, LIT_RunDone

                mov    ebp, SYS_LListOffs       ;Offset to light-list

LIT_RunLoop:
                cmp    DWORD PTR [ebp], -1      ;End of list?
                je     LIT_RunDone

                push   ebp
                mov    ebp, [ebp]
                or     ebp, ebp                 ;No light
                jz     LIT_RunDump
                mov    SYS_LOffs, ebp           ;Save object's offset

                mov    ebx, [ebp]               ;Load light-type
                mov    eax, SYS_LRoutines[ebx*4];Load the light's routine

                or     eax, eax
                jz     LIT_RunDump

                call   eax                      ;Run light
LIT_RunDump:
                pop    ebp
                add    ebp, 4                   ;Next light...
                jmp    LIT_RunLoop

LIT_RunDone:
              ENDM
