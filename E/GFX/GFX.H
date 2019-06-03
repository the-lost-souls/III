/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : GFX.H
쿌uthor       : Cyberfish/TLS & Toe/DU
쿗ast update  : 22.12.97
쿌ction       : Definitions for all gfx-loaders.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef GFX_included
#define GFX_included

#include "w:\gfx\tga.h"
#include "w:\gfx\jpg.h"

class RAW_FILE_Header
{
public:
	char PAD1[8];
	unsigned char XResHi;
	unsigned char XResLo;
	unsigned char YResHi;
	unsigned char YResLo;
	char Pad2[20];
};


#endif

