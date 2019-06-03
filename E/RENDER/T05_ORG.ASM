;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T05_ORG.ASM
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 10.02.1998
;쿌ction       : Draws a perspective mapped triangle, 32k colors, z-buffer
;�
;쿙otes :
;쿟o be done:
;� -fix the last pixel, if scanlength=17
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model Flat, C
.code

include w:\general\var.inc

public T05_Tri
public T05_Init
public T05_Call

extrn 	T04_Tri  	:PROC
extrn   T04_FlatColor	:WORD


Edge          STRUC

X               DD ?
UdZ             DD ?
VdZ             DD ?
NdZ             DD ?

Edge          ENDS

ALIGN 4


;컴컴 In-variables...
T05_MapModU     DD 1.0                           ;FP: Modifier, mappingcoordinates
T05_MapModV	DD 1.0
T05_MapOffset   DD 0                            ;Offset texturemap
T05_MapWidth    DD 320                          ;INT: Tmap width, in pixels


;컴컴 Locally used "constants"
Fixed16_16      DD 65536.0
Fixed16         DD 4096.0                       ;65536.0/ 16.0
Fixed032        DD 4294967296.0
Sixteen         DD 16.0
One             DD 1.0
AlmostZero      DD 0.000000000000000000001

FPDump          DD 0




;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : ZClip
;쿔D           :
;쿟ype         : Procedure
;쿗ast update  :
;쿌ction       : Z-clips a polygon pointed to by esi
;쿚ptimized    : -
;�
;쿔nput variables :
;�
;�
;쿙otes : New faces are returned in NewFace0 and NewFace1
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴


ToU		DD ?
ToV		DD ?
FromU		DD ?
FromV		DD ?
DestU		DD ?
DestV		DD ?

ClipLine      MACRO From, To, Dest              ;st0  st1  st2  st3  st4  st5

;---- Calculating 1/DeltaZ
		fld	[To].V_ZRotated         ;Z1
		fsub	[From].V_ZRotated	;ZD
		fld	SYS_ZClip
		fst	[Dest].V_ZRotated
		fsub	[From].V_ZRotated
		fdivrp	st(1), st

;---- Getting new 3D-X
		fld	[To].V_XRotated		;X1   1/ZD
		fsub	[From].V_XRotated	;XD   1/ZD
		fmul	st, st(1)		;XM   1/ZD
		fadd	[From].V_XRotated	;X    1/ZD
		fstp    [Dest].V_XRotated	;1/ZD

;---- Getting new 3D-Y
		fld	[To].V_YRotated		;Y1   1/ZD
		fsub	[From].V_YRotated	;YD   1/ZD
		fmul	st, st(1)		;YM   1/ZD
		fadd	[From].V_YRotated	;Y    1/ZD
		fstp    [Dest].V_YRotated	;1/ZD


;---- Getting new mappingcoordinates (vertex's)
		fld	[To].V_U                ;U1   1/ZD
		fsub	[From].V_U              ;UD   1/ZD
		fmul	st, st(1)		;UM   1/ZD
		fadd	[From].V_U              ;U    1/ZD
		fstp    [Dest].V_U              ;1/ZD

		fld	[To].V_V		;V1   1/ZD
		fsub	[From].V_V		;VD   1/ZD
		fmul	st, st(1)		;VM   1/ZD
		fadd	[From].V_V		;V    1/ZD
		fstp    [Dest].V_V		;1/ZD

;---- Getting new intensitivities...
		fld	[To].V_IntensityR       ;U1   1/ZD
		fsub	[From].V_IntensityR     ;UD   1/ZD
		fmul	st, st(1)		;UM   1/ZD
		fadd	[From].V_IntensityR     ;U    1/ZD
		fstp    [Dest].V_IntensityR     ;1/ZD

		fld	[To].V_IntensityG  	;V1   1/ZD
		fsub	[From].V_IntensityG	;VD   1/ZD
		fmul	st, st(1)		;VM   1/ZD
		fadd	[From].V_IntensityG	;V    1/ZD
		fstp    [Dest].V_IntensityG	;1/ZD

		fld	[To].V_IntensityB  	;V1   1/ZD
		fsub	[From].V_IntensityB	;VD   1/ZD
		fmul	st, st(1)		;VM   1/ZD
		fadd	[From].V_IntensityB	;V    1/ZD
		fstp    [Dest].V_IntensityB	;1/ZD


;---- Getting new mappingcoordinates (face's)
		fld	ToU                	;U1   1/ZD
		fsub	FromU                   ;UD   1/ZD
		fmul	st, st(1)		;UM   1/ZD
		fadd	FromU                   ;U    1/ZD
		fstp    DestU                   ;1/ZD

		fld	ToV             	;V1   1/ZD
		fsub	FromV     		;VD   1/ZD
		fmul	st, st(1)		;VM   1/ZD
		fadd	FromV     		;V    1/ZD
		fstp    DestV     		;1/ZD

		fstp	FPDump			;empty

	      ENDM


NewVert0	DD V_Size/4 DUP (?)
ZClip_U0	DD ?
ZClip_V0	DD ?

NewVert1	DD V_Size/4 DUP (?)
ZClip_U1	DD ?
ZClip_V1	DD ?

NewVert2	DD V_Size/4 DUP (?)
ZClip_U2	DD ?
ZClip_V2	DD ?

NewVert3	DD V_Size/4 DUP (?)
ZClip_U3	DD ?
ZClip_V3	DD ?

Vertex0		EQU ebx
Vertex1		EQU ecx
Vertex2		EQU edx

NewFace0	DD F_Size/4 DUP (?)
NewFace1	DD F_Size/4 DUP (?)


;esi = face to be clipped
;changes all registers except from esi
ZClip         PROC
		mov	Vertex0, [esi].F_V0Offs
		mov	Vertex1, [esi].F_V1Offs
		mov	Vertex2, [esi].F_V2Offs

		mov	edi, OFFSET NewVert0

;------ Testing Vertex 0
		cmp	[Vertex0].V_ZFlag, 1
		je	DumpV0

		push	esi
		push	ecx

		mov	esi, Vertex0		;Copying vertex 0
		mov	ecx, V_Size/4
		rep	movsd

		pop	ecx
		pop	esi

		mov	eax, [esi].F_U0		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, [esi].F_V0
		mov	[edi + 4], eax
		add	edi, 8

DumpV0:



;---- Testing line 0->1 for clipping...
		mov	eax, [Vertex1].V_ZFlag
		xor	eax, [Vertex0].V_ZFlag
		jz	NoLineClip0

		mov	eax, [esi].F_U0
		mov	FromU, eax
		mov	eax, [esi].F_V0
		mov	FromV, eax
		mov	eax, [esi].F_U1
		mov	ToU, eax
		mov	eax, [esi].F_V1
		mov	ToV, eax

		ClipLine Vertex0, Vertex1, edi
		add	edi, V_Size

		mov	eax, DestU		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, DestV
		mov	[edi + 4], eax
		add	edi, 8
NoLineClip0:



;---- Testing vertex1...
		cmp	[Vertex1].V_ZFlag, 1
		je	DumpV1

		push	esi
		push	ecx

		mov	esi, Vertex1
		mov	ecx, V_Size/4
		rep	movsd

		pop	ecx
		pop	esi

		mov	eax, [esi].F_U1		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, [esi].F_V1
		mov	[edi + 4], eax
		add	edi, 8

DumpV1:


;---- Testing line 1->2 for clipping...
		mov	eax, [Vertex2].V_ZFlag
		xor	eax, [Vertex1].V_ZFlag
		jz	NoLineClip1

		mov	eax, [esi].F_U1
		mov	FromU, eax
		mov	eax, [esi].F_V1
		mov	FromV, eax
		mov	eax, [esi].F_U2
		mov	ToU, eax
		mov	eax, [esi].F_V2
		mov	ToV, eax

		ClipLine Vertex1, Vertex2, edi
		add	edi, V_Size

		mov	eax, DestU		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, DestV
		mov	[edi + 4], eax
		add	edi, 8
NoLineClip1:


;---- Testing vertex2...
		cmp	[Vertex2].V_ZFlag, 1
		je	DumpV2

		push	esi
		push	ecx

		mov	esi, Vertex2
		mov	ecx, V_Size/4
		rep	movsd

		pop	ecx
		pop	esi

		mov	eax, [esi].F_U2		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, [esi].F_V2
		mov	[edi + 4], eax
		add	edi, 8

DumpV2:


;---- Testing line 2->0 for clipping...
		mov	eax, [Vertex0].V_ZFlag
		xor	eax, [Vertex2].V_ZFlag
		jz	NoLineClip2

		mov	eax, [esi].F_U2
		mov	FromU, eax
		mov	eax, [esi].F_V2
		mov	FromV, eax
		mov	eax, [esi].F_U0
		mov	ToU, eax
		mov	eax, [esi].F_V0
		mov	ToV, eax

		ClipLine Vertex2 Vertex0 edi
		add	edi, V_Size

		mov	eax, DestU		;Copying mapping coords
		mov	[edi + 0], eax
		mov	eax, DestV
		mov	[edi + 4], eax
		add	edi, 8

NoLineClip2:


;---- Projecting the vertexes...
		mov	eax, 1
		and	eax, [esi].F_ZFlag	;=1 if four vertices

		lea	ecx, [eax + 3]
		mov	edi, OFFSET NewVert0

ZClip_VertLoop:
                fld1
                fld    	[edi].V_ZRotated
                fdivp  	st(1), st
                fst    	[edi].V_InvZ

                fld    	[edi].V_XRotated
                fmul   	SYS_XPerspective
                fmul   	st, st(1)
                fadd   	SYS_XCenter
                fstp   	[edi].V_X2D              ;Store 2D-X

                fmul   	[edi].V_YRotated
                fmul   	SYS_YPerspective
                fadd   	SYS_YCenter
                fstp   	[edi].V_Y2D

		add	edi, V_Size + 8
		dec	ecx
		jnz	ZClip_VertLoop


;---- Setting up first face...
		mov	ebp, OFFSET NewFace0
		mov	[ebp].F_V0Offs, OFFSET NewVert0
		mov	[ebp].F_V1Offs, OFFSET NewVert1
		mov	[ebp].F_V2Offs, OFFSET NewVert2

		mov	eax, ZClip_U0
		mov	ebx, ZClip_V0
		mov	[ebp].F_U0, eax
		mov	[ebp].F_V0, ebx
		mov	eax, ZClip_U1
		mov	ebx, ZClip_V1
		mov	[ebp].F_U1, eax
		mov	[ebp].F_V1, ebx
		mov	eax, ZClip_U2
		mov	ebx, ZClip_V2
		mov	[ebp].F_U2, eax
		mov	[ebp].F_V2, ebx

		mov	eax, [esi].F_Material
		mov	ebx, [esi].F_IntensityR
		mov	ecx, [esi].F_IntensityG
		mov	edx, [esi].F_IntensityB
		mov	[ebp].F_Material, eax
		mov	[ebp].F_IntensityR, ebx
		mov	[ebp].F_IntensityG, ecx
		mov	[ebp].F_IntensityB, edx


;---- Evt. setting up second face...
		cmp	[esi].F_ZFlag, 2
		je	NotTwoFaces

;---- Setting up second face...
		mov	ebp, OFFSET NewFace1
		mov	[ebp].F_V0Offs, OFFSET NewVert0
		mov	[ebp].F_V1Offs, OFFSET NewVert2
		mov	[ebp].F_V2Offs, OFFSET NewVert3

		mov	eax, ZClip_U0
		mov	ebx, ZClip_V0
		mov	[ebp].F_U0, eax
		mov	[ebp].F_V0, ebx
		mov	eax, ZClip_U2
		mov	ebx, ZClip_V2
		mov	[ebp].F_U1, eax
		mov	[ebp].F_V1, ebx
		mov	eax, ZClip_U3
		mov	ebx, ZClip_V3
		mov	[ebp].F_U2, eax
		mov	[ebp].F_V2, ebx

		mov	eax, [esi].F_Material
		mov	ebx, [esi].F_IntensityR
		mov	ecx, [esi].F_IntensityG
		mov	edx, [esi].F_IntensityB
		mov	[ebp].F_Material, eax
		mov	[ebp].F_IntensityR, ebx
		mov	[ebp].F_IntensityG, ecx
		mov	[ebp].F_IntensityB, edx

NotTwoFaces:

		ret
ZClip         ENDP




;-------------------------------------------------------------------
ALIGN 4

RLookup       DW 32 DUP (0)
GLookup       DW 64 DUP (0)
BLookup       DW 32 DUP (0)

One31         DD 0.0322580645161
One63	      DD 0.015873015873

CreateLookups MACRO

;---- Creating red lookup
                fld    RInt
                fmul   One31
                fmul   Fixed032
                fistp  Slope

                mov    ecx, 32                  ;loopcounter
                xor    ebx, ebx                 ;mempointer
                mov    eax, 3FFFFFFFh           ;0.5 fixed point
                mov    ebp, Slope
                xor    edx, edx
LookupR:
                mov    edi, edx
                shl    edi, 11
                mov    RLookup[ebx*2], di
                add    eax, ebp
                adc    edx, 0

                inc    ebx
                dec    ecx
                jnz    LookupR


;---- Creating green lookup
                fld    GInt
                fmul   One63
                fmul   Fixed032
                fistp  Slope

                mov    ecx, 64                  ;loopcounter
                xor    ebx, ebx                 ;mempointer
                mov    eax, 3FFFFFFFh           ;0.5 fixed point
                mov    ebp, Slope
                xor    edx, edx
LookupG:
                mov    edi, edx
                shl    edi, 5
                mov    GLookup[ebx*2], di
                add    eax, ebp
                adc    edx, 0

                inc    ebx
                dec    ecx
                jnz    LookupG


;---- Creating blue lookup
                fld    BInt
                fmul   One31
                fmul   Fixed032
                fistp  Slope

                mov    ecx, 32                  ;loopcounter
                xor    ebx, ebx                 ;mempointer
                mov    eax, 3FFFFFFFh           ;0.5 fixed point
                mov    ebp, Slope
                xor    edx, edx
LookupB:
                mov    BLookup[ebx*2], dx
                add    eax, ebp
                adc    edx, 0

                inc    ebx
                dec    ecx
                jnz    LookupB

              ENDM





;----------------------------------------------------------------------------






;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T05_Call
;쿔D           : T05
;쿟ype         : Procedure
;쿗ast update  : 10.02.1998
;쿌ction       : Runs z-clipping, then draws
;쿚ptimized    : No
;�
;쿔nput variables : [esi] = polygon
;�
;쿙otes : FPU stack must be empty.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
ALIGN 4

RInt            DD 0.0
GInt            DD 0.0
BInt            DD 0.0

ColorRangeR     DD 31.0
ColorRangeG     DD 63.0
ColorRangeB     DD 31.0

;esi = polygon
T05_Call      PROC
;---- Getting color...
                mov    ebp, [esi].F_Material

                fldcw  FPU_Round

                fld    [esi].F_IntensityR
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRangeR
                fcom   ColorRangeR
                fstsw  ax
                sahf
                jb     NoRClip
                ffree  st
                fld    ColorRangeR
NoRClip:
                fstp   RInt

                fld    [esi].F_IntensityG
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRangeG
                fcom   ColorRangeG
                fstsw  ax
                sahf
                jb     NoGClip
                ffree  st
                fld    ColorRangeG
NoGClip:
                fstp   GInt

                fld    [esi].F_IntensityB
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRangeB
                fcom   ColorRangeB
                fstsw  ax
                sahf
                jb     NoBClip
                ffree  st
                fld    ColorRangeB
NoBClip:
                fstp   BInt

                mov    ebx, [ebp].M_MapWidth
                mov    T05_MapWidth, ebx
                mov    ebx, [ebp].M_MapOffset
                mov    T05_MapOffset, ebx

                CreateLookups

;---- Z-clipping?
                cmp    [esi].F_ZFlag, 0
                je     NoZClip
						;esi = face to be clipped
		call   ZClip

                cmp    [esi].F_ZFlag, 2         ;Draw to/one polygon?
                je     DrawOne                  ;Draw one!

                mov    esi, OFFSET NewFace1

		cmp     RInt, 0			;Check if face is entirely
		jne	RunNormal0      	;/black. Then call flatfill...
		cmp	GInt, 0
		jne	RunNormal0
		cmp	BInt, 0
		jne	RunNormal0

		mov	T04_FlatColor, 0
		call	T04_Tri
		jmp	DrawOne

RunNormal0:
                call   T05_Tri

;---- Drawing only one face...
DrawOne:
                mov    esi, OFFSET NewFace0
NoZClip:

		cmp     RInt, 0			;Check if face is entirely
		jne	RunNormal1      	;/black. Then call flatfil...
		cmp	GInt, 0
		jne	RunNormal1
		cmp	BInt, 0
		jne	RunNormal1

		mov	T04_FlatColor, 0
		call	T04_Tri
		ret
RunNormal1:
                call   T05_Tri

                ret
T05_Call      ENDP













;----------------------------------------------------------------------------





;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T05_Scanline
;쿔D           : T05
;쿟ype         : Macro
;쿗ast update  : 12.07.1997
;쿌ction       : Draws a perspectivemapped, horisontal scanline, 32k, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [eax]  = left edge
;�                  [ebx]  = right edge
;�
;쿙otes : FPU stack must be empty. All integer registers are saved.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4
ScanLength      DD 0                            ;int scanline length
Slope_1         DD 0                            ;int(USlope) + (int(VSlope)+1)*MapSize
Slope           DD 0                            ;int(USlope) + int(VSlope)*MapSize
UFrac           DD 0                            ;int U fraction (0.32)
VFrac           DD 0                            ;int V fraction (0.32)
USlopeFixed     DD 0                            ;
VSlopeFixed     DD 0
ULeftFixed      DD 0                            ;int U 16.16 fixed point start left
VLeftFixed      DD 0                            ;int V 16.16 fixed point start left

EdgeRight     Edge ?                            ;Right edge data
EdgeLeft      Edge ?                            ;Left edge data

LeftNdZ         DD 0

T05_ScanLine  MACRO
                                     ;st0  st1  st2  st3  st4  st5  st6  st7


;컴컴 Clip U/Z, V/Z and 1/Z right to last pixel...
                fldcw  FPU_RoundDown

                push   eax
                fld    [ebx].X       ;XR
                fcom   SYS_FPClipRight
                fstsw  ax
                sahf
                jb     NoRightClip
                fstp   FPDump
                fld    SYS_FPClipRight

NoRightClip:
                pop    eax

                fistp  EdgeRight.X   ;
                fild   EdgeRight.X   ;XRI
                fsub   [ebx].X       ;XD

                fld    st            ;XD   XD
                fmul   UZSlope       ;XDU  XD
                fadd   [ebx].UdZ     ;UZR  XD
                fstp   EdgeRight.UdZ ;XD

                fld    st            ;XD   XD
                fmul   VZSlope       ;XDU  XD
                fadd   [ebx].VdZ     ;UZR  XD
                fstp   EdgeRight.VdZ ;XD

                fmul   NZSlope       ;XDU
                fadd   [ebx].NdZ     ;UZR
                fstp   EdgeRight.NdZ ;


;컴컴 Clip U/Z, V/Z and 1/Z left to next pixel...
                fldcw  FPU_RoundUp

                push   eax
                fld    [eax].X       ;XL
                fcom   SYS_FPClipLeft
                fstsw  ax
                sahf
                ja     NoLeftClip
                fstp   FPDump
                fld    SYS_FPClipLeft

NoLeftClip:
                pop    eax

                fistp  EdgeLeft.X    ;
                fild   EdgeLeft.X    ;XL
                fsub   [eax].X       ;XD

                fld    st            ;XD   XD
                fmul   UZSlope       ;XV   XD
                fadd   [eax].UdZ     ;UZL  XD

                fld    st(1)         ;XD   UZL  XD
                fmul   VZSlope       ;XV   UZL  XD
                fadd   [eax].VdZ     ;VZL  UZL  XD
                fxch   st(2)         ;XD   UZL  VZL


                fmul   NZSlope       ;XNZ  UZL  VZL
                fadd   [eax].NdZ     ;NZL  UZL  VZL


;컴컴 Save fixed point 1/Z left
                fld    st            ;1/ZL 1/ZL U/ZL V/ZL
                fmul   Fixed032      ;1/ZL 1/ZL U/ZL V/ZL
                fistp  LeftNdZ       ;1/ZL U/ZL V/ZL

;컴컴 Get Z left...
                fld1                 ;1    1/ZL U/ZL V/ZL
                fdiv   st, st(1)     ;ZL   1/ZL U/ZL V/ZL




;컴컴 !!!INTEGER INSTRUCTIONS HERE!!!
                pushad                          ;Save all registers

                mov    edi, PixelPtr            ;Use edi as mempointer
                add    edi, EdgeLeft.X

                mov    eax, EdgeLeft.X          ;Calculate scanline length
                mov    ebx, EdgeRight.X         ;/
                sub    ebx, eax                 ;ebx = scanline length
                mov    ScanLength, ebx

                cmp    ebx, 0
                jl     Scan_Done

                cmp    ebx, 16                  ;Run leftoverloop now?
                jbe    LeftOver1


;컴컴 Get U and V left (the fdiv is finished)
                fld    st            ;ZL   ZL   1/ZL U/ZL V/ZL
                fmul   st, st(4)     ;VL   ZL   1/ZL U/ZL V/ZL
                fxch   st(1)         ;ZL   VL   1/ZL U/ZL V/ZL
                fmul   st, st(3)     ;UL   VL   1/ZL U/ZL V/ZL


;컴컴 Get U/Z and V/Z (right)
                fxch   st(2)         ;1/ZL VL   UL   U/ZL V/ZL
                fadd   NZSlope16     ;1/ZR VL   UL   U/ZL V/ZL
                fxch   st(3)         ;U/ZL VL   UL   1/ZR V/ZL
                fadd   UZSlope16     ;U/ZR VL   UL   1/ZR V/ZL
                fxch   st(4)         ;V/ZL VL   UL   1/ZR U/ZR
                fadd   VZSlope16     ;V/ZR VL   UL   1/ZR U/ZR
                fstp   st(5)         ;VL   UL   1/ZR U/ZR V/ZR


;컴컴 Get Z-right
                fld    One           ;1    VL   UL   1/ZR U/ZR V/ZR
                fdiv   st, st(3)     ;ZR   VL   UL   1/ZR U/ZR V/ZR






;-------------------------- LOOP STARTS HERE -------------------------------

Loop16:

;컴컴 Get U- and V-right
                fld    st            ;ZR   ZR   VL   UL   1/ZR U/ZR V/ZR
                fmul   st, st(5)     ;UR   ZR   VL   UL   1/ZR U/ZR V/ZR
                fxch   st(1)         ;ZR   UR   VL   UL   1/ZR U/ZR V/ZR
                fmul   st, st(6)     ;VR   UR   VL   UL   1/ZR U/ZR V/ZR


;컴컴 Calculating slopes...
                fld    st            ;VR   VR   UR   VL   UL   1/ZR U/ZR V/ZR
                fsub   st, st(3)     ;VD   VR   UR   VL   UL   1/ZR U/ZR V/ZR
                fmul   Fixed16       ;VD   VR   UR   VL   UL   1/ZR U/ZR V/ZR
                fistp  VSlopeFixed   ;VR   UR   VL   UL   1/ZR U/ZR V/ZR
                fxch   st(1)         ;UR   VR   VL   UL   1/ZR U/ZR V/ZR
                fld    st            ;UR   UR   VR   VL   UL   1/ZR U/ZR V/ZR
                fsub   st, st(4)     ;UD   UR   VR   VL   UL   1/ZR U/ZR V/ZR
                fmul   Fixed16       ;UD   UR   VR   VL   UL   1/ZR U/ZR V/ZR
                fistp  USlopeFixed   ;UR   VR   VL   UL   1/ZR U/ZR V/ZR


;컴컴 Convert U and V left...
                fxch   st(2)         ;VL   VR   UR   UL   1/ZR U/ZR V/ZR
                fmul   Fixed16_16    ;VL   VR   UR   UL   1/ZR U/ZR V/ZR
                fistp  VLeftFixed    ;VR   UR   UL   1/ZR U/ZR V/ZR
                fxch   st(2)         ;UL   UR   VR   1/ZR U/ZR V/ZR
                fmul   Fixed16_16    ;UL   UR   VR   1/ZR U/ZR V/ZR
                fistp  ULeftFixed    ;UR   VR   1/ZR U/ZR V/ZR


;컴컴 Get U/Z and V/Z (right)
                fxch   st(2)         ;1/ZL VL   UL   U/ZL V/ZL
                fadd   NZSlope16     ;1/ZR VL   UL   U/ZL V/ZL
                fxch   st(3)         ;U/ZL VL   UL   1/ZR V/ZL
                fadd   UZSlope16     ;U/ZR VL   UL   1/ZR V/ZL
                fxch   st(4)         ;V/ZL VL   UL   1/ZR U/ZR
                fadd   VZSlope16     ;V/ZR VL   UL   1/ZR U/ZR
                fstp   st(5)         ;VL   UL   1/ZR U/ZR V/ZR


;컴컴 Check if we need Z for end of line next time...
                cmp    ScanLength, 32
                jg     NotEOL
                ffree  st(4)         ;VL   UL   1/ZR U/ZR
                ffree  st(3)         ;VL   UL   1/ZR
                ffree  st(2)         ;VL   UL

                fld    EdgeRight.VdZ ;V/ZR VL   UL
                fld    EdgeRight.UdZ ;U/ZR V/ZR VL   UL
                fld    EdgeRight.NdZ ;1/ZR U/ZR V/ZR VL   UL

                fstp   st(5)         ;U/ZR V/ZR VL   UL   NZR
                fstp   st(5)         ;V/ZR VL   UL   1/ZR U/ZR
                fstp   st(5)         ;VL   UL   1/ZR U/ZR V/ZR
NotEOL:




;컴컴 Setting up slopes...
                mov    eax, USlopeFixed
                mov    ebx, eax                 ;Take copy
                sar    eax, 16                  ;Get int part
                sal    ebx, 16                  ;Get fractional part
                mov    UFrac, ebx               ;Save fractional part

                mov    ecx, VSlopeFixed
                mov    edx, ecx
                sar    ecx, 16
                sal    edx, 16
                mov    VFrac, edx

                imul   ecx, T05_MapWidth
                add    ecx, eax
                mov    Slope, ecx               ;Save normal slope
                add    ecx, T05_MapWidth
                mov    [Slope - 4], ecx         ;Save slope + 1 scanline


;컴컴 Setting up starting values...
                mov    eax, ULeftFixed          ;
                add    eax, 8000h               ;add 0.5 (16.16 fixed point)
                mov    ebx, eax                 ;Take copy
                shr    eax, 16                  ;Get int part
                shl    ebx, 16                  ;Get fractional part

                mov    esi, VLeftFixed
                add    esi, 8000h
                mov    ecx, esi
                shr    esi, 16
                shl    ecx, 16
                imul   esi, T05_MapWidth

                add    esi, eax                 ;Add U-offset

                mov    edx, T05_MapOffset
                shr    edx, 1
                add    esi, edx                 ;Add textureoffset

                mov    edx, LeftNdZ
                mov    ebp, UFrac



;컴컴 Get next Z...
                fld1                 ;1    VL   UL   1/ZR U/ZR V/ZR
                fdiv   st, st(3)     ;ZR   VL   UL   1/ZR U/ZR V/ZR


;eax = generally used
;ebx = u-fraction
;ecx = v-fraction
;edx = 1/Z interpolation
;esi = tmap pointer
;edi = dbuf pointer
;ebp = u-fraction increase

                xor    eax, eax      ;Blank upper 16
;컴컴 Pixel 0

                cmp    edx, [edi*4 + 12345678h]
Offs00_A:       jbe    Dump0

                mov    [edi*4 + 12345678h], edx
Offs00_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs00_C:
                pop    ecx
                pop    ebx
Dump0:

;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax
                add    ebx, ebp

                adc    esi, [Slope + eax*4]
                cmp    edx, [edi*4 + 12345678h]

Offs01_A:       jbe    Dump1

                mov    [edi*4 + 12345678h], edx
Offs01_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs01_C:
                pop    ecx
                pop    ebx
Dump1:

;컴컴 Pixel 2
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs02_A:       jbe    Dump2

                mov    [edi*4 + 12345678h], edx
Offs02_B:
                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs02_C:
                pop    ecx
                pop    ebx
Dump2:
;컴컴 Pixel 3
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs03_A:       jbe    Dump3

                mov    [edi*4 + 12345678h], edx
Offs03_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs03_C:
                pop    ecx
                pop    ebx
Dump3:
;컴컴 Pixel 4
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs04_A:       jbe    Dump4

                mov    [edi*4 + 12345678h], edx
Offs04_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs04_C:
                pop    ecx
                pop    ebx
Dump4:
;컴컴 Pixel 5
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs05_A:       jbe    Dump5

                mov    [edi*4 + 12345678h], edx
Offs05_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs05_C:
                pop    ecx
                pop    ebx
Dump5:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs06_A:       jbe    Dump6

                mov    [edi*4 + 12345678h], edx
Offs06_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs06_C:
                pop    ecx
                pop    ebx
Dump6:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]
                cmp    edx, [edi*4 + 12345678h]
Offs07_A:       jbe    Dump7

                mov    [edi*4 + 12345678h], edx
Offs07_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs07_C:
                pop    ecx
                pop    ebx
Dump7:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs08_A:       jbe    Dump8
                mov    [edi*4 + 12345678h], edx
Offs08_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs08_C:
                pop    ecx
                pop    ebx
Dump8:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs09_A:       jbe    Dump9

                mov    [edi*4 + 12345678h], edx
Offs09_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs09_C:
                pop    ecx
                pop    ebx
Dump9:
;컴컴 Pixel 10
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs10_A:       jbe    Dump10

                mov    [edi*4 + 12345678h], edx
Offs10_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs10_C:
                pop    ecx
                pop    ebx
Dump10:
;컴컴 Pixel 11
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs11_A:       jbe    Dump11

                mov    [edi*4 + 12345678h], edx
Offs11_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs11_C:
                pop    ecx
                pop    ebx
Dump11:
;컴컴 Pixel 12
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs12_A:       jbe    Dump12

                mov    [edi*4 + 12345678h], edx
Offs12_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs12_C:
                pop    ecx
                pop    ebx
Dump12:
;컴컴 Pixel 13
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs13_A:       jbe    Dump13

                mov    [edi*4 + 12345678h], edx
Offs13_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs13_C:
                pop    ecx
                pop    ebx
Dump13:
;컴컴 Pixel 14
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs14_A:       jbe    Dump14

                mov    [edi*4 + 12345678h], edx
Offs14_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs14_C:
                pop    ecx
                pop    ebx
Dump14:
;컴컴 Pixel 15
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs15_A:       jbe    Dump15

                mov    [edi*4 + 12345678h], edx
Offs15_B:

                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
Offs15_C:
                pop    ecx
                pop    ebx
Dump15:
                add    edx, NZSlopeInt

                mov    LeftNdZ, edx

                add    edi, 16                  ;Move memory pointer
                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                jbe    LeftOver2
                jmp    Loop16



;컴컴 Write leftover pixels...

LeftOver1:

;컴컴 Get U and V left
                fld    st            ;ZL   ZL   컴컴 U/ZL V/ZL
                fmul   st, st(3)     ;UL   ZL   컴컴 U/ZL V/ZL
                fxch   st(1)         ;ZL   UL   컴컴 U/ZL V/ZL
                fmul   st, st(4)     ;VL   UL   컴컴 U/ZL V/ZL

                ffree  st(4)         ;VL   UL   컴컴 U/ZL
                ffree  st(3)         ;VL   UL   컴컴


                fld    EdgeRight.VdZ ;V/ZR VL   UL   컴컴
                fld    EdgeRight.UdZ ;U/ZR V/ZR VL   UL   컴컴

                fstp   st(5)         ;V/ZR VL   UL   컴컴 U/ZR
                fstp   st(5)         ;VL   UL   컴컴 U/ZR V/ZR

                fld    EdgeRight.NdZ ;1/ZR VL   UL   컴컴 U/ZR V/ZR
                fdivr  One           ;ZR   VL   UL   컴컴 U/ZR V/ZR


LeftOver2:

                                     ;ZR   VL   UL   컴컴 U/ZR V/ZR

;컴컴 Get U and V at right end of line...
                fld    st            ;ZR   ZR   VL   UL   컴컴 U/ZR V/ZR
                fmul   st, st(5)     ;UR   ZR   VL   UL   컴컴 U/ZR V/ZR
                fxch   st(1)         ;ZR   UR   VL   UL   컴컴 U/ZR V/ZR
                fmul   st, st(6)     ;VR   UR   VL   UL   컴컴 U/ZR V/ZR


;컴컴 Get deltas and slopes...
                fsub   st, st(2)     ;VD   UR   VL   UL   컴컴 U/ZR V/ZR
                fxch   st(1)         ;UR   VD   VL   UL   컴컴 U/ZR V/ZR
                fsub   st, st(3)     ;UD   VD   VL   UL   컴컴 U/ZR V/ZR
                fild   ScanLength    ;XD   UD   VD   VL   UL   컴컴 U/ZR V/ZR
                fdivr  Fixed16_16    ;FP/L UD   VD   VL   UL   컴컴 U/ZR V/ZR
                fmul   st(1), st     ;FP/L US   VD   VL   UL   컴컴 U/ZR V/ZR
                fmulp  st(2), st     ;US   VS   VL   UL   컴컴 U/ZR V/ZR
                fistp  USlopeFixed   ;VS   VL   UL   컴컴 U/ZR V/ZR
                fistp  VSlopeFixed   ;VL   UL   컴컴 U/ZR V/ZR


;컴컴 Convert U and V left...
                fmul   Fixed16_16    ;VL   UL   컴컴 U/ZR V/ZR
                fistp  VLeftFixed    ;UL   컴컴 U/ZR V/ZR
                fmul   Fixed16_16    ;UL   컴컴 U/ZR V/ZR
                fistp  ULeftFixed    ;컴컴 U/ZR V/ZR



;컴컴 Setting up slopes...
                mov    eax, USlopeFixed
                mov    ebx, eax                 ;Take copy
                sar    eax, 16                  ;Get int part
                sal    ebx, 16                  ;Get fractional part
                mov    UFrac, ebx               ;Save fractional part

                mov    ecx, VSlopeFixed
                mov    edx, ecx
                sar    ecx, 16
                sal    edx, 16
                mov    VFrac, edx

                imul   ecx, T05_MapWidth
                add    ecx, eax
                mov    Slope, ecx              ;Save normal slope
                add    ecx, T05_MapWidth
                mov    [Slope - 4], ecx        ;Save slope + 1 scanline




;컴컴 Setting up starting values...

                mov    eax, ULeftFixed          ;
                add    eax, 8000h               ;add 0.5 (16.16 fixed point)
                mov    ebx, eax                 ;Take copy
                shr    eax, 16                  ;Get int part
                shl    ebx, 16                  ;Get fractional part

                mov    esi, VLeftFixed
                add    esi, 8000h
                mov    ecx, esi
                shr    esi, 16
                shl    ecx, 16
                imul   esi, T05_MapWidth

                add    esi, eax                 ;Add U-offset

                mov    edx, T05_MapOffset
                shr    edx, 1
                add    esi, edx                 ;Add textureoffset

                                                ;esi = mempointer
                                                ;ebx = U fractional part
                                                ;ecx = V fractional part



                mov    edx, LeftNdZ
                mov    ebp, ScanLength
                inc    ebp
                xor    eax, eax

LeftOverLoop:
                cmp    edx, [edi*4 + 12345678h]
OffsXX_A:       jbe    DumpXX
                mov    [edi*4 + 12345678h], edx
OffsXX_B:
                push   ebx
                push   ecx
                mov    ax, [esi*2]

                mov    ebx, 011111b
                and    ebx, eax

                mov    ecx, 011111100000b
                and    ecx, eax
                shr    ecx, 5

                and    eax, 01111100000000000b
                shr    eax, 11

                mov    ax, RLookup[eax*2]
                or     ax, GLookup[ecx*2]
                or     ax, BLookup[ebx*2]

                mov    [edi*2 + 12345678h], ax
OffsXX_C:
                pop    ecx
                pop    ebx
DumpXX:
                add    ecx, VFrac               ;cf set if ecx > 0.5
                lea    edi, [edi + 1]

                sbb    eax, eax                 ;1 if carry
                add    ebx, UFrac               ;cf set if ebx > 0.5

                adc    esi, [Slope + eax*4]     ;increment texturepointer
                add    edx, NZSlopeInt

                dec    ebp
                jnz    LeftOverLoop


Scan_Done:
                ffree  st                       ;Free FPU-stack
                ffree  st(1)
                ffree  st(2)
                ffree  st(3)
                ffree  st(4)
                ffree  st(5)
                ffree  st(6)
                ffree  st(7)
                popad                           ;Restore all general regs
              ENDM









;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : T05_Tri
;쿔D           : T05
;쿟ype         : Procedure
;쿗ast update  : 26.07.1997
;쿌ction       : Draws a perspectivemapped triangel, 32k colours, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [esi]           = Offset Face STRUCT
;�                  T05_MapOffset   = Offset texturemap
;�                  T05_MapWidth    = Mapwidth in pixels (!!! INTEGER !!!)
;�                  T05_MapModifier = Mappingcoordinate-modifer (default=1.0)
;�
;쿙otes: FPU stack must be empty. General registers are destroyed.
;쿔MPORTANT: T05_MapOffset and T05_MapWidth MUST BE SET!!!
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4

XSlopeA         DD 0.0                          ;FP: X-slope edge A
UZSlopeA        DD 0.0                          ;FP: U/Z-slope edge A
VZSlopeA        DD 0.0                          ;FP: V/Z-slope edge A
NZSlopeA        DD 0.0                          ;FP: 1/Z-slope edge A

XSlopeB         DD 0.0                          ;FP: X-slope edge B
UZSlopeB        DD 0.0                          ;FP: U/Z-slope edge B
VZSlopeB        DD 0.0                          ;FP: V/Z-slope edge B
NZSlopeB        DD 0.0                          ;FP: 1/Z-slope edge B

UZ0             DD 0.0                          ;FP: U0/Z
VZ0             DD 0.0                          ;FP: V0/Z
UZ1             DD 0.0                          ;FP: U1/Z
VZ1             DD 0.0                          ;FP: V1/Z
UZ2             DD 0.0                          ;FP: U2/Z
VZ2             DD 0.0                          ;FP: V2/Z

Y0Int           DD 0                            ;INT: Y0 rounded
Y1Int           DD 0                            ;INT: Y1 rounded
Y2Int           DD 0                            ;INT: Y2 rounded

PixelPtr        DD 0                            ;INT: Ptr current scanline

;컴컴 General variables...
UZSlope         DD 0                            ;FP: U/Z slope
VZSlope         DD 0                            ;FP: V/Z slope
NZSlope         DD 0                            ;FP: 1/Z slope
UZSlope16       DD 0                            ;FP: U/Z slope *16
VZSlope16       DD 0                            ;FP: V/Z slope *16
NZSlope16       DD 0                            ;FP: 1/Z slope *16
NZSlopeInt      DD 0                            ;Int: 1/Z slope

EdgeA         Edge ?                            ;Edge A data
EdgeB         Edge ?                            ;Edge B data

;컴컴 Flipped values... (well, sorted that is)
_V0Offs         DD 0                            ;Offset to upper vertex
_V1Offs         DD 0                            ;Offset to middle vertex
_V2Offs         DD 0                            ;Offset to lower vertex

_U0             DD 0.0                          ;FP: Sorted map. coordinates
_V0             DD 0.0
_U1             DD 0.0
_V1             DD 0.0
_U2             DD 0.0
_V2             DD 0.0


T05_Tri       PROC

;컴컴 Load, multiply and locally store mappingcoordinates...
                fld    [esi].F_U0
                fmul   T05_MapModU
                fstp   _U0
                fld    [esi].F_V0
                fmul   T05_MapModV
                fstp   _V0

                fld    [esi].F_U1
                fmul   T05_MapModU
                fstp   _U1
                fld    [esi].F_V1
                fmul   T05_MapModV
                fstp   _V1

                fld    [esi].F_U2
                fmul   T05_MapModU
                fstp   _U2
                fld    [esi].F_V2
                fmul   T05_MapModV
                fstp   _V2



;컴컴 Load offsets to vertices...
;[ebx] = V0
;[ecx] = V1
;[edx] = V2
;
;These pointers (ebx, ecx, edx) are valid until the edgeflip

                mov    ebx, [esi].F_V0Offs
                mov    ecx, [esi].F_V1Offs
                mov    edx, [esi].F_V2Offs




;컴컴 Sorting vertices...
                                      ;st0  st1  st2  st3  st4  st5  st6  st7
                fld    [edx].V_Y2D      ;V2Y
                fld    [ecx].V_Y2D      ;V1Y  V2Y
                fld    [ebx].V_Y2D      ;V0Y  V1Y  V2Y


;컴컴 Check Y0 <-> Y1
                fcom   st(1)          ;V0Y  V1Y  V2Y
                fstsw  ax
                sahf
                jbe    Swapped01

                mov    eax, _U0                 ;Swapping V0 and V1
                mov    edi, _U1
                mov    _U0, edi
                mov    _U1, eax
                mov    eax, _V0
                mov    edi, _V1
                mov    _V0, edi
                mov    _V1, eax

                xchg   ebx, ecx
                fxch   st(1)

Swapped01:




;컴컴 Check Y0 <-> Y2
                fcom   st(2)
                fstsw  ax
                sahf
                jbe    Swapped02

                mov    eax, _U0                 ;Swapping V0 and V2
                mov    edi, _U2
                mov    _U0, edi
                mov    _U2, eax
                mov    eax, _V0
                mov    edi, _V2
                mov    _V0, edi
                mov    _V2, eax


                xchg   ebx, edx
                fxch   st(2)
Swapped02:




;컴컴 Check Y1 <-> Y2
                fxch   st(2)
                fcom   st(1)
                fstsw  ax
                sahf
                jae    Swapped12

                mov    eax, _U1                 ;Swapping V1 and V2
                mov    edi, _U2
                mov    _U1, edi
                mov    _U2, eax
                mov    eax, _V1
                mov    edi, _V2
                mov    _V1, edi
                mov    _V2, eax

                xchg   ecx, edx
                fxch   st(1)
Swapped12:

                mov    _V0Offs, ebx             ;Save verticeoffsets
                mov    _V1Offs, ecx
                mov    _V2Offs, edx


;컴컴 There... Now st(0) > st(1) > st(2)
;                  =edx    =ecx    =ebx
; "Xxxx" means X-something
; "Yxxx" means Y-something
; "xNxx" means 1/xxx
; "xDxx" means delta-something
; "xSxx" means slope-something
; "xxxA" means something with edge A = V0 - V1
; "xxxB" means something with edge B = V0 - V2


                                      ;st0  st1  st2  st3  st4  st5  st6  st7
                                      ;Y2   Y1   Y0
;컴컴 Get 1/YDeltaA and 1/YDeltaB
                fsub   st, st(2)      ;YDB  Y1   Y0
                fadd   AlmostZero
                fxch   st(1)          ;Y1   YDB  Y0
                fsub   st, st(2)      ;YDA  YDB  Y0
                fadd   AlmostZero
                fdivr  One            ;YNDA YDB  Y0
                fxch   st(1)          ;YDB  YNDA Y0
                fdivr  One            ;YNDB YNDA Y0
                ffree  st(2)          ;YNDB YNDA



;컴컴 Get U/ZSlope A...
                fld    _U1            ;U1   YNDB YNDA
                fmul   [ecx].V_InvZ     ;UZ1  YNDB YNDA
                fst    UZ1            ;UZ1  YNDB YNDA
                fld    _U0            ;U0   UZ1  YNDB YNDA
                fmul   [ebx].V_InvZ     ;UZ0  UZ1  YNDB YNDA
                fst    UZ0            ;UZ0  UZ1  YNDB YNDA
                fsubp  st(1), st      ;UZDA YNDB YNDA
                fmul   st, st(2)      ;UZSA YNDB YNDA
                fstp   UZSlopeA       ;YNDB YNDA


;컴컴 Get V/ZSlope A...
                fld    _V1            ;V1   YNDB YNDA
                fmul   [ecx].V_InvZ     ;VZ1  YNDB YNDA
                fst    VZ1            ;VZ1  YNDB YNDA
                fld    _V0            ;V0   VZ1  YNDB YNDA
                fmul   [ebx].V_InvZ     ;VZ0  VZ1  YNDB YNDA
                fst    VZ0            ;VZ0  VZ1  YNDB YNDA
                fsubp  st(1), st      ;VZDA YNDB YNDA
                fmul   st, st(2)      ;VZSA YNDB YNDA
                fstp   VZSlopeA       ;YNDB YNDA


;컴컴 Get U/ZSlope B...
                fld    _U2            ;U2   YNDB YNDA
                fmul   [edx].V_InvZ     ;UZ2  YNDB YNDA
                fst    UZ2            ;UZ2  YNDB YNDA
                fsub   UZ0            ;UZDB YNDB YNDA
                fmul   st, st(1)      ;UZSB YNDB YNDA
                fstp   UZSlopeB       ;YNDB YNDA


;컴컴 Get V/ZSlope B...
                fld    _V2            ;V2   YNDB YNDA
                fmul   [edx].V_InvZ     ;VZ2  YNDB YNDA
                fst    VZ2            ;VZ2  YNDB YNDA
                fsub   VZ0            ;VZDB YNDB YNDA
                fmul   st, st(1)      ;VZSB YNDB YNDA
                fstp   VZSlopeB       ;YNDB YNDA


;컴컴 Get 1/ZSlope A...
                fld    [ecx].V_InvZ     ;NZ1  YNDB YNDA
                fsub   [ebx].V_InvZ     ;NZDA YNDB YNDA
                fmul   st, st(2)      ;NZSA YNDB YNDA
                fstp   NZSlopeA       ;YNDB YNDA

;컴컴 Get 1/ZSlope B...
                fld    [edx].V_InvZ     ;NZ2  YNDB YNDA
                fsub   [ebx].V_InvZ     ;NZDB YNDB YNDA
                fmul   st, st(1)      ;NZSB YNDB YNDA
                fstp   NZSlopeB       ;YNDB YNDA


;컴컴 Get XSlope A and B...
                fld    [ebx].V_X2D      ;X0   YNDB YNDA
                fld    [ecx].V_X2D      ;X1   X0   YNDB YNDA
                fsub   st, st(1)      ;XDA  X0   YNDB YNDA
                fmul   st, st(3)      ;XSA  X0   YNDB YNDA
                fstp   XSlopeA        ;X0   YNDB YNDA

                fld    [edx].V_X2D      ;X2   X0   YNDB YNDA
                fsub   st, st(1)      ;XDB  X0   YNDB YNDA
                fmul   st, st(2)      ;XSB  X0   YNDB YNDA
                fstp   XSlopeB        ;X0   YNDB YNDA

                ffree  st
                ffree  st(1)
                ffree  st(2)

;컴컴 Get general U/ZSlope, V/ZSlope and 1/ZSlope...
;xMxx = value at middle of edge B
                fld    [ecx].V_Y2D      ;Y1
                fld    [ebx].V_Y2D      ;Y0   Y1
                fsubp  st(1), st      ;YDA

                fld    XSlopeB        ;XSB  YDA
                fmul   st, st(1)      ;XM0B YDA
                fadd   [ebx].V_X2D      ;XMB  YDA
                fsub   [ecx].V_X2D      ;XMD  YDA
                fdivr  One            ;XNMD YDA

                fld    UZSlopeB       ;UZSB XNMD YDA
                fmul   st, st(2)      ;UZMB XNMD YDA
                fadd   UZ0            ;UZMB XNMD YDA
                fsub   UZ1            ;UZMD XNMD YDA
                fmul   st, st(1)      ;UZS  XNMD YDA
                fst    UZSlope        ;UZS  XNMD YDA
                fmul   Sixteen        ;UZS  XNMD YDA
                fstp   UZSlope16      ;XNMD YDA

                fld    VZSlopeB       ;VZSB XNMD YDA
                fmul   st, st(2)      ;VZMB XNMD YDA
                fadd   VZ0            ;VZMB XNMD YDA
                fsub   VZ1            ;VZMD XNMD YDA
                fmul   st, st(1)      ;VZS  XNMD YDA
                fst    VZSlope        ;VZS  XNMD YDA
                fmul   Sixteen        ;VZS  XNMD YDA
                fstp   VZSlope16      ;XNMD YDA

                fld    NZSlopeB       ;NZSB XNMD YDA
                fmul   st, st(2)      ;NZMB XNMD YDA
                fadd   [ebx].V_InvZ     ;NZMB XNMD YDA
                fsub   [ecx].V_InvZ     ;NZMD XNMD YDA
                fmul   st, st(1)      ;NZS  XNMD YDA
                fst    NZSlope        ;NZS  XNMD YDA
                fld    st             ;NZS  NZS  XNMD YDA
                fmul   Fixed032       ;NZS  NZS  XNMD YDA
                fistp  NZSlopeInt     ;NZS  XNMD YDA
                fmul   Sixteen        ;NZS  XNMD YDA
                fstp   NZSlope16      ;XNMD YDA

                ffree  st(1)          ;XNMD
                ffree  st             ;empty

;컴컴 Rounding and clipping Y-values...
                fldcw  FPU_RoundUp

                fld    [ebx].V_Y2D              ;Clipping Y0
                fcom   SYS_FPClipTop
                fstsw  ax
                sahf
                jae    NoClipTop0
                fstp   FPDump
                fld    SYS_FPClipTop
NoClipTop0:

                fcom   SYS_FPClipBottom
                fstsw  ax
                sahf
                jbe    NoClipBottom0
                fstp   FPDump
                fld    SYS_FPClipBottom
		fadd	One
NoClipBottom0:
                fistp  Y0Int                    ;Storing clipped Y0


                fld    [ecx].V_Y2D              ;Clipping Y1
                fcom   SYS_FPClipTop
                fstsw  ax
                sahf
                jae    NoClipTop1
                fstp   FPDump
                fld    SYS_FPClipTop
NoClipTop1:

                fcom   SYS_FPClipBottom
                fstsw  ax
                sahf
                jb     NoClipBottom1
                fstp   FPDump
                fld    SYS_FPClipBottom
		fadd	One
NoClipBottom1:
                fistp  Y1Int                    ;Storing clipped Y1



                fld    [edx].V_Y2D              ;Clipping Y2
                fcom   SYS_FPClipTop
                fstsw  ax
                sahf
                jae    NoClipTop2
                fstp   FPDump
                fld    SYS_FPClipTop
NoClipTop2:

                fcom   SYS_FPClipBottom
                fstsw  ax
                sahf
                jb     NoClipBottom2
                fstp   FPDump
                fld    SYS_FPClipBottom
		fadd	One
NoClipBottom2:
                fistp  Y2Int                    ;Storing Y2


                fild   Y0Int
                fcom   SYS_FPClipTop
                fstsw  ax
                sahf
                jae    NoTopClip
                fstp   FPDump
                fld    SYS_FPClipTop

NoTopClip:

;컴컴 Clipping edge A to next scanline...
                fsub   [ebx].V_Y2D      ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebx].V_X2D      ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   UZSlopeA       ;UZDA YD
                fadd   UZ0            ;UZA  YD
                fstp   EdgeA.UdZ      ;YD

                fld    st             ;YD   YD
                fmul   VZSlopeA       ;VZDA YD
                fadd   VZ0            ;VZA  YD
                fstp   EdgeA.VdZ      ;YD

                fld    st             ;YD   YD
                fmul   NZSlopeA       ;NZDA YD
                fadd   [ebx].V_InvZ     ;NZA  YD
                fstp   EdgeA.NdZ      ;YD



;컴컴 Clipping edge B to next scanline...
                fld    st             ;YD   YD
                fmul   XSlopeB        ;XDB  YD
                fadd   [ebx].V_X2D      ;XB   YD
                fstp   EdgeB.X        ;YD

                fld    st             ;YD   YD
                fmul   UZSlopeB       ;UZDB YD
                fadd   UZ0            ;UZB  YD
                fstp   EdgeB.UdZ      ;YD

                fld    st             ;YD   YD
                fmul   VZSlopeB       ;VZDB YD
                fadd   VZ0            ;VZB  YD
                fstp   EdgeB.VdZ      ;YD

                fmul   NZSlopeB       ;NZDA
                fadd   [ebx].V_InvZ     ;NZA
                fstp   EdgeB.NdZ

;컴컴 Calculating memoffset
                fild   Y0Int          ;Y0I
                fild   SYS_XRes       ;SLS  Y0I
                fmulp  st(1), st      ;Offs Y0I
                fistp  PixelPtr       ;Y0I



;컴컴 Setup eax and ebx as edgepointers...
;[eax] = left edge
;[ebx] = right edge

                mov    edx, OFFSET EdgeA        ;Use edx temporary
                mov    ebx, OFFSET EdgeB

                fld    XSlopeB
                fld    XSlopeA
                fcom   st(1)
                fstsw  ax
                sahf
                jbe    NoFlipAB

                xchg   edx, ebx
NoFlipAB:

                ffree  st
                ffree  st(1)

                mov    eax, edx                 ;Use eax as pointer to edge A
                xor    edx, edx

                mov    ecx, Y1Int               ;Get number of lines to draw
                sub    ecx, Y0Int

                                                ;[eax] = left edge
                                                ;[ebx] = right edge
                                                ; ecx  = loopcounter
                                                ; edx  = flag


ScanLoop:
                or     ecx, ecx
                jz     DoEdge12


                T05_ScanLine


;旼컴컴컴컴컴컴컴컴컴� OPTIMIZED BLOCK D (PL_Dx.TXT) 컴컴컴컴컴컴컴컴컴컴컴컴
;�* 32 cycles
;�* Updating variables for both edges
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
                                        ;st0  st1  st2  st3  st4  st5  st6  st7

                fld    EdgeA.X     ;vr0  XA
                fadd   XSlopeA     ;vr0  XAn
                fld    EdgeA.UdZ   ;vr1  UZA  XAn
                fadd   UZSlopeA    ;vr1  UZAn XAn
                fxch   st(1)       ;     XAn  UZAn
                fld    EdgeA.VdZ   ;vr2  VZA  XAn UZAn
                fxch   st(1)       ;     XAn  VZA UZAn
                fstp   EdgeA.X     ;vr0  VZA  UZAn
                fadd   VZSlopeA    ;vr2  VZAn UZAn
                fxch   st(1)       ;     UZAn VZAn
                fstp   EdgeA.UdZ   ;vr1  VZAn
                fld    EdgeA.NdZ   ;vr3  NZA  VZAn
                fadd   NZSlopeA    ;vr3  NZAn VZAn
                fxch   st(1)       ;     VZAn NZAn
                fstp   EdgeA.VdZ   ;vr2  NZAn
                fld    EdgeB.X     ;vr4  XB   NZAn
                fadd   XSlopeB     ;vr4  XBn  NZAn
                fxch   st(1)       ;     NZAn XBn
                fstp   EdgeA.NdZ   ;vr3  XBn
                fld    EdgeB.UdZ   ;vr5  UZB  XBn
                fadd   UZSlopeB    ;vr5  UZBn XBn
                fxch   st(1)       ;     XBn  UZBn
                fstp   EdgeB.X     ;vr4  UZBn
                fld    EdgeB.VdZ   ;vr6  VZB  UZBn
                fadd   VZSlopeB    ;vr6  VZBn UZBn
                fxch   st(1)       ;     UZBn VZBn
                fstp   EdgeB.UdZ   ;vr5  VZBn
                fld    EdgeB.NdZ   ;vr7  NZB  VZBn
                fadd   NZSlopeB    ;vr7  NZBn VZBn
                fxch   st(1)       ;     VZBn NZBn
                fstp   EdgeB.VdZ   ;vr6  NZBn
                fstp   EdgeB.NdZ   ;vr7  empty

                dec    ecx                   ;U:1
                mov    ebp, SYS_XRes         ;V:1

                add    PixelPtr, ebp       ;U:2-3
                jmp    ScanLoop              ;V:2


DoEdge12:       or     edx, edx
                jnz    Done

                mov    ebp, _V1Offs
                mov    edi, _V2Offs

                                                ;FPU stack is empty

;컴컴 Get 1/DeltaY...
                fld    [edi].V_Y2D      ;Y2
                fsub   [ebp].V_Y2D      ;YD
                fdivr  One            ;YNDA


;컴컴 Get new U/ZSlope A...
                fld    UZ2            ;UZ2  YNDA
                fsub   UZ1            ;UZDA YNDA
                fmul   st, st(1)      ;UZSA YNDA
                fstp   UZSlopeA       ;YNDA


;컴컴 Get new V/ZSlope A...
                fld    VZ2            ;VZ2  YNDA
                fsub   VZ1            ;VZDA YNDA
                fmul   st, st(1)      ;VZSA YNDA
                fstp   VZSlopeA       ;YNDA


;컴컴 Get new 1/ZSlope A...
                fld    [edi].V_InvZ     ;NZ2  YNDA
                fsub   [ebp].V_InvZ     ;NZDA YNDA
                fmul   st, st(1)      ;NZSA YNDA
                fstp   NZSlopeA       ;YNDA

;컴컴 Get new XSlopeA...
                fld    [edi].V_X2D      ;X2   YNDA
                fsub   [ebp].V_X2D      ;XDA  YNDA
                fmul   st, st(1)      ;XSA  YNDA
                fstp   XSlopeA
                ffree  st


;컴컴 Clipping edge C to next scanline and setting initial values...

                fild   Y1Int          ;Y1I
                fsub   [ebp].V_Y2D      ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebp].V_X2D      ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   UZSlopeA       ;UZDA YD
                fadd   UZ1            ;UZA  YD
                fstp   EdgeA.UdZ      ;YD

                fld    st             ;YD   YD
                fmul   VZSlopeA       ;VZDA YD
                fadd   VZ1            ;VZA  YD
                fstp   EdgeA.VdZ      ;YD

                fmul   NZSlopeA       ;NZDA
                fadd   [ebp].V_InvZ     ;NZA
                fstp   EdgeA.NdZ

                inc    edx            ;Set flag
                mov    ecx, Y2Int
                sub    ecx, Y1Int
                jmp    ScanLoop


Done:
                ret
T05_Tri       ENDP



T05_Init      PROC
                mov    eax, SYS_ZBufOffs

                mov    DWORD PTR [OffsXX_A - 4], eax
                mov    DWORD PTR [OffsXX_B - 4], eax

                mov    DWORD PTR [Offs00_A - 4], eax
                mov    DWORD PTR [Offs00_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs01_A - 4], eax
                mov    DWORD PTR [Offs01_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs02_A - 4], eax
                mov    DWORD PTR [Offs02_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs03_A - 4], eax
                mov    DWORD PTR [Offs03_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs04_A - 4], eax
                mov    DWORD PTR [Offs04_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs05_A - 4], eax
                mov    DWORD PTR [Offs05_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs06_A - 4], eax
                mov    DWORD PTR [Offs06_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs07_A - 4], eax
                mov    DWORD PTR [Offs07_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs08_A - 4], eax
                mov    DWORD PTR [Offs08_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs09_A - 4], eax
                mov    DWORD PTR [Offs09_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs10_A - 4], eax
                mov    DWORD PTR [Offs10_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs11_A - 4], eax
                mov    DWORD PTR [Offs11_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs12_A - 4], eax
                mov    DWORD PTR [Offs12_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs13_A - 4], eax
                mov    DWORD PTR [Offs13_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs14_A - 4], eax
                mov    DWORD PTR [Offs14_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs15_A - 4], eax
                mov    DWORD PTR [Offs15_B - 4], eax



;컴컴 Set DBufOffs (edi)
                mov    eax, SYS_DBufOffs

                mov    DWORD PTR [OffsXX_C - 4], eax

                mov    DWORD PTR [Offs00_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs01_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs02_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs03_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs04_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs05_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs06_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs07_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs08_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs09_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs10_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs11_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs12_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs13_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs14_C - 4], eax

                add    eax, 2
                mov    DWORD PTR [Offs15_C - 4], eax

                ret
T05_Init      ENDP

END
