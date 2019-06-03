/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : CANVAS.H
�Author       : Cyberfish/TLS
�Last update  : 12.12.1998
�Action       : Definitions for canvas updating
�
�Notes :
����������������������������������������������������������������������������*/

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

