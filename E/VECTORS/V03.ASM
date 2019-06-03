;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : V03.ASM
;³Author       : Cyberfish/TLS & Toe/DU
;³Last update  : 12.12.1998
;³Action       : Object routine for CVEC_03 (objtype 5) (flatshading)
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

.386
.Model flat, C
.code

public V03

include w:\general\var.inc
include w:\vectors\V03macro.inc
include w:\render\render.inc
include w:\vectors\lights.inc


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : V03
;³ID           : V03
;³Type         : Procedure
;³Last update  : 12.12.1998
;³Action       : Object routine for CVEC_03 (perspective mapping)
;³Optimized    : Nope
;³
;³Input variables : SYS_OOffs = Offset object
;³
;³Registers changed : All!
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

XSin            DD 0.0
XCos            DD 0.0
YSin            DD 0.0
YCos            DD 0.0
ZSin            DD 0.0
ZCos            DD 0.0


One             DD 1.0
Zero            DD 0.0
RFlag           DD 0
PFlag           DD 0

FPDump		DD 0
ZToUse		DD 0

V03           PROC
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
		cmp	[ebp].V_ZFlag, 1
		je	V0Behind
		mov	eax, [ebp].V_InvZ
		mov	ZToUse, eax
V0Behind:
;---- Rotating normalvector X
                fld    [edi].F_NVX
                fmul   [esi].H_M00
                fld    [edi].F_NVY
                fmul   [esi].H_M01
                fld    [edi].F_NVZ
                fmul   [esi].H_M02
                faddp  st(1), st
                faddp  st(1), st
		fst	LIT_NVX
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
		fst	LIT_NVY
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
		fst	LIT_NVZ
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
		cmp	[ebp].V_ZFlag, 1
		je	V1Behind
		mov	eax, [ebp].V_InvZ
		mov	ZToUse, eax
V1Behind:
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
		cmp	[ebp].V_ZFlag, 1
		je	V2Behind
		mov	eax, [ebp].V_InvZ
		mov	ZToUse, eax
V2Behind:
                mov    eax, [ebp].V_ZFlag       ;Update poly's Z-flag
                add    [edi].F_ZFlag, eax       ;/
                cmp    [edi].F_ZFlag, 3         ;Entire polygon behind?
                je     DumpFace                 ;yes...

                mov    eax, [ebp].V_PFlag       ;Update poly's P-flag
                and    PFlag, eax               ;/
                jnz    DumpFace                 ;/entire face outside screen

		fld    ZToUse                   ;Converting 1/Z into fixed
		fadd	One
                fmul   SYS_ZBufFP               ;0.32 fixed point
                fistp  [edi].F_SortValue        ;Store it as integer


;---- Running lights...
;OBS! Don't change esi, edi, ecx
;eax, ebx, edx, ebp is free

                mov    LIT_ROut, 0
                mov    LIT_GOut, 0
                mov    LIT_BOut, 0

                LIT_Run

                fld    LIT_ROut
		fmul 	SYS_Ambient
                fstp   [edi].F_IntensityR

                fld    LIT_GOut
		fmul 	SYS_Ambient
                fstp   [edi].F_IntensityG

                fld    LIT_BOut
		fmul 	SYS_Ambient
                fstp   [edi].F_IntensityB


;---- Done lights
		mov    [edi].F_Type, 2





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

DumpFace:

                add    edi, F_Size              ;Next face
                dec    ecx
                jnz    VecLoop

                ret
V03           ENDP

END


