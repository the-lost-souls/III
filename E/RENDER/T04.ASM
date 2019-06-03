;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T04_ORG.ASM
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 10.02.1998
;쿌ction       : Draws a flat shaded triangle, 32k colors, z-buffer
;�
;쿙otes :
;쿟o be done:
;�
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model Flat, C
.code

include w:\general\var.inc

public T04_Tri
public T04_Init
public T04_Call
public T04_FlatColor

ALIGN 4


Edge          STRUC

X               DD ?
NdZ             DD ?

Edge          ENDS


;컴컴 In-variables...
T04_FlatColor	dw ?
		dw ?


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

ALIGN 4

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

ALIGN 4


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











;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T04_Call
;쿔D           : T04
;쿟ype         : Procedure
;쿗ast update  : 10.02.1998
;쿌ction       : Runs z-clipping, then draws
;쿚ptimized    : No
;�
;쿔nput variables : [esi]  = polygon
;�
;쿙otes : FPU stack must be empty.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4

ColorRange      DD 31.0
Color           DD 0.0

T04_Call      PROC
;---- Getting color...
                mov    ebp, [esi].F_Material

                fldcw  FPU_Round

                fld    [esi].F_IntensityR
                fmul   [ebp].M_R
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRange
                fcom   ColorRange
                fstsw  ax
                sahf
                jb     NoRClip
                ffree
                fld    ColorRange
NoRClip:
                fistp  Color

                mov    ebx, Color

                fld    [esi].F_IntensityG
                fmul   [ebp].M_G
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRange
                fcom   ColorRange
                fstsw  ax
                sahf
                jb     NoGClip
                ffree
                fld    ColorRange
NoGClip:
                fistp  Color

                mov    ecx, Color

                fld    [esi].F_IntensityB
                fmul   [ebp].M_B
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fmul   ColorRange
                fcom   ColorRange
                fstsw  ax
                sahf
                jb     NoBClip
                ffree
                fld    ColorRange
NoBClip:
                fistp  Color

                mov    edx, Color

                shl    ecx, 5
                shl    edx, 10
                or     ebx, ecx
                or     ebx, edx
                mov    T04_FlatColor, bx        ;Save fill-word


;---- Z-clipping?
                cmp    [esi].F_ZFlag, 0
                je     NoZClip

                call   ZClip

                cmp    [esi].F_ZFlag, 2         ;Draw to/one polygon?
                je     DrawOne                  ;Draw one!

                mov    esi, OFFSET NewFace1
                call   T04_Tri

DrawOne:
                mov    esi, OFFSET NewFace0
NoZClip:
                call   T04_Tri

                ret
T04_Call      ENDP





;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T04_Scanline
;쿔D           : T04
;쿟ype         : Macro
;쿗ast update  : 12.07.1997
;쿌ction       : Draws a flat-filled, horisontal scanline, 32k, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [eax]  = left edge
;�                  [ebx]  = right edge
;�
;쿙otes : FPU stack must be empty. All integer registers are saved.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4

ScanLength      DD 0                            ;int scanline length

EdgeRight     Edge ?                            ;Right edge data
EdgeLeft      Edge ?                            ;Left edge data

LeftNdZ         DD 0

T04_ScanLine  PROC
                                     ;st0  st1  st2  st3  st4  st5  st6  st7


;컴컴 Clip 1/Z-right to last pixel...
                fldcw  FPU_RoundDown

                push   eax
                fld    [ebx].X       ;XR
                fcom   SYS_FPClipRight
                fstsw  ax
                sahf
                jb     NoRightClip
                ffree  st
                fld    SYS_FPClipRight

NoRightClip:
                pop    eax

                fistp  EdgeRight.X   ;
                fild   EdgeRight.X   ;XRI
                fsub   [ebx].X       ;XD

                fmul   NZSlope       ;XDU
                fadd   [ebx].NdZ     ;UZR
                fstp   EdgeRight.NdZ ;


;컴컴 Clip 1/Z left to next pixel...
                fldcw  FPU_RoundUp

                push   eax
                fld    [eax].X       ;XL
                fcom   SYS_FPClipLeft
                fstsw  ax
                sahf
                ja     NoLeftClip
                ffree  st
                fld    SYS_FPClipLeft

NoLeftClip:
                pop    eax

                fistp  EdgeLeft.X    ;
                fild   EdgeLeft.X    ;XL
                fsub   [eax].X       ;XD

                fmul   NZSlope       ;XNZ
                fadd   [eax].NdZ     ;NZL


;컴컴 Save fixed point 1/Z left
                fmul   Fixed032      ;1/ZL
                fistp  LeftNdZ       ;

                pushad                          ;Save all registers

                mov    edi, PixelPtr            ;Use edi as mempointer
                add    edi, EdgeLeft.X

                mov    eax, EdgeLeft.X          ;Calculate scanline length
                mov    ebx, EdgeRight.X         ;/
                sub    ebx, eax                 ;ebx = scanline length
                mov    ScanLength, ebx

                cmp    ebx, 0
                jl     Scan_Done
		cmp    ebx, 16
		jb     LeftOver1
		jmp    LeftOver1


		mov     ax, T04_FlatColor
		mov    edx, LeftNdZ
		mov    ecx, NZSlopeInt

;eax = fill-word (R:G:B, 5:5:5:1
;ecx = 1/Z fixed point slope
;edx = 1/Z interpolation
;edi = buffer pointer

;-------------------------- LOOP STARTS HERE -------------------------------


;컴컴 Pixel 0

Loop16:

                cmp    edx, [edi*4 + 12345678h]
Offs00_A:       jbe    Dump0

                mov    [edi*4 + 12345678h], edx
Offs00_B:
                mov    [edi*2 + 12345678h], ax
Offs00_C:
Dump0:

;컴컴 Pixel 1
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]

Offs01_A:       jbe    Dump1

                mov    [edi*4 + 12345678h], edx
Offs01_B:
                mov    [edi*2 + 12345678h], ax
Offs01_C:
Dump1:

;컴컴 Pixel 2
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs02_A:       jbe    Dump2

                mov    [edi*4 + 12345678h], edx
Offs02_B:
                mov    [edi*2 + 12345678h], ax
Offs02_C:
Dump2:
;컴컴 Pixel 3
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs03_A:       jbe    Dump3

                mov    [edi*4 + 12345678h], edx
Offs03_B:
                mov    [edi*2 + 12345678h], ax
Offs03_C:
Dump3:
;컴컴 Pixel 4
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs04_A:       jbe    Dump4

                mov    [edi*4 + 12345678h], edx
Offs04_B:
                mov    [edi*2 + 12345678h], ax
Offs04_C:

Dump4:
;컴컴 Pixel 5
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs05_A:       jbe    Dump5

                mov    [edi*4 + 12345678h], edx
Offs05_B:
                mov    [edi*2 + 12345678h], ax
Offs05_C:

Dump5:
;컴컴 Pixel 6
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs06_A:       jbe    Dump6

                mov    [edi*4 + 12345678h], edx
Offs06_B:
                mov    [edi*2 + 12345678h], ax
Offs06_C:

Dump6:
;컴컴 Pixel 7
                add    edx, ecx
                cmp    edx, [edi*4 + 12345678h]
Offs07_A:       jbe    Dump7

                mov    [edi*4 + 12345678h], edx
Offs07_B:
                mov    [edi*2 + 12345678h], ax
Offs07_C:

Dump7:
;컴컴 Pixel 8
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs08_A:       jbe    Dump8
                mov    [edi*4 + 12345678h], edx
Offs08_B:
                mov    [edi*2 + 12345678h], ax
Offs08_C:

Dump8:
;컴컴 Pixel 9
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs09_A:       jbe    Dump9

                mov    [edi*4 + 12345678h], edx
Offs09_B:
                mov    [edi*2 + 12345678h], ax
Offs09_C:

Dump9:
;컴컴 Pixel 10
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs10_A:       jbe    Dump10

                mov    [edi*4 + 12345678h], edx
Offs10_B:
                mov    [edi*2 + 12345678h], ax
Offs10_C:

Dump10:
;컴컴 Pixel 11
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs11_A:       jbe    Dump11

                mov    [edi*4 + 12345678h], edx
Offs11_B:
                mov    [edi*2 + 12345678h], ax
Offs11_C:

Dump11:
;컴컴 Pixel 12
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs12_A:       jbe    Dump12

                mov    [edi*4 + 12345678h], edx
Offs12_B:
                mov    [edi*2 + 12345678h], ax
Offs12_C:

Dump12:
;컴컴 Pixel 13
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs13_A:       jbe    Dump13

                mov    [edi*4 + 12345678h], edx
Offs13_B:
                mov    [edi*2 + 12345678h], ax
Offs13_C:

Dump13:
;컴컴 Pixel 14
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs14_A:       jbe    Dump14

                mov    [edi*4 + 12345678h], edx
Offs14_B:
                mov    [edi*2 + 12345678h], ax
Offs14_C:

Dump14:
;컴컴 Pixel 15
                add    edx, ecx

                cmp    edx, [edi*4 + 12345678h]
Offs15_A:       jbe    Dump15

                mov    [edi*4 + 12345678h], edx
Offs15_B:
                mov    [edi*2 + 12345678h], ax
Offs15_C:

Dump15:
                add    edx, ecx

                mov    LeftNdZ, edx

                add    edi, 16                  ;Move memory pointer
                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                jbe    LeftOver1
                jmp    Loop16



;컴컴 Write leftover pixels...

LeftOver1:

                mov    edx, LeftNdZ
		mov     ax, T04_FlatColor
		mov    ecx, NZSlopeInt

                mov    ebp, ScanLength
                inc    ebp

LeftOverLoop:
                cmp    edx, [edi*4 + 12345678h]
OffsXX_A:       jbe    DumpXX
                mov    [edi*4 + 12345678h], edx
OffsXX_B:

                mov    [edi*2 + 12345678h], ax
OffsXX_C:

DumpXX:

                lea    edi, [edi + 1]

                add    edx, ecx

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
		ret
T04_ScanLine  ENDP









;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : T04_Tri
;쿔D           : T04
;쿟ype         : Procedure
;쿗ast update  : 16.09.1997
;쿌ction       : Draws a flat shaded triangel, 32k colours, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [esi]           = Offset Face STRUCT
;�                  T04_FlatColor   = Fillword (color)
;�
;쿙otes: FPU stack must be empty. General registers are destroyed.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4

XSlopeA         DD 0.0                          ;FP: X-slope edge A
NZSlopeA        DD 0.0                          ;FP: 1/Z-slope edge A

XSlopeB         DD 0.0                          ;FP: X-slope edge B
NZSlopeB        DD 0.0                          ;FP: 1/Z-slope edge B

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


T04_Tri       PROC


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

                xchg   ebx, ecx
                fxch   st(1)

Swapped01:




;컴컴 Check Y0 <-> Y2
                fcom   st(2)
                fstsw  ax
                sahf
                jbe    Swapped02

                xchg   ebx, edx
                fxch   st(2)
Swapped02:




;컴컴 Check Y1 <-> Y2
                fxch   st(2)
                fcom   st(1)
                fstsw  ax
                sahf
                jae    Swapped12

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



;컴컴 Get general 1/ZSlope...
;xMxx = value at middle of edge B
                fld    [ecx].V_Y2D    ;Y1
                fld    [ebx].V_Y2D    ;Y0   Y1
                fsubp  st(1), st      ;YDA

                fld    XSlopeB        ;XSB  YDA
                fmul   st, st(1)      ;XM0B YDA
                fadd   [ebx].V_X2D    ;XMB  YDA
                fsub   [ecx].V_X2D    ;XMD  YDA
                fdivr  One            ;XNMD YDA

                fld    NZSlopeB       ;NZSB XNMD YDA
                fmul   st, st(2)      ;NZMB XNMD YDA
                fadd   [ebx].V_InvZ   ;NZMB XNMD YDA
                fsub   [ecx].V_InvZ   ;NZMD XNMD YDA
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
                jbe    NoClipBottom1
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
                jbe    NoClipBottom2
                fstp   FPDump
                fld    SYS_FPClipBottom
NoClipBottom2:
                fistp  Y2Int                    ;Storing Y2


                fild   Y0Int
;컴컴 Clipping edge A to next scanline...

                fsub   [ebx].V_Y2D    ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebx].V_X2D    ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   NZSlopeA       ;NZDA YD
                fadd   [ebx].V_InvZ   ;NZA  YD
                fstp   EdgeA.NdZ      ;YD

;컴컴 Clipping edge B to next scanline...
                fld    st             ;YD   YD
                fmul   XSlopeB        ;XDB  YD
                fadd   [ebx].V_X2D      ;XB   YD
                fstp   EdgeB.X        ;YD

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

                call T04_ScanLine


SkipScanLine1:

;컴컴 Update edge A...
                fld    EdgeA.X
                fadd   XSlopeA
                fstp   EdgeA.X

                fld    EdgeA.NdZ
                fadd   NZSlopeA
                fstp   EdgeA.NdZ



;컴컴 Update edge B...
                fld    EdgeB.X
                fadd   XSlopeB
                fstp   EdgeB.X

                fld    EdgeB.NdZ
                fadd   NZSlopeB
                fstp   EdgeB.NdZ

                mov    ebp, SYS_XRes
                add    PixelPtr, ebp

                dec    ecx
                jmp    ScanLoop


DoEdge12:       or     edx, edx
                jnz    Done

                mov    ebp, _V1Offs
                mov    edi, _V2Offs

                                                ;FPU stack is empty
;컴컴 Get 1/DeltaY...
                fld    [edi].V_Y2D      ;Y2
                fsub   [ebp].V_Y2D      ;YD
                fdivr  One            ;YNDA



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
                fsub   [ebp].V_Y2D    ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebp].V_X2D      ;XA   YD
                fstp   EdgeA.X        ;YD

                fmul   NZSlopeA       ;NZDA
                fadd   [ebp].V_InvZ     ;NZA
                fstp   EdgeA.NdZ

                inc    edx            ;Set flag
                mov    ecx, Y2Int
                sub    ecx, Y1Int
                jmp    ScanLoop


Done:
		ffree	st(0)
		ffree	st(1)
		ffree	st(2)
		ffree	st(3)
		ffree	st(4)
		ffree	st(5)
		ffree	st(6)
		ffree	st(7)

                ret
T04_Tri       ENDP



T04_Init      PROC
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
T04_Init      ENDP

END
