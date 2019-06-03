;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T07_ORG.ASM
;쿌uthor       : Cyberfish/TLS
;쿗ast update  : 07.01.99
;쿌ction       : Draws a linearly mapped triangle, 32 bits colors, z-buffer
;�
;쿙otes :
;쿟o be done: Should fix the z-clipping. Not necessary with U/V/z-clipping...
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model flat, C
.code

include w:\general\var.inc

public T07_Tri
public T07_Init
public T07_Call

Edge          STRUC

X               DD ?
U               DD ?
V               DD ?
NdZ             DD ?

Edge          ENDS


;컴컴 In-variables...
T07_MapModifier DD 1.0                          ;FP: Modifier, mappingcoordinates
T07_MapOffset   DD 0                            ;Offset texturemap
T07_MapWidth    DD 320                          ;INT: Tmap width, in pixels


;컴컴 Locally used "constants"
Fixed16_16      DD 65536.0
Fixed16         DD 4096.0                       ;65536.0/ 16.0
;Fixed032        DD 2147483647.0
Sixteen         DD 16.0
One             DD 1.0
AlmostZero      DD 0.000000000000000000001

FPDump          DD 0







;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : ZClip
;쿔D           :
;쿟ype         : Macro
;쿗ast update  : 12.07.1997
;쿌ction       : Z-clips a polygon pointed to by edi
;쿚ptimized    : ?
;�
;쿔nput variables : [eax]  = left edge
;�                  [ebx]  = right edge
;�
;쿙otes : Macros "Project_Vertex" and "Clip_Line" are macros used by
;퀃his routine.
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






;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T07_Call
;쿔D           : T07
;쿟ype         : Procedure
;쿗ast update  : 03.10.1997
;쿌ction       : Runs z-clipping, then draws
;쿚ptimized    : No
;�
;쿔nput variables : [esi] = polygon
;�
;쿙otes : FPU stack must be empty.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

;esi = polygon
T07_Call      PROC
                mov	eax, [esi].F_Material

		mov	ebx, [eax].M_MapWidth
                mov	T07_MapWidth, ebx
                mov    	ebx, [eax].M_MapOffset
                mov    	T07_MapOffset, ebx

                cmp    	[esi].F_ZFlag, 0
                je     	NoZClip

                push   	esi
                mov    	ecx, F_Size/4            ;Copy polys
                mov    	edi, OFFSET NewFace0
                rep    	movsd

                mov    	esi, OFFSET NewFace0
                mov    	edi, OFFSET NewFace1
                mov    	ecx, F_Size/4
                rep    	movsd

                pop    	edi                      ;edi = face

                call	ZClip

                mov    	esi, OFFSET NewFace1
                cmp    	[esi].F_ZFlag, 2         ;Draw To/one polygon?
                je     	DrawOne                  ;Draw one!
                call   	T07_Tri

DrawOne:
                mov    	esi, OFFSET NewFace0
NoZClip:
                call   	T07_Tri

                ret
T07_Call      ENDP






;----------------------------------------------------------------------------














;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T07_Scanline
;쿔D           : T07
;쿟ype         : Macro
;쿗ast update  : 12.12.1998
;쿌ction       : Draws a linearly mapped, horisontal scanline, 32k, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [eax]  = left edge
;�                  [ebx]  = right edge
;�
;쿙otes : FPU stack must be empty. All integer registers are saved.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

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

T07_ScanLine  MACRO
                                     ;st0  st1  st2  st3  st4  st5  st6  st7

;컴컴 Clip U/Z, V/Z and 1/Z right To last pixel...
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
                fmul   USlope        ;XDU  XD
                fadd   [ebx].U       ;UR   XD
                fstp   EdgeRight.U   ;XD

                fld    st            ;XD   XD
                fmul   VSlope        ;XDV  XD
                fadd   [ebx].V       ;VR   XD
                fstp   EdgeRight.V   ;XD

                fmul   NZSlope       ;XDZ
                fadd   [ebx].NdZ     ;NZR
                fstp   EdgeRight.NdZ ;


;컴컴 Clip U, V and 1/Z left To next pixel...
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
                fmul   USlope        ;XDU  XD
                fadd   [eax].U       ;UL   XD
		fmul   Fixed16_16    ;ULF  XD
		fistp  ULeftFixed    ;XD

                fld    st            ;XD   XD
                fmul   VSlope        ;XDV  XD
                fadd   [eax].V       ;VL   XD
		fmul   Fixed16_16    ;VLF  XD
		fistp  VLeftFixed    ;XD

                fmul   NZSlope       ;XNZ
                fadd   [eax].NdZ     ;NZL
                fmul   SYS_ZBufFP    ;NZLF
                fistp  LeftNdZ



;컴컴 Doing different integer-stuff... :)
                pushad                          ;Save all registers

		mov    eax, SYS_ZBufAdd
		add    LeftNdZ, eax

                mov    edi, PixelPtr            ;Use edi as mempointer
                add    edi, EdgeLeft.X

                mov    eax, EdgeLeft.X          ;Calculate scanline length
                mov    ebp, EdgeRight.X         ;/
                sub    ebp, eax                 ;ebx = scanline length
                mov    ScanLength, ebp

                cmp    ebp, 0
                jl     Scan_Done

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
                imul   esi, T07_MapWidth

                add    esi, eax                 ;Add U-offset

                mov    edx, T07_MapOffset
                shr    edx, 2
                add    esi, edx                 ;Add textureoffset

                mov    edx, LeftNdZ

                cmp    ebp, 16                  ;Run lefToverloop now?
                jbe    LeftOver

                mov    ebp, UFrac


;-------------------------- LOOP STARTS HERE -------------------------------

Loop16:


;eax = generally used
;ebx = u-fraction
;ecx = v-fraction
;edx = 1/Z interpolation
;esi = tmap pointer
;edi = dbuf pointer
;ebp = u-fraction increase


;컴컴 Pixel 0

                cmp    edx, [edi*4 + 12345678h]
Offs00_A:       jb     Dump0

                mov    [edi*4 + 12345678h], edx
Offs00_B:
                mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs00_C:
Dump0:

;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax
                add    ebx, ebp

                adc    esi, [Slope + eax*4]
                cmp    edx, [edi*4 + 12345678h]

Offs01_A:       jb     Dump1

                mov    [edi*4 + 12345678h], edx
Offs01_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs01_C:
Dump1:

;컴컴 Pixel 2
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs02_A:       jb     Dump2

                mov    [edi*4 + 12345678h], edx
Offs02_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs02_C:
Dump2:
;컴컴 Pixel 3
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs03_A:       jb     Dump3

                mov    [edi*4 + 12345678h], edx
Offs03_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs03_C:
Dump3:
;컴컴 Pixel 4
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs04_A:       jb     Dump4

                mov    [edi*4 + 12345678h], edx
Offs04_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs04_C:

Dump4:
;컴컴 Pixel 5
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs05_A:       jb     Dump5

                mov    [edi*4 + 12345678h], edx
Offs05_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs05_C:

Dump5:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs06_A:       jb     Dump6

                mov    [edi*4 + 12345678h], edx
Offs06_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs06_C:

Dump6:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]
                cmp    edx, [edi*4 + 12345678h]
Offs07_A:       jb     Dump7

                mov    [edi*4 + 12345678h], edx
Offs07_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs07_C:

Dump7:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs08_A:       jb     Dump8
                mov    [edi*4 + 12345678h], edx
Offs08_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs08_C:

Dump8:
;컴컴 Pixel 1
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs09_A:       jb     Dump9

                mov    [edi*4 + 12345678h], edx
Offs09_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs09_C:

Dump9:
;컴컴 Pixel 10
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs10_A:       jb     Dump10

                mov    [edi*4 + 12345678h], edx
Offs10_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs10_C:

Dump10:
;컴컴 Pixel 11
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs11_A:       jb     Dump11

                mov    [edi*4 + 12345678h], edx
Offs11_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs11_C:

Dump11:
;컴컴 Pixel 12
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs12_A:       jb     Dump12

                mov    [edi*4 + 12345678h], edx
Offs12_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs12_C:

Dump12:
;컴컴 Pixel 13
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs13_A:       jb     Dump13

                mov    [edi*4 + 12345678h], edx
Offs13_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs13_C:

Dump13:
;컴컴 Pixel 14
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs14_A:       jb     Dump14

                mov    [edi*4 + 12345678h], edx
Offs14_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs14_C:

Dump14:
;컴컴 Pixel 15
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                cmp    edx, [edi*4 + 12345678h]
Offs15_A:       jb     Dump15

                mov    [edi*4 + 12345678h], edx
Offs15_B:       mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
Offs15_C:

Dump15:
                add    edx, NZSlopeInt
                add    ecx, VFrac

                sbb    eax, eax

                add    ebx, ebp
                adc    esi, [Slope + eax*4]

                mov    LeftNdZ, edx

                add    edi, 16                  ;Move memory pointer
                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                ja     Loop16



;컴컴 Write lefTover pixels...

LeftOver:


;eax = generally used
;ebx = u-fraction
;ecx = v-fraction
;edx = 1/Z interpolation
;esi = tmap pointer
;edi = dbuf pointer


                mov    edx, LeftNdZ
                mov    ebp, ScanLength
                inc    ebp

LeftOverLoop:
                cmp    edx, [edi*4 + 12345678h]
OffsXX_A:       jb     DumpXX
                mov    [edi*4 + 12345678h], edx
OffsXX_B:

                mov    eax, [esi*4]
                mov    [edi*4 + 12345678h], eax
OffsXX_C:

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
                popad                           ;ResTore all general regs

              ENDM









;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : T07_Tri
;쿔D           : T07
;쿟ype         : Procedure
;쿗ast update  : 26.07.1997
;쿌ction       : Draws a perspectivemapped triangel, 32k colours, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [esi]           = Offset Face STRUCT
;�                  T07_MapOffset   = Offset texturemap
;�                  T07_MapWidth    = Mapwidth in pixels (!!! INTEGER !!!)
;�                  T07_MapModifier = Mappingcoordinate-modifer (default=1.0)
;�
;쿙otes: FPU stack must be empty. General registers are Destroyed.
;쿔MPORTANT: T07_MapOffset and T07_MapWidth MUST BE SET!!!
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

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
_V0Offs         DD 0                            ;Offset To upper vertex
_V1Offs         DD 0                            ;Offset To middle vertex
_V2Offs         DD 0                            ;Offset To lower vertex

_U0             DD 0.0                          ;FP: Sorted map. coordinates
_V0             DD 0.0
_U1             DD 0.0
_V1             DD 0.0
_U2             DD 0.0
_V2             DD 0.0

USlopeA		DD 0.0
VSlopeA		DD 0.0
USlopeB		DD 0.0
VSlopeB		DD 0.0
USlope		DD 0.0
VSlope		DD 0.0


T07_Tri       PROC

;컴컴 Load, multiply and locally sTore mappingcoordinates...
                fld    [esi].F_U0
                fmul   T07_MapModifier
                fstp   _U0
                fld    [esi].F_V0
                fmul   T07_MapModifier
                fstp   _V0

                fld    [esi].F_U1
                fmul   T07_MapModifier
                fstp   _U1
                fld    [esi].F_V1
                fmul   T07_MapModifier
                fstp   _V1

                fld    [esi].F_U2
                fmul   T07_MapModifier
                fstp   _U2
                fld    [esi].F_V2
                fmul   T07_MapModifier
                fstp   _V2



;컴컴 Load offsets To vertices...
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



;컴컴 Get USlope A...
                fld    _U1            ;U1   YNDB YNDA
		fsub   _U0	      ;UDA  YNDB YNDA
                fmul   st, st(2)      ;USA  YNDB YNDA
                fstp   USlopeA        ;YNDB YNDA


;컴컴 Get VSlope A...
                fld    _V1            ;V1   YNDB YNDA
		fsub   _V0            ;VDA  YNDB YNDA
                fmul   st, st(2)      ;VSA  YNDB YNDA
                fstp   VSlopeA        ;YNDB YNDA


;컴컴 Get USlope B...
                fld    _U2            ;U2   YNDB YNDA
		fsub   _U0            ;UDB  YNDB YNDA
                fmul   st, st(1)      ;USB  YNDB YNDA
                fstp   USlopeB        ;YNDB YNDA


;컴컴 Get VSlope B...
                fld    _V2            ;V2   YNDB YNDA
		fsub   _V0            ;VDB  YNDB YNDA
                fmul   st, st(1)      ;VZSB YNDB YNDA
                fstp   VSlopeB        ;YNDB YNDA


;컴컴 Get 1/ZSlope A...
                fld    [ecx].V_InvZ   ;NZ1  YNDB YNDA
                fsub   [ebx].V_InvZ   ;NZDA YNDB YNDA
                fmul   st, st(2)      ;NZSA YNDB YNDA
                fstp   NZSlopeA       ;YNDB YNDA

;컴컴 Get 1/ZSlope B...
                fld    [edx].V_InvZ   ;NZ2  YNDB YNDA
                fsub   [ebx].V_InvZ   ;NZDB YNDB YNDA
                fmul   st, st(1)      ;NZSB YNDB YNDA
                fstp   NZSlopeB       ;YNDB YNDA


;컴컴 Get XSlope A and B...
                fld    [ecx].V_X2D    ;X1   YNDB YNDA
                fsub   [ebx].V_X2D    ;XDA  YNDB YNDA
                fmul   st, st(2)      ;XSA  YNDB YNDA
                fstp   XSlopeA        ;YNDB YNDA

                fld    [edx].V_X2D    ;X2   YNDB YNDA
                fsub   [ebx].V_X2D    ;XDB  YNDB YNDA
                fmul   st, st(1)      ;XSB  YNDB YNDA
                fstp   XSlopeB        ;YNDB YNDA

                ffree  st(0)
                ffree  st(1)


;컴컴 Get general USlope, VSlope and 1/ZSlope...
;xMxx = value at middle of edge B
                fld    [ecx].V_Y2D    ;Y1
                fsub   [ebx].V_Y2D    ;YDA

                fld    XSlopeB        ;XSB  YDA
                fmul   st, st(1)      ;XM0B YDA
                fadd   [ebx].V_X2D    ;XMB  YDA
                fsub   [ecx].V_X2D    ;XMD  YDA
                fdivr  One            ;XNMD YDA

                fld    USlopeB        ;USB  XNMD YDA
                fmul   st, st(2)      ;UMB  XNMD YDA
                fadd   _U0            ;UMB  XNMD YDA
                fsub   _U1            ;UMD  XNMD YDA
                fmul   st, st(1)      ;US   XNMD YDA
		fst    USlope
		fmul   Fixed16_16     ;USF  XNMD YDA
                fistp  USlopeFixed    ;XNMD YDA

                fld    VSlopeB        ;VSB  XNMD YDA
                fmul   st, st(2)      ;VMB  XNMD YDA
                fadd   _V0            ;VMB  XNMD YDA
                fsub   _V1            ;VMD  XNMD YDA
                fmul   st, st(1)      ;VS   XNMD YDA
		fst    VSlope
		fmul   Fixed16_16     ;VSF  XNMD YDA
                fistp  VSlopeFixed    ;XNMD YDA

                fld    NZSlopeB       ;NZSB XNMD YDA
                fmul   st, st(2)      ;NZMB XNMD YDA
                fadd   [ebx].V_InvZ   ;NZMB XNMD YDA
                fsub   [ecx].V_InvZ   ;NZMD XNMD YDA
                fmul   st, st(1)      ;NZS  XNMD YDA
		fst    NZSlope
                fmul   SYS_ZBufFP     ;NZS  XNMD YDA
                fistp  NZSlopeInt     ;XNMD YDA

                ffree  st(1)          ;XNMD
                ffree  st(0)          ;empty


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
NoClipBottom0:
                fistp  Y0Int                    ;SToring clipped Y0


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
NoClipBottom1:
                fistp  Y1Int                    ;SToring clipped Y1



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
                fistp  Y2Int                    ;SToring Y2


                fild   Y0Int
                fcom   SYS_FPClipTop
                fstsw  ax
                sahf
                jae    NoTopClip
                fstp   FPDump
                fld    SYS_FPClipTop

NoTopClip:
				      ;Y0int

;컴컴 Clipping edge A To next scanline...
                fsub   [ebx].V_Y2D    ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebx].V_X2D    ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   USlopeA        ;UDA  YD
                fadd   _U0            ;UA   YD
                fstp   EdgeA.U        ;YD

                fld    st             ;YD   YD
                fmul   VSlopeA        ;VDA  YD
                fadd   _V0            ;VA   YD
                fstp   EdgeA.V        ;YD

                fld    st             ;YD   YD
                fmul   NZSlopeA       ;NZDA YD
                fadd   [ebx].V_InvZ   ;NZA  YD
                fstp   EdgeA.NdZ      ;YD



;컴컴 Clipping edge B To next scanline...
                fld    st             ;YD   YD
                fmul   XSlopeB        ;XDB  YD
                fadd   [ebx].V_X2D    ;XB   YD
                fstp   EdgeB.X        ;YD

                fld    st             ;YD   YD
                fmul   USlopeB        ;UDB  YD
                fadd   _U0            ;UB   YD
                fstp   EdgeB.U        ;YD

                fld    st             ;YD   YD
                fmul   VSlopeB        ;VDB  YD
                fadd   _V0            ;VB   YD
                fstp   EdgeB.V        ;YD

                fmul   NZSlopeB       ;NZDA
                fadd   [ebx].V_InvZ   ;NZA
                fstp   EdgeB.NdZ

;컴컴 Calculating memoffset
                fild   Y0Int          ;Y0I
                fild   SYS_XRes       ;SLS  Y0I
                fmulp  st(1), st      ;Offs Y0I
                fistp  PixelPtr       ;Y0I


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

                imul   ecx, T07_MapWidth
                add    ecx, eax
                mov    Slope, ecx               ;Save normal slope
                add    ecx, T07_MapWidth
                mov    [Slope - 4], ecx         ;Save slope + 1 scanline


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


;---- Setting up for loop...
                mov    eax, edx                 ;Use eax as pointer To edge A
                xor    edx, edx

                mov    ecx, Y1Int               ;Get number of lines To draw
                sub    ecx, Y0Int

                                                ;[eax] = left edge
                                                ;[ebx] = right edge
                                                ; ecx  = loopcounter
                                                ; edx  = flag

ScanLoop:
                or     ecx, ecx
                jz     DoEdge12

                T07_ScanLine


;컴컴 Update edge A...
                fld    EdgeA.X
                fadd   XSlopeA
                fstp   EdgeA.X

                fld    EdgeA.U
                fadd   USlopeA
                fstp   EdgeA.U

                fld    EdgeA.V
                fadd   VSlopeA
                fstp   EdgeA.V

                fld    EdgeA.NdZ
                fadd   NZSlopeA
                fstp   EdgeA.NdZ



;컴컴 Update edge B...
                fld    EdgeB.X
                fadd   XSlopeB
                fstp   EdgeB.X

                fld    EdgeB.U
                fadd   USlopeB
                fstp   EdgeB.U

                fld    EdgeB.V
                fadd   VSlopeB
                fstp   EdgeB.V

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
                fld    [edi].V_Y2D    ;Y2
                fsub   [ebp].V_Y2D    ;YD
                fdivr  One            ;YNDA


;컴컴 Get new U/ZSlope A...
                fld    _U2            ;U2   YNDA
                fsub   _U1            ;UDA  YNDA
                fmul   st, st(1)      ;USA  YNDA
                fstp   USlopeA        ;YNDA


;컴컴 Get new V/ZSlope A...
                fld    _V2            ;V2   YNDA
                fsub   _V1            ;VDA  YNDA
                fmul   st, st(1)      ;VSA  YNDA
                fstp   VSlopeA        ;YNDA


;컴컴 Get new 1/ZSlope A...
                fld    [edi].V_InvZ   ;NZ2  YNDA
                fsub   [ebp].V_InvZ   ;NZDA YNDA
                fmul   st, st(1)      ;NZSA YNDA
                fstp   NZSlopeA       ;YNDA

;컴컴 Get new XSlopeA...
                fld    [edi].V_X2D    ;X2   YNDA
                fsub   [ebp].V_X2D    ;XDA  YNDA
                fmul   st, st(1)      ;XSA  YNDA
                fstp   XSlopeA	      ;YNDA
                ffree  st	      ;empty


;컴컴 Clipping edge C To next scanline and setting initial values...

                fild   Y1Int          ;Y1I
                fsub   [ebp].V_Y2D    ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebp].V_X2D    ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   USlopeA        ;UDA  YD
                fadd   _U1            ;UA   YD
                fstp   EdgeA.U        ;YD

                fld    st             ;YD   YD
                fmul   VSlopeA        ;VDA  YD
                fadd   _V1            ;VA   YD
                fstp   EdgeA.V        ;YD

                fmul   NZSlopeA       ;NZDA
                fadd   [ebp].V_InvZ   ;NZA
                fstp   EdgeA.NdZ

                inc    edx            ;Set flag
                mov    ecx, Y2Int
                sub    ecx, Y1Int
                jmp    ScanLoop


Done:
                ret
T07_Tri       ENDP



T07_Init      PROC
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

                add    eax, 4
                mov    DWORD PTR [Offs01_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs02_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs03_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs04_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs05_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs06_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs07_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs08_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs09_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs10_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs11_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs12_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs13_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs14_C - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs15_C - 4], eax

                ret
T07_Init      ENDP

END
