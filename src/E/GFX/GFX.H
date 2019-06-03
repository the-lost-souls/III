/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : GFX.H
�Author       : Cyberfish/TLS & Toe/DU
�Last update  : 22.12.97
�Action       : Definitions for all gfx-loaders.
�
�Notes :
����������������������������������������������������������������������������*/

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
