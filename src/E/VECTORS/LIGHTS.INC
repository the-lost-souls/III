;�������������������������� ASSEMBLER INCLUDEFILE ���������������������������
;�Name         : LIGHTS.INC
;�Author       : Cyberfish/TLS & Toe/DU
;�Last update  : 09.03.1998
;�Action       : External definitions for light-routines/variables.
;�
;�Notes : DO NOT USE ESI, EDI, ECX INSIDE LIGHT-ROUTINES!!!!!!!!
;����������������������������������������������������������������������������

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




;�������������������������� ASSEMBLER MACRO ���������������������������������
;�Name         : LIT_Run
;�ID           : LIT
;�Type         : Macro
;�Last update  : 27.12.97
;�Action       : Macro to run all lights for an object
;�Optimized    : Nope
;�
;�Input variables : All variables above...
;�
;�Registers changed : All!
;�
;�Notes : DO NOT USE ESI, EDI, ECX INSIDE LIGHT-ROUTINES!!!!!!
;����������������������������������������������������������������������������

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