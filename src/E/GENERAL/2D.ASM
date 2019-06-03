.386
.model flat, C
.code

include w:\general\var.inc

public PIC
public BLN
public FOG
public INV

ALIGN 4

PIC_Data      STRUC

PIC_Type	DD ?
PIC_Plugins	DD 16 DUP (?)
PIC_XPos	DD ?
PIC_YPos	DD ?
PIC_Width 	DD ?
PIC_Heigth	DD ?
PIC_Picture	DD ?

	      ENDS




PIC	      PROC
		mov	ebp, SYS_OOffs

		mov	esi, [ebp].PIC_Picture
		mov	edi, SYS_DBufOffs

		mov	eax, [ebp].PIC_YPos
		imul	eax, SYS_XRes
		add	eax, [ebp].PIC_XPos
		shl	eax, 2
		add	edi, eax

		mov	edx, [ebp].PIC_Heigth
		xor	ebx, ebx
PIC_YLoop:
		mov	ecx, [ebp].PIC_Width
PIC_XLoop:

		mov	ebx, [esi]

		or	ebx, ebx
		jz	PIC_Dump

		mov	[edi], ebx
PIC_Dump:

		add	esi, 4
		add	edi, 4
		dec	ecx
		jnz	PIC_XLoop

		mov	eax, SYS_XRes
		sub	eax, [ebp].PIC_Width
		shl	eax, 2
		add	edi, eax

		dec	edx
		jnz	PIC_YLoop

		ret
PIC	      ENDP


ALIGN 4

BLN_Data      STRUC

BLN_Type	DD ?
BLN_Plugins	DD 16 DUP (?)

BLN_R  		DD ?
BLN_G  		DD ?
BLN_B  		DD ?

BLN_Opacity0	DD ?
BLN_Opacity1	DD ?


BLN_StartFlag	DD ?
BLN_StartTime	DD ?
BLN_FadingTime  DD ?

	      ENDS

RLookup      	DB 256 DUP (0)
GLookup       	DB 256 DUP (0)
BLookup       	DB 256 DUP (0)

TSlope32	DD ?
TSlope16	DD ?

_256         	DD 255.0

Fixed1616	DD 65536.0

FPDump		DD 0
Opacity		DD 0




BLN	      PROC
		mov	esi, SYS_OOffs

		cmp	[esi].BLN_StartFlag, 0
		jne	BLN_Initialized
		ret
BLN_Initialized:


		mov	eax, [esi].BLN_StartTime
		add	eax, [esi].BLN_FadingTime
		sub	eax, SYS_TimerTick
		cmp	eax, 0
		jg	BLN_FadeMore
		xor	eax, eax
BLN_FadeMore:
		mov	FPDump, eax
		fild	FPDump
		fidiv	[esi].BLN_FadingTime

		fld1
		fsub	st, st(1)
		fmul    [esi].BLN_Opacity1
		fxch	st(1)
		fmul	[esi].BLN_Opacity0
		faddp	st(1), st
		fstp	Opacity





;---- Creating red lookup
		fldcw   FPU_Round

		mov    	ecx, 256                 ;loopcounter
		xor	edi, edi

		fld	[esi].BLN_R
		fmul	_256

LookupR:
		dec	ecx
		mov	FPDump, edi
		fild	FPDump
		fsubr   st, st(1)
		fmul	Opacity
		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	RLookup[edi], al

		inc	edi
		or 	ecx, ecx
                jnz    	LookupR

		fstp	FPDump



;---- Creating green lookup
		mov    	ecx, 256                  ;loopcounter
		xor	edi, edi

		fld	[esi].BLN_G
		fmul	_256

LookupG:
		dec	ecx
		mov	FPDump, edi
		fild	FPDump
		fsubr   st, st(1)
		fmul	Opacity
		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	GLookup[edi], al

		inc	edi
		or 	ecx, ecx
                jnz    	LookupG

		fstp	FPDump


;---- Creating blue lookup
		mov    	ecx, 256                  ;loopcounter
		xor	edi, edi

		fld	[esi].BLN_B
		fmul	_256

LookupB:
		dec	ecx
		mov	FPDump, edi
		fild	FPDump
		fsubr   st, st(1)
		fmul	Opacity
		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	BLookup[edi], al

		inc	edi
		or 	ecx, ecx
                jnz    	LookupB

		fstp	FPDump



;---- Blending doublebuffer...
		mov	edi, SYS_DBufOffs
		mov	ecx, SYS_XRes
		imul	ecx, SYS_YRes

		xor	eax, eax
		xor	ebx, ebx
		xor	edx, edx

BLN_Loop:
                mov    	al, [edi + 2]
		mov	bl, [edi + 1]
		mov	dl, [edi + 0]

		mov	al, RLookup[eax]
		mov	bl, GLookup[ebx]
		mov	dl, BLookup[edx]

		mov	[edi + 2], al
		mov	[edi + 1], bl
		mov	[edi + 0], dl

		add	edi, 4

		dec	ecx
		jnz	BLN_Loop

BLN_Done:

		ret
BLN	      ENDP



FOG_Data      STRUC

FOG_Type	DD ?
FOG_Plugins	DD 16 DUP (?)

FOG_R  		DD ?
FOG_G  		DD ?
FOG_B  		DD ?

FOG_Distance	DD ?

	      ENDS

ALIGN 4

FOG_LUTR	DD 32*32 DUP (0)
FOG_LUTG	DD 32*64 DUP (0)
FOG_LUTB	DD 32*32 DUP (0)

_16p0		DD 31.0
_31p0		DD 31.0
_63p0		DD 63.0
FPTemp1		DD 0
FPTemp2		DD 0

FOG           PROC
		mov	esi, SYS_OOffs

		fldcw	FPU_Round

;---- Setting red lut...
		mov	edi, OFFSET FOG_LUTR
		xor	edx, edx

RColLoop:
		xor	ecx, ecx

RTranspLoop:
						;st0  st1  st2  st3
		fld	[esi].FOG_R		;R
		fmul	_31p0
		mov	FPTemp1, edx
		fisub	FPTemp1  		;R-C

		fdiv	_16p0   		;slope
		mov	FPTemp2, ecx
		fimul	FPTemp2
		fiadd   FPTemp1

		fistp	FPTemp1 		;round and store

		mov	eax, FPTemp1    	;store into lut...
		shl	eax, 11
		mov	[edi], eax		;

		add	edi, 4

		inc	ecx
		cmp	ecx, 32
		jb	RTranspLoop

		inc	edx
		cmp	edx, 32
		jb	RColLoop


;---- Setting green lut...
		xor	edx, edx

	GColLoop:
		xor	ecx, ecx

	GTranspLoop:
						;st0  st1  st2  st3
		fld	[esi].FOG_G		;R
		fmul	_63p0
		mov	FPTemp1, edx
		fisub	FPTemp1  		;R-C

		fdiv	_16p0   		;slope
		mov	FPTemp2, ecx
		fimul	FPTemp2
		fiadd   FPTemp1

		fistp	FPTemp1 		;round and store

		mov	eax, FPTemp1    	;store into lut...
		shl	eax, 5
		mov	[edi], eax		;

		add	edi, 4

		inc	ecx
		cmp	ecx, 32
		jb	GTranspLoop

		inc	edx
		cmp	edx, 64
		jb	GColLoop


;---- Setting blue lut...
		xor	edx, edx

	BColLoop:
		xor	ecx, ecx

	BTranspLoop:
						;st0  st1  st2  st3
		fld	[esi].FOG_B		;R
		fmul	_31p0
		mov	FPTemp1, edx
		fisub	FPTemp1  		;R-C

		fdiv	_16p0   		;slope
		mov	FPTemp2, ecx
		fimul	FPTemp2
		fiadd   FPTemp1

		fistp	FPTemp1 		;round and store

		mov	eax, FPTemp1    	;store into lut...
		mov	[edi], eax		;

		add	edi, 4

		inc	ecx
		cmp	ecx, 32
		jb	BTranspLoop

		inc	edx
		cmp	edx, 32
		jb	BColLoop



;---- Lookuptables done...
		mov	eax, SYS_ZBufOffs
		mov	d[FOG_ZBuf - 4], eax

		mov	eax, SYS_DBufOffs
		mov	d[FOG_DBuf1 - 4], eax
		mov	d[FOG_DBuf2 - 4], eax

		mov	ecx, [esi].FOG_Distance
		mov	ebx, 011111b
		shl	ebx, cl
		mov	d[FOG_Distance1 - 4], ebx
		mov	d[FOG_Distance2 - 4], ebx

		xor	esi, esi

		mov	edi, SYS_XRes
		imul	edi, SYS_YRes

;---- Horisontal loop...

FOG_Loop:
		mov     ebx, [esi*4 + 12345678h]	;Load transparency-value
FOG_ZBuf:

		xor	edx, edx
		cmp	ebx, 12345678h
FOG_Distance1:
		seta    dl
		dec	edx

		and	ebx, 12345678h
FOG_Distance2:
		shr	ebx, cl
		mov	eax, 31
		sub	eax, ebx
		and	eax, edx
		shl	eax, 2


		mov	dx, [esi*2 + 12345678h]		;Get pixel
FOG_DBuf1:

		mov	ebp, 01111100000000000b ;Get new red value
		mov	ebx, 011111100000b	;Get new green value
		and	ebp, edx
		and	ebx, edx
		shr	ebp, 6
		and	edx, 011111b		;Get new blue value
		shl	edx, 5
		mov	ebp, FOG_LUTR[ebp*4 + eax]
		mov     ebx, FOG_LUTG[ebx*4 + eax]
		or	ebp, FOG_LUTB[edx*4 + eax]
		or	ebp, ebx

		mov	[esi*2 + 12345678h], bp
FOG_DBuf2:

		inc	esi
		dec	edi
		jnz	FOG_Loop

		ret
FOG           ENDP












INV_Data      STRUC

INV_Type	DD ?
INV_Plugins	DD 16 DUP (?)

INV_StartFlag	DD ?
INV_StartTime	DD ?
INV_FadingTime  DD ?

INV_RMul	DD ?
INV_GMul	DD ?
INV_BMul	DD ?

INV_Opacity0	DD ?
INV_Opacity1	DD ?

	      ENDS

INV_Opacity	DD ?

INV_RLUT 	DB 256 DUP (?)
INV_GLUT	DB 256 DUP (?)
INV_BLUT	DB 256 DUP (?)

_31		DD 31.0

INV	      PROC
		mov	esi, SYS_OOffs

		cmp	[esi].INV_StartFlag, 0
		jne	INV_Initialized
		ret
INV_Initialized:


		mov	eax, [esi].INV_StartTime
		add	eax, [esi].INV_FadingTime
		sub	eax, SYS_TimerTick
		cmp	eax, 0
		jg	INV_FadeMore
		xor	eax, eax
INV_FadeMore:
		mov	FPDump, eax
		fild	FPDump
		fidiv	[esi].INV_FadingTime

		fld1
		fsub	st, st(1)
		fmul    [esi].INV_Opacity1
		fxch	st(1)
		fmul	[esi].INV_Opacity0
		faddp	st(1), st
		fstp	INV_Opacity


;		cmp	[esi].INV_StartFlag, 0
;		je	INV_Done

;		mov	[esi].INV_StartFlag, 1
;		mov	eax, SYS_TimerTick
;		mov	[esi].INV_StartTime, eax
;INV_Initialized:


;		mov	eax, [esi].INV_StartTime
;		add	eax, [esi].INV_FadingTime
;		sub	eax, SYS_TimerTick
;		cmp	eax, 0
;		jg	INV_FadeMore
;		xor	eax, eax
;INV_FadeMore:
;		mov	FPDump, eax
;		fild	FPDump
;		fidiv	[esi].INV_FadingTime
;		fstp	INV_Opacity





;---- Creating red lookup
		fldcw   FPU_Round

		mov    	ecx, 256                 ;loopcounter
		xor	edi, edi

INV_RLoop:
		mov	FPDump, edi
		fld	_256
		fisub	FPDump
		fmul	[esi].INV_RMul
		fisub	FPDump
		fmul	INV_Opacity

		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	INV_RLUT[edi], al

		inc	edi

		dec	ecx
                jnz    	INV_RLoop


;---- Creating green lookup
		mov    	ecx, 256                 ;loopcounter
		xor	edi, edi

INV_GLoop:
		mov	FPDump, edi
		fld	_256
		fisub	FPDump
		fmul	[esi].INV_GMul
		fisub	FPDump
		fmul	INV_Opacity

		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	INV_GLUT[edi], al

		inc	edi

		dec	ecx
                jnz    	INV_GLoop


;---- Creating red lookup
		mov    	ecx, 256                 ;loopcounter
		xor	edi, edi

INV_BLoop:
		mov	FPDump, edi
		fld	_256
		fisub	FPDump
		fmul	[esi].INV_BMul
		fisub	FPDump
		fmul	INV_Opacity

		fiadd	FPDump
		fistp	FPDump

		mov	eax, FPDump
		mov	INV_BLUT[edi], al

		inc	edi

		dec	ecx
                jnz    	INV_BLoop



;---- Blending doublebuffer...
		mov	edi, SYS_DBufOffs
		mov	ecx, SYS_XRes
		imul	ecx, SYS_YRes

		xor	eax, eax
		xor	ebx, ebx
		xor	edx, edx

INV_Loop:
                mov    	al, [edi + 2]
		mov	bl, [edi + 1]
		mov	dl, [edi + 0]

                mov    	al, INV_RLUT[eax]
                mov     bl, INV_GLUT[ebx]
		mov	dl, INV_BLUT[edx]

                mov    	[edi + 2], al
		mov	[edi + 1], bl
		mov	[edi + 0], dl

		add	edi, 4

		dec	ecx
		jnz	INV_Loop

INV_Done:



		ret
INV	      ENDP

END

