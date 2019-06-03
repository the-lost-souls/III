;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T09_ORG.ASM
;³Author       : Cyberfish/TLS & Toe/DU
;³Last update  : 07.01.99
;³Action       : Transparent, flat, glass-bumped
;³
;³Notes :
;³To be done:
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.386
.Model Flat, C
.code

include w:\general\var.inc

public T09_Tri
public T09_Init
public T09_Call
public T09_DBuf
public T09_SetupFlag
public T09_DBufFlag


Edge          STRUC

X               DD ?
UdZ             DD ?
VdZ             DD ?
NdZ             DD ?

Edge          ENDS

ALIGN 4

T09_DBuf	DD 0
T09_SetupFlag	DD 0
T09_DBufFlag	DD 0

;ÄÄÄÄ In-variables...
T09_MapModU     DD 1.0                           ;FP: Modifier, mappingcoordinates
T09_MapModV	DD 1.0
T09_MapOffset   DD 0                            ;Offset texturemap
T09_MapWidth    DD 320                          ;INT: Tmap width, in pixels



;ÄÄÄÄ Locally used "constants"
Fixed16_16      DD 65536.0
Fixed16         DD 4096.0                       ;65536.0/ 16.0
Fixed032        DD 4294967295.0
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
RLookup       	DB 256 DUP (0)
GLookup       	DB 256 DUP (0)
BLookup       	DB 256 DUP (0)
YLookup		DD 64 DUP (0)

_31         	DD 31.0

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
;³Name         : T09_Call
;³ID           : T09
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

XMax		DD 0
YMax		DD 0

;esi = polygon
T09_Call      PROC
		cmp	T09_DBufFlag, 0
		je	NoDBufUpdate
		mov	T09_DBufFlag, 0

		push	esi

;---- Copying doublebuffer...
		mov	ecx, SYS_DBufSize
		shr	ecx, 6
		mov	edi, T09_DBuf
		mov	esi, SYS_DBufOffs
CopyLoop:

		mov	eax, [esi + 00]
		mov	ebx, [esi + 32]
		mov	[edi + 00], eax
		mov	[edi + 32], ebx

		mov	eax, [esi + 04]
		mov	ebx, [esi + 36]
		mov	[edi + 04], eax
		mov	[edi + 36], ebx

		mov	eax, [esi + 08]
		mov	ebx, [esi + 40]
		mov	[edi + 08], eax
		mov	[edi + 40], ebx

		mov	eax, [esi + 12]
		mov	ebx, [esi + 44]
		mov	[edi + 12], eax
		mov	[edi + 44], ebx

		mov	eax, [esi + 16]
		mov	ebx, [esi + 48]
		mov	[edi + 16], eax
		mov	[edi + 48], ebx

		mov	eax, [esi + 20]
		mov	ebx, [esi + 52]
		mov	[edi + 20], eax
		mov	[edi + 52], ebx

		mov	eax, [esi + 24]
		mov	ebx, [esi + 56]
		mov	[edi + 24], eax
		mov	[edi + 56], ebx

		mov	eax, [esi + 28]
		mov	ebx, [esi + 60]
		mov	[edi + 28], eax
		mov	[edi + 60], ebx

		add	esi, 64
		add	edi, 64
		dec	ecx
		jnz	CopyLoop

		pop	esi


NoDBufUpdate:

;---- Getting color...
                mov    ebp, [esi].F_Material

                fld    [esi].F_IntensityR
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fcom   One
                fstsw  ax
                sahf
                jb     NoRClip
                ffree  st(0)
                fld    One
NoRClip:
                fmul   [ebp].M_R
		fmul   ColorRangeR
                fstp   R

                fld    [esi].F_IntensityG
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fcom   One
                fstsw  ax
                sahf
                jb     NoGClip
                ffree  st(0)
                fld    One
NoGClip:
                fmul   [ebp].M_G
		fmul   ColorRangeG
                fstp   G

                fld    [esi].F_IntensityB
                fmul   [ebp].M_Diffuse
                fadd   [ebp].M_Luminosity
                fcom   One
                fstsw  ax
                sahf
                jb     NoBClip
                ffree  st(0)
                fld    One
NoBClip:
                fmul   [ebp].M_B
		fmul   ColorRangeB
                fstp   B

                mov    ebx, [ebp].M_MapWidth
                mov    T09_MapWidth, ebx
                mov    ebx, [ebp].M_MapOffset
                mov    T09_MapOffset, ebx

                CreateLookups

;---- Z-clipping?
                cmp    [esi].F_ZFlag, 0
                je     NoZClip

                call   ZClip

                cmp    [esi].F_ZFlag, 2         ;Draw to/one polygon?
                je     DrawOne                  ;Draw one!

                mov    esi, OFFSET NewFace1
                call   T09_Tri

DrawOne:
                mov    esi, OFFSET NewFace0
NoZClip:
                call   T09_Tri

                ret
T09_Call      ENDP












;----------------------------------------------------------------------------

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T09_Scanline
;³ID           : T09
;³Type         : Macro
;³Last update  : 12.07.1997
;³Action       : Draws a perspectivemapped, horisontal scanline, 32k, z-buffer
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

CurrentX	DD 0
CurrentY	DD 0

T09_ScanLine  MACRO
              ENDM









;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³Name         : T09_Tri
;³ID           : T09
;³Type         : Procedure
;³Last update  : 12.12.1998
;³Action       : Draws a perspectivemapped triangel, 32k colours, z-buffer
;³Optimized    : No
;³
;³Input variables : [esi]           = Offset Face STRUCT
;³                  T09_MapOffset   = Offset texturemap
;³                  T09_MapWidth    = Mapwidth in pixels (!!! INTEGER !!!)
;³                  T09_MapModifier = Mappingcoordinate-modifer (default=1.0)
;³
;³Notes: FPU stack must be empty. General registers are destroyed.
;³IMPORTANT: T09_MapOffset and T09_MapWidth MUST BE SET!!!
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
MemAdd  	DD 0				;Memory adder to get dbuf working

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


T09_Tri       PROC
;ÄÄÄÄ Load, multiply and locally store mappingcoordinates...
                fld    [esi].F_U0
                fmul   T09_MapModU
                fstp   _U0
                fld    [esi].F_V0
                fmul   T09_MapModV
                fstp   _V0

                fld    [esi].F_U1
                fmul   T09_MapModU
                fstp   _U1
                fld    [esi].F_V1
                fmul   T09_MapModV
                fstp   _V1

                fld    [esi].F_U2
                fmul   T09_MapModU
                fstp   _U2
                fld    [esi].F_V2
                fmul   T09_MapModV
                fstp   _V2



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




;ÄÄÄÄ Check Y0 <-> Y2
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




;ÄÄÄÄ Check Y1 <-> Y2
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



;ÄÄÄÄ Get U/ZSlope A...
                fld    _U1            ;U1   YNDB YNDA
                fmul   [ecx].V_InvZ     ;UZ1  YNDB YNDA
                fst    UZ1            ;UZ1  YNDB YNDA
                fld    _U0            ;U0   UZ1  YNDB YNDA
                fmul   [ebx].V_InvZ     ;UZ0  UZ1  YNDB YNDA
                fst    UZ0            ;UZ0  UZ1  YNDB YNDA
                fsubp  st(1), st      ;UZDA YNDB YNDA
                fmul   st, st(2)      ;UZSA YNDB YNDA
                fstp   UZSlopeA       ;YNDB YNDA


;ÄÄÄÄ Get V/ZSlope A...
                fld    _V1            ;V1   YNDB YNDA
                fmul   [ecx].V_InvZ     ;VZ1  YNDB YNDA
                fst    VZ1            ;VZ1  YNDB YNDA
                fld    _V0            ;V0   VZ1  YNDB YNDA
                fmul   [ebx].V_InvZ     ;VZ0  VZ1  YNDB YNDA
                fst    VZ0            ;VZ0  VZ1  YNDB YNDA
                fsubp  st(1), st      ;VZDA YNDB YNDA
                fmul   st, st(2)      ;VZSA YNDB YNDA
                fstp   VZSlopeA       ;YNDB YNDA


;ÄÄÄÄ Get U/ZSlope B...
                fld    _U2            ;U2   YNDB YNDA
                fmul   [edx].V_InvZ     ;UZ2  YNDB YNDA
                fst    UZ2            ;UZ2  YNDB YNDA
                fsub   UZ0            ;UZDB YNDB YNDA
                fmul   st, st(1)      ;UZSB YNDB YNDA
                fstp   UZSlopeB       ;YNDB YNDA


;ÄÄÄÄ Get V/ZSlope B...
                fld    _V2            ;V2   YNDB YNDA
                fmul   [edx].V_InvZ     ;VZ2  YNDB YNDA
                fst    VZ2            ;VZ2  YNDB YNDA
                fsub   VZ0            ;VZDB YNDB YNDA
                fmul   st, st(1)      ;VZSB YNDB YNDA
                fstp   VZSlopeB       ;YNDB YNDA


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
                jb     NoClipBottom0
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



;ÄÄÄÄ Clipping edge B to next scanline...
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

;ÄÄÄÄ Calculating memoffset
                fild   Y0Int          ;Y0I
                fild   SYS_XRes       ;SLS  Y0I
                fmulp  st(1), st      ;Offs Y0I
                fistp  PixelPtr       ;Y0I



;ÄÄÄÄ Setup eax and ebx as edgepointers...
;[eax] = left edge
;[ebx] = right edge

                mov    esi, OFFSET EdgeA        ;Use edx temporary
                mov    ebx, OFFSET EdgeB

                fld    XSlopeB
                fld    XSlopeA
                fcom   st(1)
                fstsw  ax
                sahf
                jbe    NoFlipAB

                xchg   esi, ebx
NoFlipAB:

                ffree  st
                ffree  st(1)

		;Setting up memadder...
		mov	ecx, Y0Int
		sub	ecx, 32
		imul	ecx, SYS_XRes
		mov	CurrentY, ecx

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


;                T09_ScanLine
                                     ;st0  st1  st2  st3  st4  st5  st6  st7


;ÄÄÄÄ Clip U/Z, V/Z and 1/Z right to last pixel...
                fldcw  FPU_RoundDown

                fld    [ebx].X       ;XR
                fcom   SYS_FPClipRight
                fstsw  ax
                sahf
                jb     NoRightClip
                fstp   FPDump
                fld    SYS_FPClipRight

NoRightClip:
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


;ÄÄÄÄ Clip U/Z, V/Z and 1/Z left to next pixel...
                fldcw  FPU_RoundUp

                fld    [esi].X       ;XL
                fcom   SYS_FPClipLeft
                fstsw  ax
                sahf
                ja     NoLeftClip
                fstp   FPDump
                fld    SYS_FPClipLeft

NoLeftClip:
                fistp  EdgeLeft.X    ;
                fild   EdgeLeft.X    ;XL
                fsub   [esi].X       ;XD

                fld    st            ;XD   XD
                fmul   UZSlope       ;XV   XD
                fadd   [esi].UdZ     ;UZL  XD

                fld    st(1)         ;XD   UZL  XD
                fmul   VZSlope       ;XV   UZL  XD
                fadd   [esi].VdZ     ;VZL  UZL  XD
                fxch   st(2)         ;XD   UZL  VZL

                fmul   NZSlope       ;XNZ  UZL  VZL
                fadd   [esi].NdZ     ;NZL  UZL  VZL


;ÄÄÄÄ Save fixed point 1/Z left
                fld    st            ;1/ZL 1/ZL U/ZL V/ZL
                fmul   SYS_ZBufFP    ;1/ZL 1/ZL U/ZL V/ZL
                fistp  LeftNdZ       ;1/ZL U/ZL V/ZL

;ÄÄÄÄ Get Z left...
                fld1                 ;1    1/ZL U/ZL V/ZL
                fdiv   st, st(1)     ;ZL   1/ZL U/ZL V/ZL




;ÄÄÄÄ !!!INTEGER INSTRUCTIONS HERE!!!
                pushad                          ;Save all registers

		mov	eax, SYS_ZBufAdd
		add	LeftNdZ, eax

                mov    edi, PixelPtr            ;Use edi as mempointer
                add    edi, EdgeLeft.X

                mov    eax, EdgeLeft.X          ;Calculate scanline length
                mov    ebx, EdgeRight.X         ;/
                sub    ebx, eax                 ;ebx = scanline length
                mov    ScanLength, ebx
		mov	CurrentX, eax
		sub	CurrentX, 32

                cmp    ebx, 0
                jl     Scan_Done

                cmp    ebx, 16                  ;Run leftoverloop now?
                jbe    LeftOver1
		jmp    LeftOver1


;ÄÄÄÄ Get U and V left (the fdiv is finished)
                fld    st            ;ZL   ZL   1/ZL U/ZL V/ZL
                fmul   st, st(4)     ;VL   ZL   1/ZL U/ZL V/ZL
                fxch   st(1)         ;ZL   VL   1/ZL U/ZL V/ZL
                fmul   st, st(3)     ;UL   VL   1/ZL U/ZL V/ZL


;ÄÄÄÄ Get U/Z and V/Z (right)
                fxch   st(2)         ;1/ZL VL   UL   U/ZL V/ZL
                fadd   NZSlope16     ;1/ZR VL   UL   U/ZL V/ZL
                fxch   st(3)         ;U/ZL VL   UL   1/ZR V/ZL
                fadd   UZSlope16     ;U/ZR VL   UL   1/ZR V/ZL
                fxch   st(4)         ;V/ZL VL   UL   1/ZR U/ZR
                fadd   VZSlope16     ;V/ZR VL   UL   1/ZR U/ZR
                fstp   st(5)         ;VL   UL   1/ZR U/ZR V/ZR


;ÄÄÄÄ Get Z-right
                fld    One           ;1    VL   UL   1/ZR U/ZR V/ZR
                fdiv   st, st(3)     ;ZR   VL   UL   1/ZR U/ZR V/ZR






;-------------------------- LOOP STARTS HERE -------------------------------

Loop16:


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ OPTIMIZED BLOCK B (PL_Bx.TXT) ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³* 45 cycles
;³* Calculating U and V right
;³* Calculating DeltaU and DeltaV
;³* Calculating slopes and stores them as 16.16 fixed point integers
;³* Saving U and V left as 16.16 fixed point integers
;³* Calculating U/Z, V/Z and 1/Z at next 16. pixel
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                               ;st0  st1  st2  st3  st4  st5  st6
                                               ;ZR   VL   UL   NZR  UZR  VZR

                fld    st            ;vr0->vr1  ZR   ZR   VL   UL   NZR  UZR  VZR
                fmul   st, st(5)     ;vr1, vr7  UR   ZR   VL   UL   NZR  UZR  VZR
                fxch   st(4)         ;          NZR  ZR   VL   UL   UR   UZR  VZR
                fadd   NZSlope16     ;vr6       NZRR ZR   VL   UL   UR   UZR  VZR
                fxch   st(1)         ;          ZR   NZRR VL   UL   UR   UZR  VZR
                fmul   st, st(6)     ;vr0, vr8  VR   NZRR VL   UL   UR   UZR  VZR
                fld    st(4)         ;vr1->vr3  UR   VR   NZRR VL   UL   UR   UZR  VZR
                fsub   st, st(4)     ;vr3, vr5  UD   VR   NZRR VL   UL   UR   UZR  VZR
                fxch   st(4)         ;          UL   VR   NZRR VL   UD   UR   UZR  VZR
                fmul   Fixed16_16    ;vr5       ULF  VR   NZRR VL   UD   UR   UZR  VZR
                fxch   st(6)         ;          UZR  VR   NZRR VL   UD   UR   ULF  VZR
                fadd   UZSlope16     ;vr7       UZRR VR   NZRR VL   UD   UR   ULF  VZR
                fxch   st(4)         ;          UD   VR   NZRR VL   UZRR UR   ULF  VZR
                fmul   Fixed16       ;vr3       US   VR   NZRR VL   UZRR UR   ULF  VZR
                fxch   st(7)         ;          VZR  VR   NZRR VL   UZRR UR   ULF  US
                fadd   VZSlope16     ;vr8       VZRR VR   NZRR VL   UZRR UR   ULF  US
                fxch   st(6)         ;          ULF  VR   NZRR VL   UZRR UR   VZRR US
                fistp  ULeftFixed    ;vr5       VR   NZRR VL   UZRR UR   VZRR US
                fld    st            ;vr0->vr2  VR   VR   NZRR VL   UZRR UR   VZRR US
                fsub   st, st(3)     ;vr2, vr4  VD   VR   NZRR VL   UZRR UR   VZRR US
                fxch   st(3)         ;          VL   VR   NZRR VD   UZRR UR   VZRR US
                fmul   Fixed16_16    ;vr4       VLF  VR   NZRR VD   UZRR UR   VZRR US
                fxch   st(7)         ;          US   VR   NZRR VD   UZRR UR   VZRR VLF
                fistp  USlopeFixed   ;vr3       VR   NZRR VD   UZRR UR   VZRR VLF
                fxch   st(2)         ;          VD   NZRR VR   UZRR UR   VZRR VLF
                fmul   Fixed16       ;vr2       VS   NZRR VR   UZRR UR   VZRR VLF
                fxch   st(6)         ;          VLF  NZRR VR   UZRR UR   VZRR VS
                fistp  VLeftFixed    ;vr4       NZRR VR   UZRR UR   VZRR VS
                fxch   st(5)         ;          VS   VR   UZRR UR   VZRR NZRR
                fistp  VSlopeFixed   ;vr2       VR   UZRR UR   VZRR NZRR

                fxch   st(1)         ;          UZRR VR   UR   VZRR NZRR
                fxch   st(4)         ;          NZRR VR   UR   VZRR UZRR
                fxch   st(3)         ;          VZRR VR   UR   NZRR UZRR
                fstp   st(5)         ;          VR   UR   NZRR UZRR VZRR


;ÄÄÄÄ END BLOCK B





;ÄÄÄÄ Check if we need Z for end of line next time...
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



;ÄÄÄÄ Setting up slopes and initial values...
                mov    eax, USlopeFixed         ;U:1 (A1) !!!Cache-delay!!!
                mov    ecx, VSlopeFixed         ;V:2 (B1) /

                mov    ebx, eax                 ;U:3 (A2) Take copy
                mov    edx, ecx                 ;V:3 (B2) /

                sal    ebx, 16                  ;U:4 (A3) Get fractional part
                mov    esi, VLeftFixed          ;V:4 (D1)

                sar    eax, 16                  ;U:5 (A3) Get int part
                mov    UFrac, ebx               ;V:5 (A4) Save fractional part

                sal    edx, 16                  ;U:6 (B3)
                add    esi, 8000h               ;V:6 (D2)

                sar    ecx, 16                  ;U:7 (B3)
                mov    VFrac, edx               ;V:7 (B4)

                imul   ecx, T09_MapWidth        ;U:8-19 (B4)

                add    ecx, eax                 ;U:20 (AB5)
                mov    ebp, T09_MapWidth        ;V:20 (AB6)

                mov    Slope, ecx               ;U:21 (AB6) Save normal slope
                mov    eax, ULeftFixed          ;V:21 (C1)

                add    ecx, ebp                 ;U:22 (AB7)
                add    eax, 8000h               ;V:22 (C2) add 0.5 (16.16 fixed point)

                mov    ebx, eax                 ;U:23 (C3) Take copy
                mov    [Slope - 4], ecx         ;V:23 (AB8) Save slope + 1 scanline

                mov    ecx, esi                 ;U:24 (D3)

                shr    esi, 16                  ;U:25 (D4)

                imul   esi, T09_MapWidth        ;U:26-37 (D5)

                shr    eax, 16                  ;U:38 (C4) Get int part
                mov    ebp, T09_MapOffset       ;V:38

                shl    ebx, 16                  ;U:39 (C4) Get fractional part
                add    esi, eax                 ;V:39 Add U-offset

                shl    ecx, 16                  ;U:40 (D5)
		shr	ebp, 1
                add    esi, ebp                 ;V:40 Add textureoffset

                mov    edx, LeftNdZ


;ÄÄÄÄ Get next Z...
                fld1                 ;1    VL   UL   1/ZR U/ZR V/ZR
                fdiv   st, st(3)     ;ZR   VL   UL   1/ZR U/ZR V/ZR


;eax = generally used
;ebx = u-fraction
;ecx = v-fraction
;edx = 1/Z interpolation
;esi = tmap pointer
;edi = dbuf pointer
;ebp = u-fraction increase

		mov	ebp, 16
InnerLoop_16:
                cmp    	edx, [edi*4 + 12345678h]
Offs16_Z1:      jbe    	Dump16
;		mov	[edi*4 + 12345678h], edx
Offs16_Z2:

                push   	ecx				;U
		push	ebx				;V

;---- Calculating displacement-offset...
		xor	ebx, ebx			;U
		xor	eax, eax			;U

		mov	al, BYTE PTR [esi*2 + 1]	;U
		mov     bl, BYTE PTR [esi*2]		;V

		mov	ebx, YLookup[ebx*4]
		mov	ecx, CurrentY

		add	eax, CurrentX
		add	ebx, ecx

							;Checking left/upper side

		cmp	eax, 12345678h			;Checking left side
Offs16_XMin1:
		jae	LeftOK_16
		mov	eax, 12345678h
Offs16_XMin2:
LeftOK_16:
		cmp	ebx, 12345678h			;Checking top side
Offs16_YMin1:
		jae 	TopOK_16
		mov	ebx, 12345678h
Offs16_YMin2:
TopOK_16:





		cmp	eax, 12345678h			;Checking right side
Offs16_XMax1:
		jle	RightOK_16
		mov	eax, 12345678h
Offs16_XMax2:
RightOK_16:
		cmp	ebx, 12345678h			;Checking lower side
Offs16_YMax1:
		jle 	LowerOK_16
		mov	ebx, 12345678h
Offs16_YMax2:
LowerOK_16:
		add	ebx, eax

		xor	eax, eax
		xor	ecx, ecx

		cmp	edx, [ebx*4 + 12345678h]
Offs16_Z3:      jae	ZOk_16
		mov	ebx, edi
ZOk_16:

;---- Getting colors and shading them...
		mov	al, [ebx*4 + 12345678h]		;U: R
Offs16_T09DBufR:
		mov	cl, [ebx*4 + 12345678h]		;V: G
Offs16_T09DBufG:
		mov	bl, [ebx*4 + 12345678h]		;U:
Offs16_T09DBufB:
		nop					;V:

;		and	ebx, 011111111b			;U:
;		mov     al, RLookup[eax]		;V: R

;		mov	cl, GLookup[ecx]		;U: G
		mov	[edi*4 + 12345678h], al		;V: R
Offs16_R:
		mov	[edi*4 + 12345678h], cl		;U: G
Offs16_G:
;		mov     bl, BLookup[ebx]		;V: B

		mov	[edi*4 + 12345678h], bl		;U:
Offs16_B:

		pop	ebx                     ;U:1
                pop    	ecx                     ;V:1
Dump16:
                add    	ecx, VFrac              ;U:2-3 cf set if ecx > 0.5
                lea    	edi, [edi + 1]          ;V:2-3

                sbb    	eax, eax                ;U:4-5 1 if carry
                add    	ebx, UFrac              ;V:4-5 cf set if ebx > 0.5

                adc    	esi, [Slope + eax*4]    ;U:6-8 increment texturepointer
                add    	edx, NZSlopeInt         ;U:6-8

		inc	CurrentX

                dec    	ebp			;U:9
                jnz    	InnerLoop_16		;V:9

                mov    LeftNdZ, edx

                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                jbe    LeftOver2
                jmp    Loop16



;ÄÄÄÄ Write leftover pixels...

LeftOver1:

;ÄÄÄÄ Get U and V left
                fld    st            ;ZL   ZL   ÄÄÄÄ U/ZL V/ZL
                fmul   st, st(3)     ;UL   ZL   ÄÄÄÄ U/ZL V/ZL
                fxch   st(1)         ;ZL   UL   ÄÄÄÄ U/ZL V/ZL
                fmul   st, st(4)     ;VL   UL   ÄÄÄÄ U/ZL V/ZL

                ffree  st(4)         ;VL   UL   ÄÄÄÄ U/ZL
                ffree  st(3)         ;VL   UL   ÄÄÄÄ


                fld    EdgeRight.VdZ ;V/ZR VL   UL   ÄÄÄÄ
                fld    EdgeRight.UdZ ;U/ZR V/ZR VL   UL   ÄÄÄÄ

                fstp   st(5)         ;V/ZR VL   UL   ÄÄÄÄ U/ZR
                fstp   st(5)         ;VL   UL   ÄÄÄÄ U/ZR V/ZR

                fld    EdgeRight.NdZ ;1/ZR VL   UL   ÄÄÄÄ U/ZR V/ZR
                fdivr  One           ;ZR   VL   UL   ÄÄÄÄ U/ZR V/ZR


LeftOver2:

;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ OPTIMIZED BLOCK C (PL_Cx.TXT) ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³* 38 cycles
;³* Calculating U and V right
;³* Calculating DeltaU and DeltaV
;³* Calculating slopes and stores them as 16.16 fixed point integers
;³* Saving U and V left as 16.16 fixed point integers
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

                                              ;st0  st1  st2  st3  st4  st5  st6
                                              ;ZR   VL   UL   ---- UZR  VZR

                fld    st            ;vr0->vr1 ZR   ZR   VL   UL   ---- UZR  VZR
                fmul   st, st(5)     ;vr1      UR   ZR   VL   UL   ---- UZR  VZR
                fild   ScanLength    ;->vr2    SL   UR   ZR   VL   UL   ---- UZR  VZR
                fxch   st(2)         ;         ZR   UR   SL   VL   UL   ---- UZR  VZR
                fmul   st, st(7)     ;vr0      VR   UR   SL   VL   UL   ---- UZR  VZR
                fxch   st(1)         ;         UR   VR   SL   VL   UL   ---- UZR  VZR
                fsub   st, st(4)     ;vr1      UD   VR   SL   VL   UL   ---- UZR  VZR
                fxch   st(4)         ;         UL   VR   SL   VL   UD   ---- UZR  VZR
                fmul   Fixed16_16    ;vr3      ULF  VR   SL   VL   UD   ---- UZR  VZR
                fxch   st(1)         ;         VR   ULF  SL   VL   UD   ---- UZR  VZR
                fsub   st, st(3)     ;vr0      VD   ULF  SL   VL   UD   ---- UZR  VZR
                fxch   st(3)         ;         VL   ULF  SL   VD   UD   ---- UZR  VZR
                fmul   Fixed16_16    ;vr4      VLF  ULF  SL   VD   UD   ---- UZR  VZR
                fxch   st(1)         ;         ULF  VLF  SL   VD   UD   ---- UZR  VZR
                fistp  ULeftFixed    ;vr3      VLF  SL   VD   UD   ---- UZR  VZR
                fistp  VLeftFixed    ;vr4      SL   VD   UD   ---- UZR  VZR
                fdivr  Fixed16_16    ;vr2      FP/L VD   UD   ---- UZR  VZR
                fmul   st(1), st     ;vr0      FP/L VS   UD   ---- UZR  VZR
                fmulp  st(2), st     ;vr1      VS   US   ---- UZR  VZR
                fistp  VSlopeFixed   ;vr0      US   ---- UZR  VZR
                fistp  USlopeFixed   ;vr1      ---- UZR  VZR



;ÄÄÄÄ Setting up slopes and initial values...
                mov    eax, USlopeFixed         ;U:1 (A1) !!!Cache-delay!!!
                mov    ecx, VSlopeFixed         ;V:2 (B1) /

                mov    ebx, eax                 ;U:3 (A2) Take copy
                mov    edx, ecx                 ;V:3 (B2) /

                sal    ebx, 16                  ;U:4 (A3) Get fractional part
                mov    esi, VLeftFixed          ;V:4 (D1)

                sar    eax, 16                  ;U:5 (A3) Get int part
                mov    UFrac, ebx               ;V:5 (A4) Save fractional part

                sal    edx, 16                  ;U:6 (B3)
                add    esi, 8000h               ;V:6 (D2)

                sar    ecx, 16                  ;U:7 (B3)
                mov    VFrac, edx               ;V:7 (B4)

                imul   ecx, T09_MapWidth        ;U:8-19 (B4)

                add    ecx, eax                 ;U:20 (AB5)
                mov    ebp, T09_MapWidth        ;V:20 (AB6)

                mov    Slope, ecx               ;U:21 (AB6) Save normal slope
                mov    eax, ULeftFixed          ;V:21 (C1)

                add    ecx, ebp                 ;U:22 (AB7)
                add    eax, 8000h               ;V:22 (C2) add 0.5 (16.16 fixed point)

                mov    ebx, eax                 ;U:23 (C3) Take copy
                mov    [Slope - 4], ecx         ;V:23 (AB8) Save slope + 1 scanline

                shl    ebx, 16                  ;U:24 (C4) Get fractional part
                mov    ecx, esi                 ;V:24 (D3)

                shr    esi, 16                  ;U:25 (D4)
                mov    ebp, T09_MapOffset       ;V:25

                imul   esi, T09_MapWidth        ;U:26-37 (D5)

		shr	ebp, 1                  ;U:38
                mov    edx, LeftNdZ             ;V:38

                shr    eax, 16                  ;U:39 (C4) Get int part
                add    esi, ebp                 ;V:39 Add textureoffset

                shl    ecx, 16                  ;U:40 (D5)
                mov    ebp, ScanLength

                add    esi, eax                 ;V:40 Add U-offset
                inc    ebp


                                                ;esi = mempointer
                                                ;ebx = U fractional part
                                                ;ecx = V fractional part



LeftOverLoop:
                cmp    	edx, [edi*4 + 12345678h]
OffsXX_Z1:      jbe    	DumpXX
;		mov	[edi*4 + 12345678h], edx
OffsXX_Z2:

                push   	ecx				;U
		push	ebx				;V

;---- Calculating displacement-offset...
		xor	ebx, ebx			;U
		xor	eax, eax			;U

		mov	al, BYTE PTR [esi*2 + 1]	;U
		mov     bl, BYTE PTR [esi*2]		;V

		mov	ebx, YLookup[ebx*4]
		mov	ecx, CurrentY

		add	eax, CurrentX
		add	ebx, ecx

							;Checking left/upper side

		cmp	eax, 12345678h			;Checking left side
OffsXX_XMin1:
		jge	LeftOK_XX
		mov	eax, 12345678h
OffsXX_XMin2:
LeftOK_XX:
		cmp	ebx, 12345678h			;Checking top side
OffsXX_YMin1:
		jge 	TopOK_XX
		mov	ebx, 12345678h
OffsXX_YMin2:
TopOK_XX:





		cmp	eax, 12345678h			;Checking right side
OffsXX_XMax1:
		jle	RightOK
		mov	eax, 12345678h
OffsXX_XMax2:
RightOK:
		cmp	ebx, 12345678h			;Checking lower side
OffsXX_YMax1:
		jle 	LowerOK
		mov	ebx, 12345678h
OffsXX_YMax2:
LowerOK:
		add	ebx, eax

		xor	eax, eax
		xor	ecx, ecx

		cmp	edx, [ebx*4 + 12345678h]
OffsXX_Z3:      jae	ZOk
		mov	ebx, edi
ZOk:

;---- Getting colors and shading them...
		mov	al, [ebx*4 + 12345678h]		;U: R
OffsXX_T09DBufR:
		mov	cl, [ebx*4 + 12345678h]		;V: G
OffsXX_T09DBufG:
		mov	bl, [ebx*4 + 12345678h]		;U:
OffsXX_T09DBufB:
		nop					;V:

		and	ebx, 011111111b			;U:
 ;		mov     al, RLookup[eax]		;V: R

;		mov	cl, GLookup[ecx]		;U: G
		mov	[edi*4 + 12345678h], al		;V: R
OffsXX_R:
		mov	[edi*4 + 12345678h], cl		;U: G
OffsXX_G:
;		mov     bl, BLookup[ebx]		;V: B

		mov	[edi*4 + 12345678h], bl		;U:
OffsXX_B:

		pop	ebx                     ;U:1
                pop    	ecx                     ;V:1
DumpXX:
                add    	ecx, VFrac              ;U:2-3 cf set if ecx > 0.5
                lea    	edi, [edi + 1]          ;V:2-3

                sbb    	eax, eax                ;U:4-5 1 if carry
                add    	ebx, UFrac              ;V:4-5 cf set if ebx > 0.5

                adc    	esi, [Slope + eax*4]    ;U:6-8 increment texturepointer
                add    	edx, NZSlopeInt         ;U:6-8

		inc	CurrentX

                dec    	ebp			;U:9
                jnz    	LeftOverLoop		;V:9


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


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ OPTIMIZED BLOCK D (PL_Dx.TXT) ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;³* 32 cycles
;³* Updating variables for both edges
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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

		add    CurrentY, ebp

                add    PixelPtr, ebp       ;U:2-3
                jmp    ScanLoop              ;V:2


DoEdge12:       or     edx, edx
                jnz    Done

                mov    ebp, _V1Offs
                mov    edi, _V2Offs

                                                ;FPU stack is empty

;ÄÄÄÄ Get 1/DeltaY...
                fld    [edi].V_Y2D      ;Y2
                fsub   [ebp].V_Y2D      ;YD
                fdivr  One            ;YNDA


;ÄÄÄÄ Get new U/ZSlope A...
                fld    UZ2            ;UZ2  YNDA
                fsub   UZ1            ;UZDA YNDA
                fmul   st, st(1)      ;UZSA YNDA
                fstp   UZSlopeA       ;YNDA


;ÄÄÄÄ Get new V/ZSlope A...
                fld    VZ2            ;VZ2  YNDA
                fsub   VZ1            ;VZDA YNDA
                fmul   st, st(1)      ;VZSA YNDA
                fstp   VZSlopeA       ;YNDA


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
T09_Tri       ENDP



T09_Init      PROC
                mov    	eax, SYS_ZBufOffs

                mov    	d[OffsXX_Z1 - 4], eax
;                mov     d[OffsXX_Z2 - 4], eax
                mov    	d[OffsXX_Z3 - 4], eax

                mov    	d[Offs16_Z1 - 4], eax
;                mov     d[Offs16_Z2 - 4], eax
                mov    	d[Offs16_Z3 - 4], eax




;ÄÄÄÄ Set DBufOffs (edi)
                mov     eax, SYS_DBufOffs

                mov     d[OffsXX_B - 4], eax
		inc	eax
                mov     d[OffsXX_G - 4], eax
		inc	eax
                mov     d[OffsXX_R - 4], eax

		mov	eax, SYS_DBufOffs
                mov     d[Offs16_B - 4], eax
		inc	eax
                mov     d[Offs16_G - 4], eax
		inc	eax
                mov     d[Offs16_R - 4], eax


;---- Setting screenlimits...
		fld	SYS_FPClipLeft
		fistp	FPDump
		mov	eax, FPDump

		mov	d[OffsXX_XMin1 - 4], eax
		mov	d[OffsXX_XMin2 - 4], eax
		mov	d[Offs16_XMin1 - 4], eax
		mov	d[Offs16_XMin2 - 4], eax

		fld	SYS_FPClipRight
		fistp	FPDump
		mov	eax, FPDump

		mov	d[OffsXX_XMax1 - 4], eax
		mov	d[OffsXX_XMax2 - 4], eax
		mov	d[Offs16_XMax1 - 4], eax
		mov	d[Offs16_XMax2 - 4], eax

		fld	SYS_FPClipTop
		fistp	FPDump
		mov	ecx, FPDump
		mov	ebx, SYS_XRes
		imul	ebx, ecx

		mov	d[OffsXX_YMin1 - 4], ebx
		mov	d[OffsXX_YMin2 - 4], ebx
		mov	d[Offs16_YMin1 - 4], ebx
		mov	d[Offs16_YMin2 - 4], ebx

		fld	SYS_FPClipBottom
		fistp	FPDump
		mov	ecx, FPDump
		mov	ebx, SYS_XRes
		imul	ebx, ecx

		mov	d[OffsXX_YMax1 - 4], ebx
		mov	d[OffsXX_YMax2 - 4], ebx
		mov	d[Offs16_YMax1 - 4], ebx
		mov	d[Offs16_YMax2 - 4], ebx




;---- Building y*XRes lookup
		xor	ecx, ecx
YLoop:
		mov	eax, ecx

		imul	eax, SYS_XRes
;		shl	eax, 2

		mov	YLookup[ecx*4], eax

		inc	ecx
		cmp	ecx, 64
		jb	YLoop

;---- Setting T09_DBuf-thingy...
		mov	eax, T09_DBuf
		mov	d[OffsXX_T09DBufB - 4], eax
		mov	d[Offs16_T09DBufB - 4], eax
		inc	eax
		mov	d[OffsXX_T09DBufG - 4], eax
		mov	d[Offs16_T09DBufG - 4], eax
		inc	eax
		mov	d[OffsXX_T09DBufR - 4], eax
		mov	d[Offs16_T09DBufR - 4], eax

                ret
T09_Init      ENDP

END
