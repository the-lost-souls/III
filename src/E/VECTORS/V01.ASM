;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : V01.ASM
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 12.12.1998
;쿌ction       : Object routine for CVEC_01 (objtype 3) (perspective mapping)
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model flat, C
.code

public V01

include w:\general\var.inc
include w:\vectors\v01macro.inc

;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : V01
;쿔D           : V01
;쿟ype         : Procedure
;쿗ast update  : 10.02.1998
;쿌ction       : Object routine for CVEC_01 (perspective mapping)
;쿚ptimized    : Nope
;�
;쿔nput variables : SYS_OOffs = Offset object
;�
;쿝egisters changed : All!
;�
;쿙otes : FPU stack must be empty.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

XSin            DD 0.0
XCos            DD 0.0
YSin            DD 0.0
YCos            DD 0.0
ZSin            DD 0.0
ZCos            DD 0.0

M00             DD 0.0
M10             DD 0.0
M20             DD 0.0
M01             DD 0.0
M11             DD 0.0
M21             DD 0.0
M02             DD 0.0
M12             DD 0.0
M22             DD 0.0

Zero            DD 0.0
One		DD 1.0
RFlag           DD 0
PFlag           DD 0

V01           PROC
                mov    esi, SYS_OOffs
                inc    RFlag

                mov    ecx, [esi].H_Faces
                mov    edi, [esi].H_FaceOffs
VecLoop:
                mov    ebp, [edi].F_V0Offs
                mov    eax, [ebp].V_RFlag
                cmp    eax, RFlag
                je     V0Done

                CalculateVert                   ;[ebp] = vertice structure
V0Done:
;---- Rotating normalvector X
                fld    [edi].F_NVX
                fmul   [esi].H_M00
                fld    [edi].F_NVY
                fmul   [esi].H_M01
                fld    [edi].F_NVZ
                fmul   [esi].H_M02
                faddp  st(1), st
                faddp  st(1), st
                fmul   [ebp].V_XRotated         ;Mul by "cameravector"

;---- Rotating normalvector Y
                fld    [edi].F_NVX
                fmul   [esi].H_M10
                fld    [edi].F_NVY
                fmul   [esi].H_M11
                fld    [edi].F_NVZ
                fmul   [esi].H_M12
                faddp  st(1), st
                faddp  st(1), st
                fmul   [ebp].V_YRotated         ;Mul by "cameravector"


;---- Rotating normalvector Z
                fld    [edi].F_NVX
                fmul   [esi].H_M20
                fld    [edi].F_NVY
                fmul   [esi].H_M21
                fld    [edi].F_NVZ
                fmul   [esi].H_M22
                faddp  st(1), st
                faddp  st(1), st
                fmul   [ebp].V_ZRotated         ;Mul by "cameravector"

;---- Calculating dot-product
                faddp  st(1), st
                faddp  st(1), st                ;stack = NormalV dot CameraV


                fcomp  Zero                     ;Face visible?
                fstsw  ax
                sahf
                jae    DumpFace                 ;jae coz the sign is wrong...

                mov    eax, [ebp].V_ZFlag       ;Update poly's Z-flag
                mov    [edi].F_ZFlag, eax       ;/
                mov    eax, [ebp].V_PFlag       ;Update poly's P-flag
                mov    PFlag, eax               ;/

                mov    ebp, [edi].F_V1Offs
                mov    eax, [ebp].V_RFlag
                cmp    eax, RFlag
                je     V1Done

                CalculateVert
V1Done:
                mov    eax, [ebp].V_ZFlag       ;Update poly's Z-flag
                add    [edi].F_ZFlag, eax       ;/
                mov    eax, [ebp].V_PFlag       ;Update poly's P-flag
                and    PFlag, eax               ;/

                mov    ebp, [edi].F_V2Offs
                mov    eax, [ebp].V_RFlag
                cmp    eax, RFlag
                je     V2Done

                CalculateVert

V2Done:
                mov    eax, [ebp].V_ZFlag       ;Update poly's Z-flag
                add    [edi].F_ZFlag, eax       ;/
                cmp    [edi].F_ZFlag, 3         ;Entire polygon behind?
                je     DumpFace                 ;yes...

                mov    eax, [ebp].V_PFlag       ;Update poly's P-flag
                and    PFlag, eax               ;/
                jnz    DumpFace                 ;/entire face outside screen

                fld    [ebp].V_InvZ             ;Converting 1/Z into fixed
		fadd	One
                fmul   SYS_ZBufFP               ;0.32 fixed point
                fistp  [edi].F_SortValue        ;Store it as integer


;---- Insert polygon in part-lists...
                mov    edx, SRT_End1
                mov    eax, [edi].F_SortValue
                and    eax, 255                 ;Mask out first byte (radix)
                mov    ebx, 255                 ;sort highest first
                sub    ebx, eax                 ;/
                mov    eax, [edx + ebx*4]       ;Load end of list
                mov    [eax], edi               ;Update end-poly
                mov    [edx + ebx*4], edi       ;Update end-list

		mov	[edi].F_Priority, 250
		mov	[edi].F_Type, 1

DumpFace:


                add    edi, F_Size              ;Next face
                dec    ecx
                jnz    VecLoop

                ret
V01           ENDP

END
