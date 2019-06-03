/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : TGA.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 15.08.98
쿌ction       : Definition of TGA_Load-routine + public variables.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef TGA_included
#define TGA_included

struct TGA_Header			//Header of .TGA-file
{
	unsigned char IDFieldLength;
	unsigned char ColorMapType;
	unsigned char ImageType;

	unsigned char CMapOrigLo;
	unsigned char CMapOrigHi;
	unsigned char CMapLengthLo;
	unsigned char CMapLengthHi;
	unsigned char CMapSize;
	unsigned char XOrigLo;
	unsigned char XOrigHi;
	unsigned char YOrigLo;
	unsigned char YOrigHi;
	unsigned char WidthLo;
	unsigned char WidthHi;
	unsigned char HeigthLo;
	unsigned char HeigthHi;
	unsigned char Bits;

	unsigned char IDescriptor;
};

void *TGA_Load(const char *FileName);

extern  "C" int		TGA_Width;              //Image width
extern  "C" int 	TGA_Heigth;             //Image heigth
extern  "C" int 	TGA_XPos;               //Image horisontal position
extern  "C" int 	TGA_YPos;               //Image vertical position
extern  "C" int 	TGA_Bits;               //Bits per pixel

#endif

