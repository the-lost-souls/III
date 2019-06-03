;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : VAR.INC
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 12.12.1998
;쿌ction       :
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extrn	      SYS_Ambient      :DWORD
extrn         SYS_QuitMsg      :DWORD           ;Quitmessage
extrn         SYS_OListOffs    :DWORD           ;ObjectListOffset
extrn         SYS_LListOffs    :DWORD           ;LightListOffset
extrn         SYS_OOffs        :DWORD           ;ObjectOffset
extrn         SYS_POffs        :DWORD           ;Plugin data-offset
extrn         SYS_PCaller      :DWORD           ;PluginCaller
extrn         SYS_LOffs        :DWORD           ;LightOffset
extrn         SYS_QuitFlag     :DWORD           ;QuitFlag
extrn         SYS_ProgBase     :DWORD           ;Program base-address
extrn         SYS_buf_es       :WORD            ;8kb buffer segment-address
extrn         SYS_buf_flat     :DWORD           ;8kb buffer linear address
extrn         SYS_TempBuf      :DWORD           ;Offset tempbuffer

extrn         SYS_DBufOffs     :DWORD           ;Offset doublebuffer
extrn         SYS_DBufSize     :DWORD           ;Size doublebuffer
extrn         SYS_DBufLine     :DWORD           ;Bytes per line, doublebuffer
extrn         SYS_ZBufOffs     :DWORD           ;Offset Z-buffer
extrn         SYS_ZBufSize     :DWORD           ;Size Z-buffer
extrn         SYS_ZBufLine     :DWORD           ;Bytes per line, Z-buffer
extrn         SYS_ZBufFP       :DWORD           ;ZBuffer, fixed point factor
extrn         SYS_ZBufAdd      :DWORD           ;ZBuffer, optimization trick
extrn	      SYS_VideoMem     :DWORD
extrn         SYS_XCenter      :DWORD           ;
extrn 	      SYS_YCenter      :DWORD           ;
extrn         SYS_XPerspective :DWORD           ;Used for 3D->2D projection
extrn         SYS_YPerspective :DWORD           ;/
extrn         SYS_FPClipRight  :DWORD
extrn         SYS_FPClipLeft   :DWORD
extrn         SYS_FPClipTop    :DWORD
extrn         SYS_FPClipBottom :DWORD
extrn         SYS_ZClip        :DWORD

extrn	      SYS_XRes	       :DWORD
extrn         SYS_YRes         :DWORD

;extrn         SYS_Materials    :DWORD           ;Offset materiallist


extrn         FPU_Original     :WORD            ;The original controlword
extrn         FPU_24           :WORD            ;???
extrn         FPU_Round        :WORD            ;Controlword for round
extrn         FPU_Chop         :WORD            ;Controlword for chop
extrn         FPU_RoundUp      :WORD            ;Controlword for round up
extrn         FPU_RoundDown    :WORD            ;Controlword for round down

extrn         SYS_ORoutines    :DWORD           ;The list of objectroutines
extrn         SYS_PRoutines    :DWORD           ;The list of part-routines
extrn         SYS_LRoutines    :DWORD           ;The list of light-routines

extrn         SRT_Start1
extrn         SRT_Start2
extrn         SRT_End1
extrn         SRT_End2


extrn	      SYS_TimerTick
extrn	      SYS_NrFrames

;Constants for objects, "C" for Constant.. :)

C_ZeroObject   EQU 0001                         ;Zeroobject
C_KeyPressed   EQU 0002                         ;KeyPressed-check
C_VEC01        EQU 0003                         ;Perspective tmapping
                                                ;View canvas
C_VEC03        EQU 0005
C_SortReset    EQU 0006                         ;Reset part-tables
C_Sort         EQU 0007                         ;Sort part-tables
C_Run          EQU 0008                         ;Run all parts
C_InitLights   EQU 0009                         ;Run lights' plugins
C_VEC04        EQU 0010
C_BreakFrames  EQU 0011
C_VEC05	       EQU 0012
C_VEC06	       EQU 0013
C_VEC07	       EQU 0014

;Constants for parts...

C_T02          EQU 0001                         ;Tmapped, 15bit, perspective
C_T04          EQU 0002
C_T05          EQU 0003

;Datastructure for standard vectorobject header

Header        STRUC
H_Type          DD ?                            ;Objecttype
H_Plugins       DD 16 DUP (?)                   ;Plugins

H_M00		DD ?
H_M01		DD ?
H_M02		DD ?
H_M03		DD ?
H_M10		DD ?
H_M11		DD ?
H_M12		DD ?
H_M13		DD ?
H_M20		DD ?
H_M21		DD ?
H_M22		DD ?
H_M23		DD ?

H_Vertices      DD ?                            ;Number of vertices
H_VertOffs      DD ?                            ;Offset vertices
H_Faces         DD ?                            ;Number of faces
H_FaceOffs      DD ?                            ;Offset faces
H_Materials	DD ?				;Number of materials
H_MatrOffs	DD ?				;Offset materials
H_Memory        DD ?                            ;Total memory used for object

              ENDS



;Datastructure for standard face


F_Size          EQU 96
Face          STRUC

F_NextOffset    DD ?                            ;00
F_SortValue     DD ?                            ;04
F_Type          DD ?                            ;08
F_Priority	DD ?				;12
F_NVX           DD ?                            ;12
F_NVY           DD ?                            ;16
F_NVZ           DD ?                            ;20
F_V0Offs        DD ?                            ;24
F_V1Offs        DD ?                            ;28
F_V2Offs        DD ?                            ;32
F_Material      DD ?                            ;36
F_IntensityR    DD ?                            ;40
F_IntensityG    DD ?                            ;44
F_IntensityB    DD ?                            ;48
F_U0            DD ?                            ;52
F_V0            DD ?                            ;56
F_U1            DD ?                            ;60
F_V1            DD ?                            ;64
F_U2            DD ?                            ;68
F_V2            DD ?                            ;72
F_ZFlag         DD ?                            ;76

Face          ENDS





;Datastructure for standard vertice
V_Size        EQU 96
Vertice       STRUC

V_X             DD ?                            ;00
V_Y             DD ?                            ;04
V_Z             DD ?                            ;08
V_InvZ          DD ?                            ;12
V_X2D           DD ?                            ;16
V_Y2D           DD ?                            ;20
V_XRotated      DD ?                            ;24
V_YRotated      DD ?                            ;28
V_ZRotated      DD ?                            ;32
V_U             DD ?                            ;36
V_V             DD ?                            ;40
V_NVX           DD ?                            ;44
V_NVY           DD ?                            ;48
V_NVZ           DD ?                            ;52
V_IntensityR    DD ?                            ;56
V_IntensityG    DD ?                            ;60
V_IntensityB    DD ?                            ;64
V_ZFlag         DD ?                            ;68 Z-flag
V_RFlag         DD ?                            ;72 Rotation-flag
V_PFlag         DD ?                            ;76 Position-flag

V_NVXRotated	= V_IntensityR
V_NVYRotated	= V_IntensityG
V_NVZRotated	= V_IntensityB

Vertice       ENDS


M_Size        EQU 9*4
Material      STRUC

M_Type		DD ?
M_R             DD ?
M_G             DD ?
M_B             DD ?
M_Luminosity    DD ?
M_Diffuse       DD ?
M_Transparency  DD ?
M_MapWidth      DD ?
M_MapHeigth     DD ?
M_MapOffset     DD ?

Material      ENDS


Matrix44      STRUC

M44_00          DD ?
M44_01          DD ?
M44_02          DD ?
M44_03          DD ?
M44_10          DD ?
M44_11          DD ?
M44_12          DD ?
M44_13          DD ?
M44_20          DD ?
M44_21          DD ?
M44_22          DD ?
M44_23          DD ?

Matrix44      ENDS

Light	      STRUC

L_Type          DD ?
L_Plugins       DD 16 DUP (?)

L_Matrix        DD 12 DUP (?)                   ;Space for transformation
                                                ;/matrix

L_Pos           DD 3 DUP (?)                    ;Unified vector
L_R	        DD ?				;Color
L_G 	        DD ?
L_B  	        DD ?

L_Intensity	DD ?				;FP: (0-1) Intensity
L_MaxRange 	DD ?				;FP: Max. range of light
L_Falloff	DD ?				;FP:Falloff (intensity/Maxrange)

L_ConeAngle	DD ?				;FP
L_EdgeAngle	DD ?				;FP

	      ENDS


q EQU QWORD PTR
d EQU DWORD PTR
w EQU WORD PTR
b EQU BYTE PTR
