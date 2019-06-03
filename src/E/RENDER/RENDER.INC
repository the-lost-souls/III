;旼컴컴컴컴컴컴컴컴컴컴컴컴 ASSEMBLER INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴�
;쿙ame         : RENDER.INC
;쿌uthor       : Cyberfish/TLS & Toe/DU
;쿗ast update  :
;쿌ction       :
;�
;쿙otes :
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extrn  T02_Tri              :PROC        ;Perspective, no shading, Z-buffer
extrn  T02_Init             :PROC        ;Set address displacement constants
extrn  T02_Call             :PROC        ;

extrn  T04_Tri              :PROC        ;Flat-filled, z-buffer
extrn  T04_Init             :PROC
extrn  T04_Call             :PROC        ;
extrn  T04_FlatColor	    :WORD

extrn  T05_Tri              :PROC        ;Flat-shaded, tmapped, z-buffer, persp
extrn  T05_Init             :PROC        ;Set address displacement constants
extrn  T05_Call             :PROC        ;

extrn  T06_Tri              :PROC        ;Flat, transparent, z-buffer
extrn  T06_Init             :PROC        ;Set address displacement constants
extrn  T06_Call             :PROC        ;

extrn  T07_Tri              :PROC        ;Linear tmap, z-buffer
extrn  T07_Init             :PROC        ;Set address displacement constants
extrn  T07_Call             :PROC        ;

extrn  T08_Tri              :PROC        ;Linear tmap, flatshaded, z-buffer
extrn  T08_Init             :PROC        ;Set address displacement constants
extrn  T08_Call             :PROC        ;

extrn  T09_Tri              :PROC        ;
extrn  T09_Init             :PROC        ;
extrn  T09_Call             :PROC        ;
extrn  T09_DBufFlag	    :DWORD       ;Flag: Copy canvas!



