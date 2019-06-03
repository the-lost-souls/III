@echo off
tasm %1 /m3
tasm tga.asm /m3
dlink %1 ,,,W:\DEBUG.LIB W:\GENERAL\SYS.LIB W:\GFX\GFX.LIB
pause
%1
