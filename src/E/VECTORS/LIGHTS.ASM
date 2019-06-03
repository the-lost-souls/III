;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : LIGHTS.ASM
;³Author       : Cyberfish/TLS
;³Last update  : 01.04.1998
;³Action       : Routines for div. lights
;³
;³Notes :
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model flat, C
.code

include w:\general\var.inc

public LIT_ROut
public LIT_GOut
public LIT_BOut
public LIT_Init			;Object

public LIT_NVX
public LIT_NVY
public LIT_NVZ

public LIT_InitP 		;Routine
public L01
public L02
public L03

LIT_ROut        DD 0.0
LIT_GOut        DD 0.0
LIT_BOut        DD 0.0

LIT_NVX		DD 0.0
LIT_NVY		DD 0.0
LIT_NVZ		DD 0.0

DotProduct      DD 0.0
Zero            DD 0.0
Intensity	DD 0.0



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : L01
;³ID           : L01
;³Type         : Procedure
;³Last update  : 01.04.98
;³Action       : Light routine for directional light
;³Optimized    : Nope
;³
;³Input variables : SYS_LOffs = Offset light (structure L01), edi = face
;³
;³Registers changed : All!
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

L01           PROC
                mov    ebp, SYS_LOffs
                fld    [ebp].L_Pos[0]
                fmul   LIT_NVX
                fld    [ebp].L_Pos[4]
                fmul   LIT_NVY
                fld    [ebp].L_Pos[8]
                fmul   LIT_NVZ
                faddp  st(1), st
                faddp  st(1), st                ;st(0) = cos à
                fcom   Zero
                fstp   DotProduct
                fstsw  ax
                sahf
                jbe    L01_NoLight

                fld    [ebp].L_R
                fmul   DotProduct
		fmul   [ebp].L_Intensity
                fadd   LIT_ROut
                fstp   LIT_ROut                   ;Save red intensity

                fld    [ebp].L_G
		fmul   [ebp].L_Intensity
                fmul   DotProduct
                fadd   LIT_GOut
                fstp   LIT_GOut                   ;Save red intensity

                fld    [ebp].L_B
		fmul   [ebp].L_Intensity
                fmul   DotProduct
                fadd   LIT_BOut
                fstp   LIT_BOut                   ;Save red intensity
L01_NoLight:

                ret
L01           ENDP


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : L02
;³ID           : L02
;³Type         : Procedure
;³Last update  : 10.03.1998
;³Action       : Light routine for point-light
;³Optimized    : Nope
;³
;³Input variables : SYS_LOffs = Offset light (struct L02), edi = face
;³
;³Registers changed : All!
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

L02 	      PROC
		mov	ebp, SYS_LOffs

					;st0  st1  st2  st3  st4  st5  st6  st7
;---- Creating vector0...
		mov	ebx, [edi].F_V0Offs

		fld	[ebp].L_Pos[0]	;XPos
		fsub	[ebx].V_XRotated;XVec
		fmul	st(0), st(0)	;X^2
		fld	[ebp].L_Pos[4]	;YPos X^2
		fsub	[ebx].V_YRotated;YVec X^2
		fmul	st(0), st(0)	;Y^2  X^2
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2
		fmul	st(0), st(0)	;Z^2  Y^2  X^2
		faddp	st(1)		;Sum  X^2
		faddp	st(1)		;Sum0

;---- Creating vector1...
		mov	ebx, [edi].F_V1Offs

		fld	[ebp].L_Pos[0]	;XPos Sum0
		fsub	[ebx].V_XRotated;XVec Sum0
		fmul	st(0), st(0)	;X^2  Sum0
		fld	[ebp].L_Pos[4]	;YPos X^2  Sum0
		fsub	[ebx].V_YRotated;YVec X^2  Sum0
		fmul	st(0), st(0)	;Y^2  X^2  Sum0
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2  Sum0
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2  Sum0
		fmul	st(0), st(0)	;Z^2  Y^2  X^2  Sum0
		faddp	st(1)		;Sum  X^2  Sum0
		faddp	st(1)		;Sum1 Sum0

;---- Creating vector2...
		mov	ebx, [edi].F_V2Offs

		fld	[ebp].L_Pos[0]	;XPos Sum1 Sum0
		fsub	[ebx].V_XRotated;XVec Sum1 Sum0
		fmul	st(0), st(0)	;X^2  Sum1 Sum0
		fld	[ebp].L_Pos[4]	;YPos X^2  Sum1 Sum0
		fsub	[ebx].V_YRotated;YVec X^2  Sum1 Sum0
		fmul	st(0), st(0)	;Y^2  X^2  Sum1 Sum0
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2  Sum1 Sum0
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2  Sum1 Sum0
		fmul	st(0), st(0)	;Z^2  Y^2  X^2  Sum1 Sum0
		faddp	st(1)		;Sum  X^2  Sum1 Sum0
		faddp	st(1)		;Sum2 Sum1 Sum0


		mov	ebx, [edi].F_V2Offs
		fcom	st(1)
		fstsw	ax
		sahf
		jb	L02_Check1

		mov     ebx, [edi].F_V1Offs
		fxch	st(1)
L02_Check1:

		fcom	st(2)
		fstsw	ax
		sahf
		jb	L02_Check2

		mov     ebx, [edi].F_V0Offs
L02_Check2:
		ffree	st(0)			;[ebx] = vertex with
		ffree	st(1)			;/smallest distance to light-
		ffree	st(2)			;/source



;---- Creating vector...
		fld	[ebp].L_Pos[0]	;XPos
		fsub	[ebx].V_XRotated;XVec
		fld	[ebp].L_Pos[4]	;YPos XVec
		fsub	[ebx].V_YRotated;YVec XVec
		fld	[ebp].L_Pos[8]	;ZPos YVec XVec
		fsub	[ebx].V_ZRotated;ZVec YVec XVec


;---- Calculating dot-product...
		fld	st(2)		;XVec ZVec YVec XVec
		fmul    LIT_NVX     	;Xmul ZVec YVec XVec
		fld	st(2)		;YVec Xmul ZVec YVec XVec
		fmul	LIT_NVY     	;Ymul Xmul ZVec YVec XVec
		fld	st(2)		;ZVec Ymul Xmul ZVec YVec XVec
		fmul	LIT_NVZ     	;Zmul Ymul Xmul ZVec YVec XVec

		faddp	st(1)		;DotP XMul ZVec YVec XVec
		faddp	st(1)		;DotP ZVec YVec XVec

                fcom   	Zero
                fstsw  	ax
                sahf
                jbe    	L02_NoLight


;---- Calculating vector's length...
		fxch    st(3)		;XVec ZVec YVec DotP
		fmul	st(0), st(0)    ;XV^2 ZVec YVec DotP
		fxch	st(2)		;YVec ZVec XV^2 DotP
		fmul	st(0), st(0)    ;YV^2 ZVec XV^2 DotP
		fxch	st(1)		;ZVec YV^2 XV^2 DotP
		fmul	st(0), st(0)    ;ZV^2 YV^2 XV^2 DotP

		faddp	st(1)		;Sum  XV^2 DotP
		faddp	st(1)		;Sum  DotP

		fsqrt			;Len  DotP

		fcom	[ebp].L_MaxRange	;Out of range?
		fstsw	ax
		sahf
		ja	L02_NoLight

;---- Calculating intensity with falloff (linear)...
		fld	st(0)
		fmul	[ebp].L_Falloff
		fsubr	[ebp].L_Intensity
		fstp	Intensity

		fdivp   st(1), st(0)	;st(0) = cos à
		fmul    Intensity


;---- Calculating colored light...
                fld    	[ebp].L_R
                fmul   	st(0), st(1)
                fadd   	LIT_ROut
                fstp   	LIT_ROut                   ;Save red intensity

                fld    	[ebp].L_G
                fmul   	st(0), st(1)
                fadd   	LIT_GOut
                fstp   	LIT_GOut                   ;Save red intensity

                fld    	[ebp].L_B
                fmul   	st(0), st(1)
                fadd   	LIT_BOut
                fstp   	LIT_BOut                   ;Save red intensity


L02_NoLight:
		ffree	st(0)
		ffree	st(1)
		ffree	st(2)
		ffree	st(3)

		ret
L02 	      ENDP



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : L03
;³ID           : L03
;³Type         : Procedure
;³Last update  : 10.03.1998
;³Action       : Light routine for point-light
;³Optimized    : Nope
;³
;³Input variables : SYS_LOffs = Offset light (struct L02), edi = face
;³
;³Registers changed : All!
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

L03	      PROC
		mov     ebp, SYS_LOffs

;---- Creating vector0...
		mov	ebx, [edi].F_V0Offs

		fld	[ebp].L_Pos[0]	;XPos
		fsub	[ebx].V_XRotated;XVec
		fmul	st(0), st(0)	;X^2
		fld	[ebp].L_Pos[4]	;YPos X^2
		fsub	[ebx].V_YRotated;YVec X^2
		fmul	st(0), st(0)	;Y^2  X^2
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2
		fmul	st(0), st(0)	;Z^2  Y^2  X^2
		faddp	st(1)		;Sum  X^2
		faddp	st(1)		;Sum0

;---- Creating vector1...
		mov	ebx, [edi].F_V1Offs

		fld	[ebp].L_Pos[0]	;XPos Sum0
		fsub	[ebx].V_XRotated;XVec Sum0
		fmul	st(0), st(0)	;X^2  Sum0
		fld	[ebp].L_Pos[4]	;YPos X^2  Sum0
		fsub	[ebx].V_YRotated;YVec X^2  Sum0
		fmul	st(0), st(0)	;Y^2  X^2  Sum0
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2  Sum0
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2  Sum0
		fmul	st(0), st(0)	;Z^2  Y^2  X^2  Sum0
		faddp	st(1)		;Sum  X^2  Sum0
		faddp	st(1)		;Sum1 Sum0

;---- Creating vector2...
		mov	ebx, [edi].F_V2Offs

		fld	[ebp].L_Pos[0]	;XPos Sum1 Sum0
		fsub	[ebx].V_XRotated;XVec Sum1 Sum0
		fmul	st(0), st(0)	;X^2  Sum1 Sum0
		fld	[ebp].L_Pos[4]	;YPos X^2  Sum1 Sum0
		fsub	[ebx].V_YRotated;YVec X^2  Sum1 Sum0
		fmul	st(0), st(0)	;Y^2  X^2  Sum1 Sum0
		fld	[ebp].L_Pos[8]	;ZPos Y^2  X^2  Sum1 Sum0
		fsub	[ebx].V_ZRotated;ZVec Y^2  X^2  Sum1 Sum0
		fmul	st(0), st(0)	;Z^2  Y^2  X^2  Sum1 Sum0
		faddp	st(1)		;Sum  X^2  Sum1 Sum0
		faddp	st(1)		;Sum2 Sum1 Sum0


		mov	ebx, [edi].F_V2Offs
		fcom	st(1)
		fstsw	ax
		sahf
		jb	L03_Check1

		mov     ebx, [edi].F_V1Offs
		fxch	st(1)
L03_Check1:

		fcom	st(2)
		fstsw	ax
		sahf
		jb	L03_Check2

		mov     ebx, [edi].F_V0Offs
L03_Check2:
		ffree	st(0)			;[ebx] = vertex with
		ffree	st(1)			;/smallest distance to light-
		ffree	st(2)			;/source


;---- Creating vector...
		fld     [ebx].V_XRotated	;XPos
		fsub    [ebp].L_Matrix[12]	;XVec
		fld 	[ebx].V_YRotated	;YPos XVec
		fsub    [ebp].L_Matrix[28]	;YVec XVec
		fld     [ebx].V_ZRotated	;ZPos YVec XVec
		fsub    [ebp].L_Matrix[44] 	;ZVec YVec XVec


;---- Calculating dot-product...
		fld	st(2)		;XVec ZVec YVec XVec
		fmul    LIT_NVX     	;Xmul ZVec YVec XVec
		fld	st(2)		;YVec Xmul ZVec YVec XVec
		fmul	LIT_NVY     	;Ymul Xmul ZVec YVec XVec
		fld	st(2)		;ZVec Ymul Xmul ZVec YVec XVec
		fmul	LIT_NVZ     	;Zmul Ymul Xmul ZVec YVec XVec

		faddp	st(1)		;DotP XMul ZVec YVec XVec
		faddp	st(1)		;DotP ZVec YVec XVec
		fchs

                fcom   	Zero		;Pointing away from light?
                fstsw  	ax
                sahf
                jbe    	L03_NoLight

;---- Calculating rayvector's length...
		fld	st(3)		;XVec DotP ZVec YVec XVec
		fmul	st, st   	;XV^2 DotP ZVec YVec XVec
		fld	st(3)           ;YVec XV^2 DotP ZVec YVec XVec
		fmul	st, st		;YV^2 XV^2 DotP ZVec YVec XVec
		fld	st(3)		;ZVec YV^2 XV^2 DotP ZVec YVec XVec
		fmul	st, st		;ZV^2 YV^2 XV^2 DotP ZVec YVec XVec
		faddp	st(1)		;Sum  XV^2 DotP ZVec YVec XVec
		faddp	st(1)		;Sum  DotP ZVec YVec XVec

		fsqrt			;Len  DotP ZVec YVec XVec

		fcom	[ebp].L_MaxRange;Out of range?
		fstsw	ax
		sahf
		ja	L03_NoLight

		fld	[ebp].L_Matrix[ 8];XVec LenA DotA ZVec YVec XVec
		fmul	st, st(5)         ;Mul  LenA DotA ZVec YVec XVec
		fld	[ebp].L_Matrix[24];YVec Mul  LenA DotA ZVec YVec XVec
		fmul	st, st(5)	  ;Mul  Mul  LenA DotA ZVec YVec XVe
		fld	[ebp].L_Matrix[40];ZVec Mul  Mul  LenA DotA ZVec YVec XVec
		fmul	st, st(5)	  ;Mul  Mul  Mul  LenA DotA ZVec YVec XVec
		faddp	st(1), st	  ;Sum  Mul  LenA DotA ZVec YVec XVec
		faddp	st(1), st	  ;DotB LenA DotA ZVec YVec XVec
		fdiv	st, st(1)	  ;cosB LenA DotA ZVec YVec XVec
		fcom    [ebp].L_ConeAngle ;cosB LenA DotA ZVec YVec XVec
		fstsw	ax
		sahf
		jbe     L03_NoLight

;---- Calculating intensityfalloff (linear)...
		fld	st(1)		;LenA cosB LenA DotA ZVec YVec XVec
		fmul	[ebp].L_Falloff
		fsubr	[ebp].L_Intensity;int cosB LenA DotA ZVec YVec XVec
		fxch	st(3)		;DotA cosB LenA int  ZVec YVec XVec

		fdiv    st, st(2)	;cosA cosB LenA int  ZVec YVec XVec
		fmul    st, st(3)	;intF cosB LenA int  ZVec YVec XVec
		fld	st(1)
		fcomp   [ebp].L_EdgeAngle
		fstsw	ax
		sahf
		jae     L03_NoSoftEdge

		fxch	st(1)		;cosB intF
		fsub	[ebp].L_ConeAngle

		fld	[ebp].L_EdgeAngle;C   cosB intF
		fsub	[ebp].L_ConeAngle;D   cosB intF

		fdivp	st(1), st	;
		fmulp	st(1), st       ;int


L03_NoSoftEdge:

;---- Calculating colored light...
                fld    	[ebp].L_R
                fmul   	st(0), st(1)
                fadd   	LIT_ROut
                fstp   	LIT_ROut                   ;Save red intensity

                fld    	[ebp].L_G
                fmul   	st(0), st(1)
                fadd   	LIT_GOut
                fstp   	LIT_GOut                   ;Save red intensity

                fld    	[ebp].L_B
                fmul   	st(0), st(1)
                fadd   	LIT_BOut
                fstp   	LIT_BOut                   ;Save red intensity

L03_NoLight:
		ffree	st(0)
		ffree	st(1)
		ffree	st(2)
		ffree	st(3)
		ffree	st(4)
		ffree	st(5)
		ffree	st(6)
		ffree	st(7)

		ret
L03	      ENDP



;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : LIT_InitP_
;³ID           : LIT
;³Type         : Procedure
;³Last update  : 27.12.97
;³Action       : Object routine for LIT_Init (run lights' plugins)
;³Optimized    : Nope
;³
;³Input variables : SYS_OOffs = Offset object
;³
;³Registers changed : All!
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

LIT_Init      DD C_InitLights
              DD -1                             ;No plugins


LIT_InitP     PROC
                mov    esi, SYS_LListOffs       ;Offset to light-list

LIT_InitLoop:
                cmp    DWORD PTR [esi], -1      ;End of list?
                je     LIT_InitDone

                push   esi
                mov    esi, [esi]
                or     esi, esi                 ;No light
                jz     LIT_InitDump

;ÄÄÄÄ Running plugins...
                xor    ecx, ecx
                lea    ebx, [esi + 04]          ;Point ebx to plugin-list

LIT_PluginL:    mov    eax, [ebx]               ;Load pluginoffset
                cmp    eax, -1                  ;End of pluginlist?
                je     LIT_DonePlugins          ;Done plugins
                cmp    eax, 0                   ;Fake plugin?
                je     LIT_FakePlugin           ;Dump plugin

                push   ebx
                push   ecx

                mov    eax, [ebx + 04]          ;Load plugindataoffset
                mov    SYS_POffs, eax           ;Setting up system variables
                mov    SYS_PCaller, esi         ;/

                call   [ ebx ]                  ;Call plugin!

                pop    ecx
                pop    ebx

LIT_FakePlugin: add    ebx, 8                   ;Next plugin
                inc    ecx
                cmp    ecx, 8
                je     LIT_DonePlugins
                jmp    LIT_PluginL

LIT_DonePlugins:

;ÄÄÄÄ Done plugins...


LIT_InitDump:
                pop    esi
                add    esi, 4                   ;Next light...
                jmp    LIT_InitLoop

LIT_InitDone:

                ret
LIT_InitP     ENDP

END
