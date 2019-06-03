;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : GLOW.ASM
;쿌uthor       : Cyberfish/TLS
;쿗ast update  : 14.03.1998
;쿌ction       : Plugin/part for "glow"-map. For lights and stuff.
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.model flat, C
.code

public FLR_Draw

include w:\general\var.inc
include w:\plugins\flare.inc

ALIGN 4

Fixed032        DD 4294967295.0
_1p0		DD 1.0
_0p5		DD 0.5
_65536p0	DD 65536.0


;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : FLR_Draw
;쿔D           : FLR
;쿟ype         : Procedure
;쿗ast update  : 22.01.1999
;쿌ction       : Draws a lensflare
;쿚ptimized    : -
;�
;쿔nput variables : esi
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

ALIGN 4

X0		DD ?
Y0		DD ?
X1		DD ?
Y1		DD ?
U0		DD ?
V0		DD ?
U0Frac		DD ?
V0Frac		DD ?

USlope		DD ?
VSlope		DD ?
UFrac		DD ?
VFrac		DD ?

X0Int		DD ?
Y0Int		DD ?
X1Int		DD ?
Y1Int		DD ?

MapXSizeInt	DD ?

						;colors*components
_16p0		DD 16.0
_31p0		DD 31.0
_256p0		DD 256.0
FPTemp1		DD 0
FPTemp2		DD 0

R		DD 0
G		DD 0
B		DD 0
RLookup		DB 256 DUP (?)
GLookup		DB 256 DUP (?)
BLookup		DB 256 DUP (?)


FBOCount	DD 57
FBOLevels	DD 0.016129
		DD 0.032258

FBO		DD  0, -6, 1

		DD  0, -5, 1

		DD -1, -4, 1
		DD  0, -4, 1
		DD  1, -4, 1

		DD -1, -3, 1
		DD  0, -3, 1
		DD  1, -3, 1

		DD -2, -2, 1
		DD -1, -2, 1
		DD  0, -2, 1
		DD  1, -2, 1
		DD  2, -2, 1

		DD -4, -1, 1
		DD -3, -1, 1
		DD -2, -1, 1
		DD -1, -1, 1
		DD  0, -1, 2
		DD  1, -1, 1
		DD  2, -1, 1
		DD  3, -1, 1
		DD  4, -1, 1

		DD -6,  0, 1
		DD -5,  0, 1
		DD -4,  0, 1
		DD -3,  0, 1
		DD -2,  0, 1
		DD -1,  0, 2
		DD  0,  0, 2
		DD  1,  0, 2
		DD  2,  0, 1
		DD  3,  0, 1
		DD  4,  0, 1
		DD  5,  0, 1
		DD  6,  0, 1

		DD -4,  1, 1
		DD -3,  1, 1
		DD -2,  1, 1
		DD -1,  1, 1
		DD  0,  1, 2
		DD  1,  1, 1
		DD  2,  1, 1
		DD  3,  1, 1
		DD  4,  1, 1

		DD -2,  2, 1
		DD -1,  2, 1
		DD  0,  2, 1
		DD  1,  2, 1
		DD  2,  2, 1

		DD -1,  3, 1
		DD  0,  3, 1
		DD  1,  3, 1

		DD -1,  4, 1
		DD  0,  4, 1
		DD  1,  4, 1

		DD  0,  5, 1

		DD  0,  6, 1







ClipLeft	DD 0
ClipRight	DD 0
ClipTop		DD 0
ClipBottom	DD 0


FLR_Draw      PROC
		fldcw	FPU_Round

		fld	[esi].FLR_Intensity

		mov	eax, [esi].FLR_Flags
		and	eax, FLR_FlagFadeBehindObject
		jz	NoFadeBehindObject

;---- Calculating fade-behind-object stuff.
		fld	SYS_FPClipLeft
		fistp	ClipLeft
		fld	SYS_FPClipRight
		fistp	ClipRight
		fld	SYS_FPClipTop
		fistp	ClipTop
		fld	SYS_FPClipBottom
		fistp	ClipBottom

		fldz
		mov	ecx, 57
		mov	ebp, OFFSET FBO
		mov	edx, [esi].FLR_Z
FBOLoop:
		mov	eax, [esi].FLR_X
		add	eax, [ebp + 0]

		mov	ebx, [esi].FLR_Y
		add	ebx, [ebp + 4]

		cmp	eax, ClipLeft
		jl	NoAdd
		cmp	eax, ClipRight
		ja	NoAdd
		cmp	ebx, ClipTop
		jl	NoAdd
		cmp	ebx, ClipBottom
		ja	NoAdd

		imul	ebx, SYS_XRes
		add	ebx, eax
		shl	ebx, 2
		add	ebx, SYS_ZBufOffs

		cmp	edx, [ebx]
		jb	NoAdd

		mov	eax, [ebp + 8]

		fadd    FBOLevels[eax*4]

NoAdd:
		add	ebp, 12
		dec	ecx
		jnz	FBOLoop


		;No intensity?
		fst	FPTemp1
		mov	eax, FPTemp1
		or	eax, eax
		jz	FLRDone

		fmulp	st(1), st

;---- Calculating colorintensity...

NoFadeBehindObject:
		fld	[esi].FLR_R
		fmul	st, st(1)
		fmul	_256p0
		fistp	R

		fld	[esi].FLR_G
		fmul	st, st(1)
		fmul	_256p0
		fistp	G

		fld	[esi].FLR_B
		fmulp   st(1), st
		fmul	_256p0
		fistp	B

;---- Building lookups...
                mov     eax, 3Fh           	;R 0.5 fixed point
		mov	ebx, 3Fh                ;G
		mov	ecx, 3Fh                ;B

                xor     ebp, ebp               	;loopcounter/mempointer
Lookups:
		mov	edx, eax
		cmp	edx, 0FFFFh
		jb	ROK
		mov	edx, 0FFFFh
ROK:
                mov    	RLookup[ebp], dh

		mov	edx, ebx
		cmp	edx, 0FFFFh
		jb	GOK
		mov	edx, 0FFFFh
GOK:
                mov    	GLookup[ebp], dh

		mov	edx, ecx
		cmp	edx, 0FFFFh
		jb	BOK
		mov	edx, 0FFFFh
BOK:
                mov    	BLookup[ebp], dh

                add    	eax, R
		add	ebx, G
		add	ecx, B

                inc 	ebp
		cmp	ebp, 256
		jb	Lookups

;---- Lookuptables done...

                                              	;st0  st1  st2  st3  st4  st5  st6  st7
;---- Projecting X-coordinates...
		fld	[esi].FLR_XMin
		fcom    SYS_FPClipLeft          ;clipping?
		fstsw	ax
		sahf
		jae     NoClipLeft

		fstp	X0Int			;Dump...
		fld	SYS_FPClipLeft
NoClipLeft:

		fldcw	FPU_RoundUp
		fistp	X0Int


		fld	[esi].FLR_XMax
		fcom    SYS_FPClipRight         ;clipping?
		fstsw	ax
		sahf
		jbe     NoClipRight

		fstp	X1Int			;Dump...
		fld	SYS_FPClipRight
NoClipRight:

		fldcw	FPU_RoundDown
		fistp	X1Int


;---- Projecting Y-coordinates...
		fld	[esi].FLR_YMax
		fcom    SYS_FPClipBottom        ;clipping?
		fstsw	ax
		sahf
		jbe     NoClipBottom

		fstp	Y1Int			;Dump...
		fld	SYS_FPClipBottom
		fadd	_1p0
NoClipBottom:

		fldcw	FPU_RoundDown
		fistp	Y1Int

		fld	[esi].FLR_YMin
		fcom    SYS_FPClipTop           ;clipping?
		fstsw	ax
		sahf
		jae     NoClipTop

		fstp	Y0Int
		fld	SYS_FPClipTop
NoClipTop:

		fldcw	FPU_RoundUp
		fistp	Y0Int

;---- Getting mapping slopes and mapcoords...
		fld     [esi].FLR_XMax
		fsub	[esi].FLR_XMin
		fld	[esi].FLR_MapWidth
		fdivrp  st(1), st		;USlope

		fild	X0Int			;subpixel accuracy
		fsub	[esi].FLR_XMin            ;/
		fmul	st, st(1)               ;/
		fmul	_65536p0                ;16.16 fixed point
		fistp	U0

		fmul	_65536p0		;16.16 fixed point
		fistp	USlope

		fld	[esi].FLR_YMax
		fsub	[esi].FLR_YMin
		fld	[esi].FLR_MapHeigth
		fdivrp  st(1), st               ;VSlope

		fild	Y0Int			;subpixel accuracy
		fsub	[esi].FLR_YMin          ;/
		fmul	st, st(1)               ;/
		fmul	_65536p0                ;16.16 fixed point
		fistp	V0

		fmul	_65536p0                ;16.16 fixed point
		fistp	VSlope

		add	U0, 32768		;add 0.5 fixed point
		add	V0, 32768		;/

		mov	eax, U0			;Setting starting-frac U/V
		mov	edx, V0
		shl	eax, 16
		shl	edx, 16
		mov	U0Frac, eax
		mov	V0Frac, edx

		mov	eax, U0			;Setting starting-frac U/V
		mov	edx, V0
		shr	eax, 16
		shr	edx, 16
		mov	U0, eax
		mov	V0, edx

		mov	eax, USlope
		mov	edx, USlope
		shl	eax, 16
		shr	edx, 16
		mov	UFrac, eax		;Setting up 0.32 fixed frac
		mov	USlope, edx             ;...and int-part

		mov	eax, VSlope
		mov	edx, VSlope
		shl	eax, 16
		shr	edx, 16
		mov	VFrac, eax		;Setting up 0.32 fixed frac
		mov	VSlope, edx		;...and int-part


;---- Setting SMC-stuff...
		mov	eax, [esi].FLR_Map	;setting fixed mapoffset
		mov	d[MapOffsB - 4], eax
		inc	eax
		mov	d[MapOffsG - 4], eax
		inc	eax
		mov	d[MapOffsR - 4], eax

		mov	eax, SYS_DBufOffs	;setting fixed dbuf-offset
		mov	d[DBufB1 - 4], eax
		mov	d[DBufB2 - 4], eax
		inc	eax
		mov	d[DBufG1 - 4], eax
		mov	d[DBufG2 - 4], eax
		inc	eax
		mov	d[DBufR1 - 4], eax
		mov	d[DBufR2 - 4], eax

		fld	[esi].FLR_MapWidth
		fadd	_1p0
		fistp	MapXSizeInt

;---- Rendering-loops...
		mov	edx, Y1Int		;edx = y-counter
		sub	edx, Y0Int

		mov	ebp, VFrac		;ebp = V fractional part

YLoop:						;Vertical loop
		cmp	edx, 0
		jle	FLRDone

		mov	edi, Y0Int		;edi = dbuf/zbuf-pointer
		imul	edi, SYS_XRes
		add	edi, X0Int

		mov	esi, V0			;esi = map-pointer
		imul	esi, MapXSizeInt
		add	esi, U0

		mov	ecx, X1Int		;ecx = xcounter
		sub	ecx, X0Int

		mov	ebp, U0Frac

		push	edx
		xor	edx, edx
		xor	eax, eax
		xor	ebx, ebx

		cmp	ecx, 0
		jle	FLRDoneX

;---- Horisontal loop...
;ecx = xcounter
;edi = dbufpointer
;esi = tmappointer
;ebp = UFrac

XLoop:

		mov	al, [esi*4 + 12345678h]		;R
MapOffsR:
		mov	bl, [edi*4 + 12345678h]		;R
DBufR1:
		mov	dl, [esi*4 + 12345678h]		;G
MapOffsG:
		mov	bh, [edi*4 + 12345678h]		;G
DBufG1:

		mov	al, RLookup[eax]		;R
		mov	dl, GLookup[edx]		;G

		add	bl, al				;R

		sbb	al, al				;R
		add	bh, dl				;G

		sbb	dl, dl				;G
		or	bl, al				;R

		or	bh, dl				;G
		mov	[edi*4 + 12345678h], bl		;R
DBufR2:

		mov	[edi*4 + 12345678h], bh		;G
DBufG2:

;---- B
		mov	al, [esi*4 + 12345678h]		;B
MapOffsB:
		mov	bl, [edi*4 + 12345678h]		;B
DBufB1:
		mov	al, BLookup[eax]		;intensity
		add	bl, al				;add

		sbb	al, al				;overflow
		or	bl, al

		mov	[edi*4 + 12345678h], bl
DBufB2:


		add	ebp, UFrac
		adc	esi, USlope
		inc	edi
		dec	ecx
		jnz	XLoop

FLRDoneX:

		mov	eax, V0
		mov	ebx, V0Frac
		add	ebx, VFrac
		adc	eax, VSlope
		mov	V0, eax
		mov	V0Frac, ebx

		inc	Y0Int

		pop	edx
		dec	edx
		jmp	YLoop

FLRDone:

		ffree	st(0)
		ffree	st(1)
		ffree	st(2)
		ffree	st(3)
		ffree	st(4)

		ret
FLR_Draw      ENDP


END 
