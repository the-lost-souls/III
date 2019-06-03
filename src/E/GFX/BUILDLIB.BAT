@echo off
tasm /m3 gif.asm
tasm /m3 pcx.asm
tasm /m3 tga.asm

del gfx.lib
tlib gfx.lib +gif.obj +pcx.obj +tga.obj