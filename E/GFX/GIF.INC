;旼컴컴컴컴컴컴컴컴컴컴컴컴컴� ASSEMBLER INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴
;쿙ame         : GIF.INC
;쿌uthor       : Cyberfish of The Lost Souls
;쿗ast update  : 05.08.1997
;쿌ction       : Decodes a GIF87/89-picture
;�
;쿙otes : Look GIF.DOC
;읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extrn  GIF_Load       :PROC
extrn  GIF_SWidth     :DWORD                ;Screen width
extrn  GIF_SHeigth    :DWORD                ;Screen heigth
extrn  GIF_Width      :DWORD                ;Image width
extrn  GIF_Heigth     :DWORD                ;Image heigth
extrn  GIF_XPos       :DWORD                ;Image horisontal position
extrn  GIF_YPos       :DWORD                ;Image vertical position
extrn  GIF_Colors     :DWORD                ;Number of colors
extrn  GIF_Palette    :BYTE                 ;Palette

