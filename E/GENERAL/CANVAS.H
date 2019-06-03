/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : CANVAS.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 12.12.1998
쿌ction       : Definitions for canvas updating
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef CNV_included
#define CNV_included

#include "w:\general\var.h"

int CNV_Setup(void);
void CNV_Update(void);


const int CNV_FlagDefault 	= 0;
const int CNV_FlagOptimizeZBuf 	= 0x1;


class CNV_Data
{
public:
	CNV_Data(void); 		//Constructor

	int 	Type;			//Objecttype
	int 	Plugins[16];		//Plugins...

	int Flags;

private:
};

#endif


