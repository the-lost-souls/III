;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T06_ORG.ASM
;³Author       : Cyberfish/TLS
;³Last update  : 07.01.99
;³Action       : Transparent, flat, 32 bits, z-buffer
;³
;³Notes :
;³To be done:
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model Flat, C
.code

include w:\general\var.inc

public T06_Tri
public T06_Init
public T06_Call


Edge          STRUC

X               DD ?
UdZ             DD ?
VdZ             DD ?
NdZ             DD ?

Edge          ENDS

ALIGN 4

;ÄÄÄÄ Locally used "constants"
Fixed16_16      DD 65536.0
Fixed16         DD 4096.0                       ;65536.0/ 16.0
Sixteen         DD 16.0
One             DD 1.0
AlmostZero      DD 0.000000000000000000001

FPDump          DD 0








;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : ZClip06
;³ID           :
;³Type         : Macro
;³Last update  : 12.07.1997
;³Action       : Z-clips a polygon pointed to by edi
;³Optimized    : ?
;³
;³Input variables : [eax]  = left edge
;³                  [ebx]  = right edge
;³
;³Notes : Macros "Project_Vertex" and "Clip_Line" are macros used by
;³this routine.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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









;-------------------------------------------------------------------
ALIGN 4
RLookup       DB 256 DUP (0)
GLookup       DB 256 DUP (0)
BLookup       DB 256 DUP (0)

;[ebp] = material
CreateLookups MACRO

		fldcw   FPU_Round

;---- Creating red lookup
		xor	edi, edi
		mov    	ecx, 256                  ;loopcounter

LookupR:
		mov	FPDump, edi
		fild 	FPDump
		fsub	R
		fmul	[ebp].M_Transparency
		fadd	R
		fistp	FPDump

		mov	edx, FPDump
		mov	RLookup[edi], dl

		inc	edi
		dec	ecx
                jnz    	LookupR


;---- Creating green lookup
		xor	edi, edi
		mov    	ecx, 256                  ;loopcounter

LookupG:
		mov	FPDump, edi
		fild 	FPDump
		fsub	G
		fmul	[ebp].M_Transparency
		fadd	G
		fistp	FPDump

		mov	edx, FPDump
		mov	GLookup[edi], dl

		inc	edi
		dec	ecx
                jnz    	LookupG


;---- Creating blue lookup
		xor	edi, edi
		mov    	ecx, 256                  ;loopcounter

LookupB:
		mov	FPDump, edi
		fild 	FPDump
		fsub	B
		fmul	[ebp].M_Transparency
		fadd	B
		fistp	FPDump

		mov	edx, FPDump
		mov	BLookup[edi], dl

		inc	edi
		dec	ecx
                jnz    	LookupB

              ENDM





;----------------------------------------------------------------------------






;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T06_Call
;³ID           : T06
;³Type         : Procedure
;³Last update  : 03.10.1997
;³Action       : Runs z-clipping, then draws...
;³Optimized    : No
;³
;³Input variables : [esi] = polygon
;³
;³Notes : FPU stack must be empty.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ALIGN 4

R               DD 0.0
G               DD 0.0
B               DD 0.0

ColorRangeR     DD 255.0
ColorRangeG     DD 255.0
ColorRangeB     DD 255.0

;esi = polygon
T06_Call      PROC

                mov    ebp, [esi].F_Material
                fldcw  	FPU_Round

;---- Getting red component...

                fld    	[esi].F_IntensityR
                fmul   	[ebp].M_Diffuse
                fadd   	[ebp].M_Luminosity
		fcom   	One
		fstsw  	ax
		sahf
		jb     	NoRClip
		ffree  	st(0)
		fld	One
NoRClip:
                fmul    [ebp].M_R
		fmul	ColorRangeR
                fstp    R



;---- Getting green component...
                fld    	[esi].F_IntensityG
                fmul   	[ebp].M_Diffuse
                fadd   	[ebp].M_Luminosity
		fcom   	One
		fstsw  	ax
		sahf
		jb     	NoGClip
		ffree  	st(0)
		fld	One
NoGClip:
                fmul   	[ebp].M_G
		fmul	ColorRangeG
                fstp  	G


;---- Getting blue component...
		fld    	[esi].F_IntensityB
                fmul   	[ebp].M_Diffuse
                fadd   	[ebp].M_Luminosity
		fcom	One
		fstsw	ax
		sahf
		jb	NoBClip
		ffree	st(0)
		fld	One
NoBClip:
                fmul   	[ebp].M_B
                fmul   	ColorRangeB
                fstp   	B

                CreateLookups

;---- Z-clipping?
                cmp    [esi].F_ZFlag, 0
                je     NoZClip

                call   ZClip

                cmp    [esi].F_ZFlag, 2         ;Draw to/one polygon?
                je     DrawOne                  ;Draw one!

                mov    esi, OFFSET NewFace1
                call   T06_Tri

DrawOne:
                mov    esi, OFFSET NewFace0
NoZClip:
                call   T06_Tri

                ret
T06_Call      ENDP












;----------------------------------------------------------------------------





;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T06_Scanline
;³ID           : T06
;³Type         : Macro
;³Last update  : 12.12.1998
;³Action       : Draws a transparent, flat scanline
;³Optimized    : No
;³
;³Input variables : [eax]  = left edge
;³                  [ebx]  = right edge
;³
;³Notes : FPU stack must be empty. All integer registers are saved.
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

T06_ScanLine  MACRO
                                     ;st0  st1  st2  st3  st4  st5  st6  st7


;ÄÄÄÄ Clip 1/Z-right to last pixel...
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


;ÄÄÄÄ Clip 1/Z left to next pixel...
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


;ÄÄÄÄ Save fixed point 1/Z left
                fmul   SYS_ZBufFP    ;1/ZL
                fistp  LeftNdZ       ;

                pushad                          ;Save all registers

		mov    eax, SYS_ZBufAdd
		add    LeftNdZ, eax

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


		mov    edx, LeftNdZ
		mov    esi, NZSlopeInt

;eax = fill-word (R:G:B, 5:5:5:1
;ecx = 1/Z fixed point slope
;edx = 1/Z interpolation
;edi = buffer pointer

;-------------------------- LOOP STARTS HERE -------------------------------

Loop16:

                xor     eax, eax
		xor	ebx, ebx
		xor	ecx, ecx
;ÄÄÄÄ Pixel 0

                cmp    	edx, [edi*4 + 12345678h]
Offs00_Z1:      jbe    	Dump0

;                mov    [edi*4 + 12345678h], edx
Offs00_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs00_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs00_G1:
		mov	cl, [edi*4 + 12345678h]		;Loading B
Offs00_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs00_R2:
		mov	[edi*4 + 12345678h], bl
Offs00_G2:
		mov	[edi*4 + 12345678h], cl
Offs00_B2:

Dump0:
;ÄÄÄÄ Pixel 1
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs01_Z1:      jbe    	Dump1

;                mov    [edi*4 + 12345678h], edx
Offs01_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs01_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs01_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs01_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs01_R2:
		mov	[edi*4 + 12345678h], bl
Offs01_G2:
		mov	[edi*4 + 12345678h], cl
Offs01_B2:

Dump1:


;ÄÄÄÄ Pixel 2
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs02_Z1:      jbe    	Dump2

;                mov    [edi*4 + 12345678h], edx
Offs02_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs02_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs02_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs02_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs02_R2:
		mov	[edi*4 + 12345678h], bl
Offs02_G2:
		mov	[edi*4 + 12345678h], cl
Offs02_B2:

Dump2:

;ÄÄÄÄ Pixel 3
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs03_Z1:      jbe    	Dump3

;                mov    [edi*4 + 12345678h], edx
Offs03_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs03_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs03_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs03_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs03_R2:
		mov	[edi*4 + 12345678h], bl
Offs03_G2:
		mov	[edi*4 + 12345678h], cl
Offs03_B2:

Dump3:

;ÄÄÄÄ Pixel 4
                add    edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs04_Z1:      jbe    	Dump4

;                mov    [edi*4 + 12345678h], edx
Offs04_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs04_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs04_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs04_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs04_R2:
		mov	[edi*4 + 12345678h], bl
Offs04_G2:
		mov	[edi*4 + 12345678h], cl
Offs04_B2:

Dump4:

;ÄÄÄÄ Pixel 5
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs05_Z1:      jbe    	Dump5

;                mov    [edi*4 + 12345678h], edx
Offs05_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs05_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs05_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs05_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs05_R2:
		mov	[edi*4 + 12345678h], bl
Offs05_G2:
		mov	[edi*4 + 12345678h], cl
Offs05_B2:

Dump5:

;ÄÄÄÄ Pixel 6
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs06_Z1:      jbe    	Dump6

;                mov    [edi*4 + 12345678h], edx
Offs06_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs06_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs06_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs06_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs06_R2:
		mov	[edi*4 + 12345678h], bl
Offs06_G2:
		mov	[edi*4 + 12345678h], cl
Offs06_B2:

Dump6:

;ÄÄÄÄ Pixel 7
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs07_Z1:      jbe    	Dump7

;                mov    [edi*4 + 12345678h], edx
Offs07_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs07_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs07_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs07_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs07_R2:
		mov	[edi*4 + 12345678h], bl
Offs07_G2:
		mov	[edi*4 + 12345678h], cl
Offs07_B2:

Dump7:

;ÄÄÄÄ Pixel 8
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs08_Z1:      jbe    	Dump8

;                mov    [edi*4 + 12345678h], edx
Offs08_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs08_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs08_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs08_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs08_R2:
		mov	[edi*4 + 12345678h], bl
Offs08_G2:
		mov	[edi*4 + 12345678h], cl
Offs08_B2:

Dump8:

;ÄÄÄÄ Pixel 9
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs09_Z1:      jbe    	Dump9

;                mov    [edi*4 + 12345678h], edx
Offs09_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs09_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs09_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs09_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs09_R2:
		mov	[edi*4 + 12345678h], bl
Offs09_G2:
      ÿÿ	mov	[edi*4 + 12345678h], cl
Offs09_B2:

Dump9:

;ÄÄÄÄ Pixel 10
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs10_Z1:      jbe    	Dump10

;                mov    [edi*4 + 12345678h], edx
Offs10_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs10_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs10_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs10_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs10_R2:
		mov	[edi*4 + 12345678h], bl
Offs10_G2:
		mov	[edi*4 + 12345678h], cl
Offs10_B2:

Dump10:

;ÄÄÄÄ Pixel 11
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs11_Z1:      jbe    	Dump11

;                mov    [edi*4 + 12345678h], edx
Offs11_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs11_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs11_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs11_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs11_R2:
		mov	[edi*4 + 12345678h], bl
Offs11_G2:
		mov	[edi*4 + 12345678h], cl
Offs11_B2:

Dump11:

;ÄÄÄÄ Pixel 12
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs12_Z1:      jbe    	Dump12

;                mov    [edi*4 + 12345678h], edx
Offs12_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs12_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs12_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs12_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs12_R2:
		mov	[edi*4 + 12345678h], bl
Offs12_G2:
		mov	[edi*4 + 12345678h], cl
Offs12_B2:

Dump12:

;ÄÄÄÄ Pixel 13
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs13_Z1:      jbe    	Dump13

;                mov    [edi*4 + 12345678h], edx
Offs13_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs13_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs13_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs13_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs13_R2:
		mov	[edi*4 + 12345678h], bl
Offs13_G2:
		mov	[edi*4 + 12345678h], cl
Offs13_B2:

Dump13:

;ÄÄÄÄ Pixel 14
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs14_Z1:      jbe    	Dump14

;                mov    [edi*4 + 12345678h], edx
Offs14_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs14_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs14_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs14_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs14_R2:
		mov	[edi*4 + 12345678h], bl
Offs14_G2:
		mov	[edi*4 + 12345678h], cl
Offs14_B2:

Dump14:

;ÄÄÄÄ Pixel 15
                add     edx, esi
                cmp    	edx, [edi*4 + 12345678h]
Offs15_Z1:      jbe    	Dump15

;                mov    [edi*4 + 12345678h], edx
Offs15_Z2:

		mov	al, [edi*4 + 12345678h]		;Loading R
Offs15_R1:
		mov	bl, [edi*4 + 12345678h]		;Loading G
Offs15_G1:
      ÿÿ	mov	cl, [edi*4 + 12345678h]		;Loading B
Offs15_B1:

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	cl, BLookup[ecx]

		mov	[edi*4 + 12345678h], al
Offs15_R2:
		mov	[edi*4 + 12345678h], bl
Offs15_G2:
		mov	[edi*4 + 12345678h], cl
Offs15_B2:

Dump15:
		add	edx, esi

                mov    LeftNdZ, edx

                add    edi, 16                  ;Move memory pointer
                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                jbe    LeftOver1
                jmp    Loop16



;ÄÄÄÄ Write leftover pixels...

LeftOver1:



                mov    	edx, LeftNdZ
                mov    	ebp, ScanLength
                inc    	ebp
                xor    	eax, eax
		mov	ecx, NZSlopeInt

LeftOverLoop:
                cmp    	edx, [edi*4 + 12345678h]
OffsXX_Z1:      jbe    	DumpXX
;                mov     [edi*4 + 12345678h], edx
OffsXX_Z2:


		mov	al, [edi*4 + 12345678h]
OffsXX_R1:
		mov	al, RLookup[eax]
		mov	[edi*4 + 12345678h], al
OffsXX_R2:


		mov	al, [edi*4 + 12345678h]
OffsXX_G1:
		mov	al, GLookup[eax]
		mov	[edi*4 + 12345678h], al
OffsXX_G2:


		mov	al, [edi*4 + 12345678h]
OffsXX_B1:
		mov	al, BLookup[eax]
		mov	[edi*4 + 12345678h], al
OffsXX_B2:

DumpXX:
		inc    	edi
                add    	edx, ecx

                dec    	ebp
                jnz    	LeftOverLoop


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









;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T06_Tri
;³ID           : T06
;³Type         : Procedure
;³Last update  : 26.07.1997
;³Action       : Draws a perspectivemapped triangel, 32k colours, z-buffer
;³Optimized    : No
;³
;³Input variables : [esi]           = Offset Face STRUCT
;³                  T06_MapOffset   = Offset texturemap
;³                  T06_MapWidth    = Mapwidth in pixels (!!! INTEGER !!!)
;³                  T06_MapModifier = Mappingcoordinate-modifer (default=1.0)
;³
;³Notes: FPU stack must be empty. General registers are destroyed.
;³IMPORTANT: T06_MapOffset and T06_MapWidth MUST BE SET!!!
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

;ÄÄÄÄ General variables...
UZSlope         DD 0                            ;FP: U/Z slope
VZSlope         DD 0                            ;FP: V/Z slope
NZSlope         DD 0                            ;FP: 1/Z slope
UZSlope16       DD 0                            ;FP: U/Z slope *16
VZSlope16       DD 0                            ;FP: V/Z slope *16
NZSlope16       DD 0                            ;FP: 1/Z slope *16
NZSlopeInt      DD 0                            ;Int: 1/Z slope

EdgeA         Edge ?                            ;Edge A data
EdgeB         Edge ?                            ;Edge B data

;ÄÄÄÄ Flipped values... (well, sorted that is)
_V0Offs         DD 0                            ;Offset to upper vertex
_V1Offs         DD 0                            ;Offset to middle vertex
_V2Offs         DD 0                            ;Offset to lower vertex

_U0             DD 0.0                          ;FP: Sorted map. coordinates
_V0             DD 0.0
_U1             DD 0.0
_V1             DD 0.0
_U2             DD 0.0
_V2             DD 0.0


T06_Tri       PROC


;ÄÄÄÄ Load offsets to vertices...
;[ebx] = V0
;[ecx] = V1
;[edx] = V2
;
;These pointers (ebx, ecx, edx) are valid until the edgeflip

                mov    ebx, [esi].F_V0Offs
                mov    ecx, [esi].F_V1Offs
                mov    edx, [esi].F_V2Offs




;ÄÄÄÄ Sorting vertices...
                                      ;st0  st1  st2  st3  st4  st5  st6  st7
                fld    [edx].V_Y2D      ;V2Y
                fld    [ecx].V_Y2D      ;V1Y  V2Y
                fld    [ebx].V_Y2D      ;V0Y  V1Y  V2Y


;ÄÄÄÄ Check Y0 <-> Y1
                fcom   st(1)          ;V0Y  V1Y  V2Y
                fstsw  ax
                sahf
                jbe    Swapped01

                xchg   ebx, ecx
                fxch   st(1)

Swapped01:




;ÄÄÄÄ Check Y0 <-> Y2
                fcom   st(2)
                fstsw  ax
                sahf
                jbe    Swapped02

                xchg   ebx, edx
                fxch   st(2)
Swapped02:




;ÄÄÄÄ Check Y1 <-> Y2
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


;ÄÄÄÄ There... Now st(0) > st(1) > st(2)
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
;ÄÄÄÄ Get 1/YDeltaA and 1/YDeltaB
                fsub   st, st(2)      ;YDB  Y1   Y0
                fadd   AlmostZero
                fxch   st(1)          ;Y1   YDB  Y0
                fsub   st, st(2)      ;YDA  YDB  Y0
                fadd   AlmostZero
                fdivr  One            ;YNDA YDB  Y0
                fxch   st(1)          ;YDB  YNDA Y0
                fdivr  One            ;YNDB YNDA Y0
                ffree  st(2)          ;YNDB YNDA

;ÄÄÄÄ Get 1/ZSlope A...
                fld    [ecx].V_InvZ     ;NZ1  YNDB YNDA
                fsub   [ebx].V_InvZ     ;NZDA YNDB YNDA
                fmul   st, st(2)      ;NZSA YNDB YNDA
                fstp   NZSlopeA       ;YNDB YNDA

;ÄÄÄÄ Get 1/ZSlope B...
                fld    [edx].V_InvZ     ;NZ2  YNDB YNDA
                fsub   [ebx].V_InvZ     ;NZDB YNDB YNDA
                fmul   st, st(1)      ;NZSB YNDB YNDA
                fstp   NZSlopeB       ;YNDB YNDA


;ÄÄÄÄ Get XSlope A and B...
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

;ÄÄÄÄ Get general U/ZSlope, V/ZSlope and 1/ZSlope...
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
                fmul   SYS_ZBufFP     ;NZS  NZS  XNMD YDA
                fistp  NZSlopeInt     ;NZS  XNMD YDA
                fmul   Sixteen        ;NZS  XNMD YDA
                fstp   NZSlope16      ;XNMD YDA

                ffree  st(1)          ;XNMD
                ffree  st             ;empty

;ÄÄÄÄ Rounding and clipping Y-values...
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

;ÄÄÄÄ Clipping edge A to next scanline...
                fsub   [ebx].V_Y2D      ;YD

                fld    st             ;YD   YD
                fmul   XSlopeA        ;XDA  YD
                fadd   [ebx].V_X2D      ;XA   YD
                fstp   EdgeA.X        ;YD

                fld    st             ;YD   YD
                fmul   NZSlopeA       ;NZDA YD
                fadd   [ebx].V_InvZ     ;NZA  YD
                fstp   EdgeA.NdZ      ;YD



;ÄÄÄÄ Clipping edge B to next scanline...
                fld    st             ;YD   YD
                fmul   XSlopeB        ;XDB  YD
                fadd   [ebx].V_X2D      ;XB   YD
                fstp   EdgeB.X        ;YD

                fmul   NZSlopeB       ;NZDA
                fadd   [ebx].V_InvZ     ;NZA
                fstp   EdgeB.NdZ

;ÄÄÄÄ Calculating memoffset
                fild   Y0Int          ;Y0I
                fild   SYS_XRes       ;SLS  Y0I
                fmulp  st(1), st      ;Offs Y0I
                fistp  PixelPtr       ;Y0I



;ÄÄÄÄ Setup eax and ebx as edgepointers...
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


                T06_ScanLine


;ÄÄÄÄ Update edge A...
                fld    EdgeA.X
                fadd   XSlopeA
                fstp   EdgeA.X

                fld    EdgeA.NdZ
                fadd   NZSlopeA
                fstp   EdgeA.NdZ



;ÄÄÄÄ Update edge B...
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

;ÄÄÄÄ Get 1/DeltaY...
                fld    [edi].V_Y2D      ;Y2
                fsub   [ebp].V_Y2D      ;YD
                fdivr  One            ;YNDA


;ÄÄÄÄ Get new 1/ZSlope A...
                fld    [edi].V_InvZ     ;NZ2  YNDA
                fsub   [ebp].V_InvZ     ;NZDA YNDA
                fmul   st, st(1)      ;NZSA YNDA
                fstp   NZSlopeA       ;YNDA

;ÄÄÄÄ Get new XSlopeA...
                fld    [edi].V_X2D      ;X2   YNDA
                fsub   [ebp].V_X2D      ;XDA  YNDA
                fmul   st, st(1)      ;XSA  YNDA
                fstp   XSlopeA
                ffree  st


;ÄÄÄÄ Clipping edge C to next scanline and setting initial values...

                fild   Y1Int          ;Y1I
                fsub   [ebp].V_Y2D      ;YD

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
                ret
T06_Tri       ENDP



T06_Init      PROC
                mov    eax, SYS_ZBufOffs

                mov    DWORD PTR [OffsXX_Z1 - 4], eax
;                mov    DWORD PTR [OffsXX_B - 4], eax

                mov    DWORD PTR [Offs00_Z1 - 4], eax
;                mov    DWORD PTR [Offs00_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs01_Z1 - 4], eax
;                mov    DWORD PTR [Offs01_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs02_Z1 - 4], eax
;                mov    DWORD PTR [Offs02_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs03_Z1 - 4], eax
;                mov    DWORD PTR [Offs03_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs04_Z1 - 4], eax
;                mov    DWORD PTR [Offs04_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs05_Z1 - 4], eax
;                mov    DWORD PTR [Offs05_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs06_Z1 - 4], eax
;                mov    DWORD PTR [Offs06_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs07_Z1 - 4], eax
;                mov    DWORD PTR [Offs07_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs08_Z1 - 4], eax
;                mov    DWORD PTR [Offs08_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs09_Z1 - 4], eax
;                mov    DWORD PTR [Offs09_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs10_Z1 - 4], eax
;                mov    DWORD PTR [Offs10_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs11_Z1 - 4], eax
;                mov    DWORD PTR [Offs11_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs12_Z1 - 4], eax
;                mov    DWORD PTR [Offs12_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs13_Z1 - 4], eax
;                mov    DWORD PTR [Offs13_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs14_Z1 - 4], eax
;                mov    DWORD PTR [Offs14_B - 4], eax

                add    eax, 4
                mov    DWORD PTR [Offs15_Z1 - 4], eax
;                mov    DWORD PTR [Offs15_B - 4], eax



;ÄÄÄÄ Set DBufOffs (edi)
                mov    	eax, SYS_DBufOffs
                mov    	d[OffsXX_B1 - 4], eax
                mov    	d[OffsXX_B2 - 4], eax

		inc	eax
                mov    	d[OffsXX_G1 - 4], eax
                mov    	d[OffsXX_G2 - 4], eax

		inc	eax
                mov     d[OffsXX_R1 - 4], eax
                mov     d[OffsXX_R2 - 4], eax

;0
		mov	eax, SYS_DBufOffs
                mov    	d[Offs00_B1 - 4], eax
                mov    	d[Offs00_B2 - 4], eax

		inc	eax
                mov    	d[Offs00_G1 - 4], eax
                mov    	d[Offs00_G2 - 4], eax

		inc	eax
                mov     d[Offs00_R1 - 4], eax
                mov     d[Offs00_R2 - 4], eax
		inc	eax

;1
		inc	eax
                mov    	d[Offs01_B1 - 4], eax
                mov    	d[Offs01_B2 - 4], eax

		inc	eax
                mov    	d[Offs01_G1 - 4], eax
                mov    	d[Offs01_G2 - 4], eax

		inc	eax
                mov     d[Offs01_R1 - 4], eax
                mov     d[Offs01_R2 - 4], eax
		inc	eax

;2
		inc	eax
                mov    	d[Offs02_B1 - 4], eax
                mov    	d[Offs02_B2 - 4], eax

		inc	eax
                mov    	d[Offs02_G1 - 4], eax
                mov    	d[Offs02_G2 - 4], eax

		inc	eax
                mov     d[Offs02_R1 - 4], eax
                mov     d[Offs02_R2 - 4], eax
		inc	eax

;3
		inc	eax
                mov    	d[Offs03_B1 - 4], eax
                mov    	d[Offs03_B2 - 4], eax

		inc	eax
                mov    	d[Offs03_G1 - 4], eax
                mov    	d[Offs03_G2 - 4], eax

		inc	eax
                mov     d[Offs03_R1 - 4], eax
                mov     d[Offs03_R2 - 4], eax
		inc	eax

;4
		inc	eax
                mov    	d[Offs04_B1 - 4], eax
                mov    	d[Offs04_B2 - 4], eax

		inc	eax
                mov    	d[Offs04_G1 - 4], eax
                mov    	d[Offs04_G2 - 4], eax

		inc	eax
                mov     d[Offs04_R1 - 4], eax
                mov     d[Offs04_R2 - 4], eax
		inc	eax

;5
		inc	eax
                mov    	d[Offs05_B1 - 4], eax
                mov    	d[Offs05_B2 - 4], eax

		inc	eax
                mov    	d[Offs05_G1 - 4], eax
                mov    	d[Offs05_G2 - 4], eax

		inc	eax
                mov     d[Offs05_R1 - 4], eax
                mov     d[Offs05_R2 - 4], eax
		inc	eax

;6
		inc	eax
                mov    	d[Offs06_B1 - 4], eax
                mov    	d[Offs06_B2 - 4], eax

		inc	eax
                mov    	d[Offs06_G1 - 4], eax
                mov    	d[Offs06_G2 - 4], eax

		inc	eax
                mov     d[Offs06_R1 - 4], eax
                mov     d[Offs06_R2 - 4], eax
		inc	eax

;7
		inc	eax
                mov    	d[Offs07_B1 - 4], eax
                mov    	d[Offs07_B2 - 4], eax

		inc	eax
                mov    	d[Offs07_G1 - 4], eax
                mov    	d[Offs07_G2 - 4], eax

		inc	eax
                mov     d[Offs07_R1 - 4], eax
                mov     d[Offs07_R2 - 4], eax
		inc	eax

;8
		inc	eax
                mov    	d[Offs08_B1 - 4], eax
                mov    	d[Offs08_B2 - 4], eax

		inc	eax
                mov    	d[Offs08_G1 - 4], eax
                mov    	d[Offs08_G2 - 4], eax

		inc	eax
                mov     d[Offs08_R1 - 4], eax
                mov     d[Offs08_R2 - 4], eax
		inc	eax

;9
		inc	eax
                mov    	d[Offs09_B1 - 4], eax
                mov    	d[Offs09_B2 - 4], eax

		inc	eax
                mov    	d[Offs09_G1 - 4], eax
                mov    	d[Offs09_G2 - 4], eax

		inc	eax
                mov     d[Offs09_R1 - 4], eax
                mov     d[Offs09_R2 - 4], eax
		inc	eax

;10
		inc	eax
                mov    	d[Offs10_B1 - 4], eax
                mov    	d[Offs10_B2 - 4], eax

		inc	eax
                mov    	d[Offs10_G1 - 4], eax
                mov    	d[Offs10_G2 - 4], eax

		inc	eax
                mov     d[Offs10_R1 - 4], eax
                mov     d[Offs10_R2 - 4], eax
		inc	eax

;11
		inc	eax
                mov    	d[Offs11_B1 - 4], eax
                mov    	d[Offs11_B2 - 4], eax

		inc	eax
                mov    	d[Offs11_G1 - 4], eax
                mov    	d[Offs11_G2 - 4], eax

		inc	eax
                mov     d[Offs11_R1 - 4], eax
                mov     d[Offs11_R2 - 4], eax
		inc	eax

;12
		inc	eax
                mov    	d[Offs12_B1 - 4], eax
                mov    	d[Offs12_B2 - 4], eax

		inc	eax
                mov    	d[Offs12_G1 - 4], eax
                mov    	d[Offs12_G2 - 4], eax

		inc	eax
                mov     d[Offs12_R1 - 4], eax
                mov     d[Offs12_R2 - 4], eax
		inc	eax

;13
		inc	eax
                mov    	d[Offs13_B1 - 4], eax
                mov    	d[Offs13_B2 - 4], eax

		inc	eax
                mov    	d[Offs13_G1 - 4], eax
                mov    	d[Offs13_G2 - 4], eax

		inc	eax
                mov     d[Offs13_R1 - 4], eax
                mov     d[Offs13_R2 - 4], eax
		inc	eax

;14
		inc	eax
                mov    	d[Offs14_B1 - 4], eax
                mov    	d[Offs14_B2 - 4], eax

		inc	eax
                mov    	d[Offs14_G1 - 4], eax
                mov    	d[Offs14_G2 - 4], eax

		inc	eax
                mov     d[Offs14_R1 - 4], eax
                mov     d[Offs14_R2 - 4], eax
		inc	eax

;15
		inc	eax
                mov    	d[Offs15_B1 - 4], eax
                mov    	d[Offs15_B2 - 4], eax

		inc	eax
                mov    	d[Offs15_G1 - 4], eax
                mov    	d[Offs15_G2 - 4], eax

		inc	eax
                mov     d[Offs15_R1 - 4], eax
                mov     d[Offs15_R2 - 4], eax
		inc	eax

                ret
T06_Init      ENDP

END
