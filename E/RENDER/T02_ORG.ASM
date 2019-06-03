;旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T02_ORG.ASM
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 08.01.99
;쿌ction       : Draws a perspective mapped triangle, 32 bits colors, z-buffer
;�
;쿙otes :
;쿟o be done:
;� -fix the last pixel, if scanlength=17
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model flat, C
.code

include w:\general\var.inc

public T02_Tri
public T02_Init
public T02_Call

Edge          STRUC

X               DD ?
UdZ             DD ?
VdZ             DD ?
NdZ             DD ?

Edge          ENDS


;컴컴 In-variables...
T02_MapOffset   DD 0                            ;Offset texturemap
T02_MapWidth    DD 0                            ;INT: Tmap width, in pixels

;컴컴 Locally used "constants"
Fixed16_16      DD 65536.0
Fixed16         DD 4096.0                       ;65536.0/ 16.0
;Fixed032        DD 4294967296.0
Sixteen         DD 16.0
One             DD 1.0
AlmostZero      DD 0.000000000000000000001

FPDump          DD 0







;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : ZClip02
;쿔D           :
;쿟ype         : Macro
;쿗ast update  : 12.07.1997
;쿌ction       : Z-clips a polygon pointed To by edi
;쿚ptimized    : ?
;�
;쿔nput variables : [eax]  = left edge
;�                  [ebx]  = right edge
;�
;쿙otes : Macros "Project_Vertex" and "Clip_Line" are macros used by
;퀃his routine.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

Project_Vertex Macro Vertex

                fld1
                fld    [Vertex].V_ZRotated
                fdivp  st(1), st
                fst    [Vertex].V_InvZ

                fld    [Vertex].V_XRotated
                fmul   SYS_XPerspective
                fmul   st, st(1)
                fadd   SYS_XCenter
                fstp   [Vertex].V_X2D

                fmul   [Vertex].V_YRotated
                fmul   SYS_YPerspective
                fadd   SYS_YCenter
                fstp   [Vertex].V_Y2D
	endm








Clip_Line macro From, To, Dest
local DontBreakF
	push edx

	fld	[To].V_ZRotated				;1
	fsub 	[From].V_ZRotated			;2

	;fadd	AlmostZero
	fld1	                                        ;3
	fdivrp st(1)                                    ;4
							;43
;--------------------------------------------------------------
									;Clock
	fld 	[From].V_ZRotated					;1
	fld 	[From].V_XRotated              				;2

	fsub 	[To].V_XRotated 	;X2-X1 				;3

	fld 	[From].V_YRotated              				;4

	fld 	FromU			;U2      			;5

	fld 	FromV			;V2    				;6
	fxch 	st(3)			;Get X2-X1			;6

	fmul 	st,st(5)		;(X2-X1)/(Z2-Z1)		;7
	fxch 	st(4)			;Get Z2				;7

	fsub 	SYS_ZClip		;Z2-256				;8
	fxch 	st(1)			;Get U2				;8

	fsub 	ToU			;U2-U1 				;9
	fxch 	st(2)			;Get Y2				;9

	fsub 	[To].V_YRotated 	;Y2-Y1				;10
	fxch 	st(4)			;Get (X2-X1)/(Z2-Z1)		;10

	fmul 	st,st(1)		;(X2-X1)/(Z2-Z1)*(Z2-256)	;11
	fxch 	st(3)			;Get V2				;11

	fsub 	ToV			;V2-V1				;12
	fxch 	st(2)			;Get U2-U1			;12


	fmul 	st,st(5)		;(U2-U1)/(Z2-Z1)		;13
	fxch 	st(3)			;Get (X2-X1)/(Z2-Z1)*(Z2-256)   ;13


	fadd 	[From].V_XRotated 	;(X2-X1)/(Z2-Z1)*(Z2-256)+X2	;14
	fxch 	st(3)			;Get (U2-U1)/(Z2-Z1)		;14

	fmul 	st,st(1)		;(U2-U1)/(Z2-Z1)*(Z2-256)	;16
	fxch 	st(4)			;Get Y2-Y1			;16

	fmul 	st,st(5)		;(Y2-Y1)/(Z2-Z1)		;18 - STALL
	fxch 	st(4)			;Get (U2-U1)/(Z2-Z1)*(Z2-256)	;18

	fadd 	FromU			;(U2-U1)/(Z2-Z1)*(Z2-256)+U2	;19
	fxch 	st(2)			;Get V2-V1			;19

	fmul 	st,st(5)		;(V2-V1)/(Z2-Z1)		;20
	fxch 	st(4)			;Get (Y2-Y1)/(Z2-Z1)		;20

	fmul 	st,st(1)		;(Y2-Y1)/(Z2-Z1)*(Z2-256)	;22 - STALL
	fxch 	st(4)			;Get (V2-V1)/(Z2-Z1)		;22

	fmulp 	st(1)			;(V2-V1)/(Z2-Z1)*(Z2-256)	;24 - STALL
	fxch 	st(3)			;Get (Y2-Y1)/(Z2-Z1)*(Z2-256)	;24

	fadd 	[From].V_YRotated 	;(Y2-Y1)/(Z2-Z1)*(Z2-256)+Y2	;25
	;fsub 	[From].V_YRotated

	fxch 	st(3)			;Get (V2-V1)/(Z2-Z1)*(Z2-256)	;25


	fadd 	FromV			;(V2-V1)/(Z2-Z1)*(Z2-256)+V2	;27 - STALL
	fxch 	st(2)			;Get X!				;27



	fstp 	[Dest].V_XRotated 	;STore new X1			;28

	fstp 	NewU 			;STore new U1			;30
	mov 	edx, SYS_ZClip
	fstp 	NewV 			;STore new V1			;32

	fstp 	[Dest].V_YRotated 	;STore new Y1			;34
	mov 	[Dest].V_ZRotated, edx	;STore new Z1

	ffree 	st			;remove 1/(Z2-Z1)		;36
	pop 	edx
									;Total
									;36+43 = 79

endm Clip_Line



Vertex0 equ ebp
Vertex1 equ ecx
Vertex2 equ esi



NewU	dd ?
NewV	dd ?
FromU	dd ?
FromV	dd ?
ToU	dd ?
ToV	dd ?

ZClip02	Macro
	;Save	ecx, esi, edi

	;edi	= offset To faces

	push 	ecx		;Face counter
	push    esi
	push	edi


	mov Vertex0,[edi].F_V0Offs 	;Get first vertex
	mov Vertex1,[edi].F_V1Offs	;Get second vertex
	mov Vertex2,[edi].F_V2Offs  	;Get third vertex


;Now check if vertices need clipping
;6 possibilities Total


	cmp	[Vertex0].V_ZFlag, 1	;Check if vertex0 is below clip-line
	jne 	DontClipVertex0		;No, dont clip vertex0

;Vertex0 is below, we now have 3 possibilities

ClipVertex0:
	cmp	[Vertex1].V_ZFlag, 1	;Check if vertex1 is below clip-line
	je 	ClipVertex1_And_0       ;Yes, clip vertex1 and vertex0

	cmp	[Vertex2].V_ZFlag, 1	;Check if vertex2 is below clip-line
	je 	ClipVerte2_And_0       	;Yes, clip vertex2 and vertex0

;-------------------------------------------------------------------
ClipOnly_Vertex0:

;This causes the polygon To splitt

ContClippVertex0:
;now clip vertex0

	mov	ebx, offset NewFace0	;Offset To new face 0
	mov	edx, offset NewFace1	;Offset To new face 1

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	ToU, eax
	mov	ToV, edi

	mov 	eax, offset NewVertex0_0;Find clip Dest 0 for face 0

	mov     edi, 0
	Clip_Line Vertex1, Vertex0, eax


	Project_Vertex eax

	mov	[ebx].F_V0Offs, eax	;Save vertex 0 in new face 0

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U0, edi		;Save new mapping coord U
	mov	[ebx].F_V0, eax		;Save new mapping coord V

	;-------------------------
	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [edx].F_U0
	mov	edi, [edx].F_V0
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, offset NewVertex1_0	;Find clip Dest 0 for face 1

	mov     edi, 1
	Clip_Line Vertex2, Vertex0, eax
	Project_Vertex eax

	mov	[ebx].F_V2Offs, eax	;Save as vertex 2 in face 0
	mov	[edx].F_V0Offs, eax	;Save as vertex 0 in face 1

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U2, edi
	mov	[edx].F_U0, edi
	mov	[ebx].F_V2, eax
	mov	[edx].F_V0, eax

	jmp DoneFace

;-------------------------------------------------------------------
ClipVerte2_And_0:

ContClip2_And_0:

	mov	ebx, Offset NewFace0

	;clip vertex 0

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_0

	mov     edi, 2
	Clip_Line Vertex1, Vertex0, eax	;Clip vertex0
	Project_Vertex eax

	mov 	[ebx].F_V0Offs, eax	;STore clipped vertex0

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U0, edi
	mov	[ebx].F_V0, eax

	;Clip vertex 2

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_2

	mov     edi, 3
	Clip_Line Vertex1, Vertex2, eax	;Clip vertex2
	Project_Vertex eax

	mov 	[ebx].F_V2Offs, eax

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U2, edi
	mov	[ebx].F_V2, eax

	jmp DoneFace


;-------------------------------------------------------------------
ClipVertex1_And_0:

ContClip1_And_0:

	mov	ebx, Offset NewFace0

	;clip vertex 0

	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_0

	mov     edi, 4
	Clip_Line Vertex2, Vertex0, eax	;Clip vertex0
	Project_Vertex eax

	mov 	[ebx].F_V0Offs, eax	;STore clipped vertex0

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U0, edi
	mov	[ebx].F_V0, eax

	;Clip vertex 1

	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_1

	;cmp	[Vertex0].V_ZFlag, 1
	;cmp	[Vertex1].V_ZFlag, 1
	;cmp	[Vertex2].V_ZFlag, 1
	;cmp     NumZClip, 3

	mov     edi, 5
	Clip_Line Vertex2, Vertex1, eax	;Clip vertex1
	Project_Vertex eax

	mov 	[ebx].F_V1Offs, eax

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U1, edi
	mov	[ebx].F_V1, eax

	jmp DoneFace
;-------------------------------------------------------------------

DontClipVertex0:
;Vertex0 is above clip-line, we now have 3 possibilities

	cmp	[Vertex1].V_ZFlag, 1	;Check if vertex1 is below clip-line
	je 	ClipVertex1		;Yes, vertex1 needs clipping

;else
DontClipVertex1:
;-------------------------------------------------------------------
ClipOnly_Vertex2:

;This causes the polygon To splitt

;now clip vertex2

	mov	ebx, offset NewFace0	;Offset To new face 0
	mov	edx, offset NewFace1	;Offset To new face 1

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	ToU, eax
	mov	ToV, edi

	mov 	eax, offset NewVertex0_2	;Find clip Dest 2 for face 0

	mov     edi, 6
	Clip_Line Vertex1, Vertex2, eax

	Project_Vertex eax

	mov	[ebx].F_V2Offs, eax	;Save vertex 2 in new face 0

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U2, edi		;Save new mapping coord U
	mov	[ebx].F_V2, eax		;Save new mapping coord V

	;-------------------------
	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [edx].F_U2
	mov	edi, [edx].F_V2
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, offset NewVertex1_2	;Find clip Dest 2 for face 1

	mov     edi, 7
	Clip_Line Vertex0, Vertex2, eax
	Project_Vertex eax

	mov	[ebx].F_V0Offs, eax	;Save as vertex 0 in face 0
	mov	[edx].F_V2Offs, eax	;Save as vertex 2 in face 1

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U0, edi
	mov	[edx].F_U2, edi
	mov	[ebx].F_V0, eax
	mov	[edx].F_V2, eax

	jmp DoneFace

;-------------------------------------------------------------------
ClipVertex1:
;Now we have 2 possibilities

	cmp	[Vertex2].V_ZFlag, 1	;Check if vertex2 is below clip-line
	jne 	ClipVertex1_Only		;Nope, clip only vertex2

;else
;-------------------------------------------------------------------
ClipVertex1_And_2:

	mov	ebx, Offset NewFace0

	;clip vertex 2

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_2

	mov     edi, 8
	Clip_Line Vertex0, Vertex2, eax	;Clip vertex2
	Project_Vertex eax

	mov 	[ebx].F_V2Offs, eax	;STore clipped vertex2

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U2, edi
	mov	[ebx].F_V2, eax

	;Clip vertex 1

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, Offset NewVertex0_1

	mov     edi, 9
	Clip_Line Vertex0, Vertex1, eax	;Clip vertex1
	Project_Vertex eax

	mov 	[ebx].F_V1Offs, eax

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U1, edi
	mov	[ebx].F_V1, eax

	jmp DoneFace


;-------------------------------------------------------------------
ClipVertex1_Only:

;This causes the polygon To splitt

;now clip vertex1

	mov	ebx, offset NewFace0	;Offset To new face 0
	mov	edx, offset NewFace1	;Offset To new face 1

	mov	eax, [ebx].F_U0
	mov	edi, [ebx].F_V0
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [ebx].F_U1
	mov	edi, [ebx].F_V1
	mov	ToU, eax
	mov	ToV, edi

	mov 	eax, offset NewVertex0_1	;Find clip Dest 1 for face 0

	mov     edi, 10
	Clip_Line Vertex0, Vertex1, eax

	Project_Vertex eax

	mov	[ebx].F_V1Offs, eax	;Save vertex 1 in new face 0

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U1, edi		;Save new mapping coord U
	mov	[ebx].F_V1, eax		;Save new mapping coord V

	;-------------------------
	mov	eax, [ebx].F_U2
	mov	edi, [ebx].F_V2
	mov	FromU, eax
	mov	FromV, edi

	mov	eax, [edx].F_U1
	mov	edi, [edx].F_V1
	mov	ToU, eax
	mov	ToV, edi

	mov	eax, offset NewVertex1_1	;Find clip Dest 1 for face 1

	mov     edi, 11
	Clip_Line Vertex2, Vertex1, eax
	Project_Vertex eax

	mov	[ebx].F_V2Offs, eax	;Save as vertex 2 in face 0
	mov	[edx].F_V1Offs, eax	;Save as vertex 1 in face 1

	mov	edi, NewU
	mov	eax, NewV
	mov	[ebx].F_U2, edi
	mov	[edx].F_U1, edi
	mov	[ebx].F_V2, eax
	mov	[edx].F_V1, eax

	jmp DoneFace



;-------------------------------------------------------------------
;DontClipAtAll:

DoneFace:

	pop	edi
	pop	esi
	pop	ecx


	endm









;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T02_Call
;쿔D           : T02
;쿟ype         : Procedure
;쿗ast update  : 10.02.1998
;쿌ction       : Runs z-clipping, then draws
;쿚ptimized    : No
;�
;쿔nput variables : [esi] = polygon
;�
;쿙otes : FPU stack must be empty.
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

NewFace0        Face ?
NewFace1        Face ?
NewVertex0_0	Vertice ?
NewVertex0_1	Vertice ?
NewVertex0_2	Vertice ?

NewVertex1_0	Vertice ?
NewVertex1_1	Vertice ?
NewVertex1_2	Vertice ?


;esi = polygon
T02_Call      PROC
		mov     eax, [esi].F_Material
		mov	ebx, [eax].M_MapOffset
		mov	T02_MapOffset, ebx
		mov	ebx, [eax].M_MapWidth
		mov	T02_MapWidth, ebx


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

                ZClip02

                mov    	esi, OFFSET NewFace1
                cmp    	[esi].F_ZFlag, 2         ;Draw To/one polygon?
                je     	DrawOne                  ;Draw one!
                call   	T02_Tri

DrawOne:
                mov    	esi, OFFSET NewFace0
NoZClip:
                call   	T02_Tri

                ret
T02_Call      ENDP






;----------------------------------------------------------------------------














;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : T02_Scanline
;쿔D           : T02
;쿟ype         : Macro
;쿗ast update  : 12.12.1998
;쿌ction       : Draws a perspectivemapped, horisontal scanline, 32k, z-buffer
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

T02_ScanLine  MACRO
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


;컴컴 Clip U/Z, V/Z and 1/Z left To next pixel...
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
                fmul   SYS_ZBufFP    ;1/ZL 1/ZL U/ZL V/ZL
                fistp  LeftNdZ       ;1/ZL U/ZL V/ZL

;컴컴 Get Z left...
                fld1                 ;1    1/ZL U/ZL V/ZL
                fdiv   st, st(1)     ;ZL   1/ZL U/ZL V/ZL




;컴컴 !!!INTEGER INSTRUCTIONS HERE!!!
                pushad                          ;Save all registers

		mov    eax, SYS_ZBufAdd
		add    eax, LeftNdZ

                mov    edi, PixelPtr            ;Use edi as mempointer
                add    edi, EdgeLeft.X

                mov    eax, EdgeLeft.X          ;Calculate scanline length
                mov    ebx, EdgeRight.X         ;/
                sub    ebx, eax                 ;ebx = scanline length
                mov    ScanLength, ebx

                cmp    ebx, 0
                jl     Scan_Done

                cmp    ebx, 16                  ;Run lefToverloop now?
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

                imul   ecx, T02_MapWidth
                add    ecx, eax
                mov    Slope, ecx               ;Save normal slope
                add    ecx, T02_MapWidth
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
                imul   esi, T02_MapWidth

                add    esi, eax                 ;Add U-offset

                mov    edx, T02_MapOffset
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


;컴컴 Pixel 0

                cmp    edx, [edi*4 + 12345678h]
Offs00_A:       jbe    Dump0

                mov    [edi*4 + 12345678h], edx
Offs00_B:
                mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs00_C:
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
Offs01_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs01_C:
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
Offs02_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs02_C:
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
Offs03_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs03_C:
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
Offs04_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs04_C:

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
Offs05_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs05_C:

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
Offs06_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs06_C:

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
Offs07_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs07_C:

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
Offs08_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs08_C:

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
Offs09_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs09_C:

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
Offs10_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs10_C:

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
Offs11_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs11_C:

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
Offs12_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs12_C:

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
Offs13_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs13_C:

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
Offs14_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs14_C:

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
Offs15_B:       mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
Offs15_C:

Dump15:
                add    edx, NZSlopeInt

                mov    LeftNdZ, edx

                add    edi, 16                  ;Move memory pointer
                sub    ScanLength, 16           ;Decrease counter
                cmp    ScanLength, 16           ;Finished?
                jbe    LeftOver2
                jmp    Loop16



;컴컴 Write lefTover pixels...

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

                imul   ecx, T02_MapWidth
                add    ecx, eax
                mov    Slope, ecx              ;Save normal slope
                add    ecx, T02_MapWidth
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
                imul   esi, T02_MapWidth

                add    esi, eax                 ;Add U-offset

                mov    edx, T02_MapOffset
                shr    edx, 1
                add    esi, edx                 ;Add textureoffset

                                                ;esi = mempointer
                                                ;ebx = U fractional part
                                                ;ecx = V fractional part




                mov    edx, LeftNdZ
                mov    ebp, ScanLength
                inc    ebp

LeftOverLoop:
                cmp    edx, [edi*4 + 12345678h]
OffsXX_A:       jbe    DumpXX
                mov    [edi*4 + 12345678h], edx
OffsXX_B:

                mov    ax, [esi*2]
                mov    [edi*2 + 12345678h], ax
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
;쿙ame         : T02_Tri
;쿔D           : T02
;쿟ype         : Procedure
;쿗ast update  : 26.07.1997
;쿌ction       : Draws a perspectivemapped triangel, 32k colours, z-buffer
;쿚ptimized    : No
;�
;쿔nput variables : [esi]           = Offset Face STRUCT
;�                  T02_MapOffset   = Offset texturemap
;�                  T02_MapWidth    = Mapwidth in pixels (!!! INTEGER !!!)
;�                  T02_MapModifier = Mappingcoordinate-modifer (default=1.0)
;�
;쿙otes: FPU stack must be empty. General registers are Destroyed.
;쿔MPORTANT: T02_MapOffset and T02_MapWidth MUST BE SET!!!
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


T02_Tri       PROC

;컴컴 Load and locally sTore mappingcoordinates...
		mov     eax, [esi].F_U0
		mov     ebx, [esi].F_V0
		mov	_U0, eax
		mov	_V0, ebx
		mov     eax, [esi].F_U1
		mov     ebx, [esi].F_V1
		mov	_U1, eax
		mov	_V1, ebx
		mov     eax, [esi].F_U2
		mov     ebx, [esi].F_V2
		mov	_U2, eax
		mov	_V2, ebx

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

;컴컴 Get XSlope A and B...
                fld    [ecx].V_X2D    ;X1   YNDB YNDA
                fsub   [ebx].V_X2D    ;XDA  YNDB YNDA
                fmul   st, st(2)      ;XSA  YNDB YNDA
                fstp   XSlopeA        ;YNDB YNDA

                fld    [edx].V_X2D    ;X2   YNDB YNDA
                fsub   [ebx].V_X2D    ;XDB  YNDB YNDA
                fmul   st, st(1)      ;XSB  YNDB YNDA
                fstp   XSlopeB        ;YNDB YNDA

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

                ffree  st
                ffree  st(1)


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

;컴컴 Clipping edge A To next scanline...
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



;컴컴 Clipping edge B To next scanline...
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

                T02_ScanLine


;컴컴 Update edge A...
                fld    EdgeA.X
                fadd   XSlopeA
                fstp   EdgeA.X

                fld    EdgeA.UdZ
                fadd   UZSlopeA
                fstp   EdgeA.UdZ

                fld    EdgeA.VdZ
                fadd   VZSlopeA
                fstp   EdgeA.VdZ

                fld    EdgeA.NdZ
                fadd   NZSlopeA
                fstp   EdgeA.NdZ



;컴컴 Update edge B...
                fld    EdgeB.X
                fadd   XSlopeB
                fstp   EdgeB.X

                fld    EdgeB.UdZ
                fadd   UZSlopeB
                fstp   EdgeB.UdZ

                fld    EdgeB.VdZ
                fadd   VZSlopeB
                fstp   EdgeB.VdZ

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


;컴컴 Clipping edge C To next scanline and setting initial values...

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
T02_Tri       ENDP



T02_Init      PROC
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
T02_Init      ENDP

END
