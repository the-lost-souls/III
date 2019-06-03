;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : VAR.ASM
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  : 12.12.98
;쿌ction       :
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
.386
.Model flat, C
.code

public	      SYS_Ambient

public        SYS_QuitMsg                       ;Quitmessage
public        SYS_TempSize                      ;Tempbuffersize
public        SYS_OListOffs                     ;ObjectListOffset
public        SYS_LListOffs                     ;LightListOffset
public        SYS_OOffs                         ;ObjectOffset
public        SYS_POffs                         ;Plugin data-offset
public        SYS_PCaller                       ;PluginCaller
public        SYS_LOffs                         ;LightOffset
public        SYS_QuitFlag                      ;QuitFlag
public        SYS_ProgBase                      ;Program base-address
public        SYS_buf_es                        ;8kb buffer segment-address
public        SYS_buf_flat                      ;8kb buffer linear address
public        SYS_TempBuf                       ;Offset tempbuffer

public        SYS_DBufOffs                      ;Offset doublebuffer
public        SYS_DBufSize                      ;Size doublebuffer
public        SYS_DBufLine                      ;Bytes per line, doublebuffer
public        SYS_ZBufOffs                      ;Offset Z-buffer
public        SYS_ZBufSize                      ;Size Z-buffer
public        SYS_ZBufLine                      ;Bytes per line, Z-buffer
public	      SYS_ZBufFP
public	      SYS_ZBufAdd
public	      SYS_VideoMem			;Offset videomem
public        SYS_XCenter                       ;
public	      SYS_YCenter                       ;
public        SYS_XPerspective                  ;Used for 3D->2D projection
public        SYS_YPerspective                  ;/
public        SYS_FPClipRight
public        SYS_FPClipLeft
public        SYS_FPClipTop
public        SYS_FPClipBottom

public	      SYS_XRes
public	      SYS_YRes

public        FPU_Original                      ;Original controlword
public        FPU_24                            ;24 precision?
public        FPU_Round                         ;Controlword for round
public        FPU_Chop                          ;Controlword for chop
public        FPU_RoundUp                       ;Controlword for round up
public        FPU_RoundDown                     ;Controlword for round down

public        SYS_ZClip                         ;Clip polygons here

public        SRT_Start1
public        SRT_Start2
public        SRT_End1
public        SRT_End2

public	      SYS_TimerTick
public	      SYS_NrFrames


ALIGN 4

SYS_Ambient	DD 1.0
                                                ;Main object's varibles
SYS_QuitMsg     DD OFFSET SYS_Unknown
SYS_TempSize    DD 128*1024                     ;Tempbuffer size (default)
SYS_OListOffs   DD 0                            ;Object list
SYS_LListOffs   DD 0                            ;Light list
SYS_OOffs       DD 0                            ;Offset to current object
SYS_POffs       DD 0                            ;Offset to current plugin
SYS_PCaller     DD 0                            ;Offset to current plugincaller
SYS_LOffs       DD 0                            ;Offset to current light
SYS_QuitFlag    DD 0
SYS_ProgBase    DD 0                            ;Program baseaddress
SYS_buf_es      DW 0                            ;8kb, segment address
                DW 0                            ;4 byte aligning...
SYS_buf_flat    DD 0                            ;8kb, linear address
SYS_TempBuf     DD 0                            ;Offset to tempbuffer

SYS_DBufOffs    DD 0                            ;Offset doublebuffer
SYS_DBufSize    DD 0                            ;Size of doublebuffer
SYS_DBufLine    DD 0                            ;Bytes per line, doublebuffer
SYS_ZBufOffs    DD 0                            ;Offset Z-buffer
SYS_ZBufSize    DD 0                            ;Size of Z-buffer (bytes)
SYS_ZBufLine    DD 0                            ;Bytes per line, Z-buffer
SYS_ZBufFP	DD 16777215.0
SYS_ZBufAdd	DD 000000000h
SYS_VideoMem	DD 0
SYS_XCenter     DD 160.0                        ;
SYS_YCenter     DD 100.0                        ;
SYS_XPerspective DD 3840.0                       ;Used for 3D->2D projection
SYS_YPerspective DD -320.0                       ;/
SYS_FPClipRight DD 319.0
SYS_FPClipLeft  DD 0.0
SYS_FPClipTop   DD 0.0
SYS_FPClipBottom DD 199.0

SYS_XRes	DD 320
SYS_YRes	DD 200

FPU_Original    DW 0                            ;Original controlword
FPU_24          DW 37Fh                         ;????
FPU_Round       DW 0                            ;Controlword for round
FPU_Chop        DW 110000000000b                ;Controlword for chop
FPU_RoundUp     DW 100000000000b                ;Controlword for round up
FPU_RoundDown   DW 010000000000b                ;Controlword for round down

SYS_ZClip       DD 1.0


SRT_Start1      DD OFFSET SRT_Buffer
SRT_Start2      DD OFFSET SRT_Buffer + 1024
SRT_End1        DD OFFSET SRT_Buffer + 2048
SRT_End2        DD OFFSET SRT_Buffer + 3072

SRT_Buffer      DD 4*256 DUP (?)                ;4k

SYS_TimerTick   DD 0
SYS_NrFrames	DD 0

SYS_Unknown     DB 'SYS: No quitmessage.', 0Dh, 0Ah, 0, '$'

END
