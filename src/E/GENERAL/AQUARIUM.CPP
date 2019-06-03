/*
�������������������������� WATCOM C++ SOURCE �������������������������������
�Name         : AQUARIUM.CPP
�Author       : Cyberfish/TLS
�Last update  : 23.05.1998
�Action       : Some small routines for the engine.
�
�Notes :
����������������������������������������������������������������������������*/
#include "w:\general\var.h"
#include <stdio.h>
#include <string.h>

char *SYS_QuitMsg_ 		= NULL;		//Pointer to quitmessage
const int SYS_QuitMsgSize 	= 4096;		//Size of errorbuffer


/*
�������������������������� WATCOM C++ ROUTINE ������������������������������
�Name         : SYS_Error
�ID           : SYS
�Last update  : 23.05.1998
�Optimized    : -
�
�In:	Param1: Number of errorstrings
�	Param...: Param1 number of strings
�
�Notes: If the errorbuffer is full, messages will be discarded.
����������������������������������������������������������������������������*/
void SYS_Error(int NrMsgs, const char *ErrorMsg, ... )
{
	const char **Messages = &ErrorMsg;
	for ( int C = 0; C < NrMsgs; C++ )
	{
		if ( ( strlen(SYS_QuitMsg_) + strlen(Messages[C]) ) >= 4096 ) return;
		strcpy((char*)&SYS_QuitMsg_[strlen(SYS_QuitMsg_)], Messages[C]);
	}//for
}//SYS_Error



/*
�������������������������� WATCOM C++ ROUTINE ������������������������������
�Name         : Aquarium
�ID           :
�Last update  : 23.10.1998
�Optimized    : -
�
�
�Notes: Sets up some stuff for the engine.
�		-clears screen
�		-writes engine-startup-string
�		-sets the fpu-flag-variables
�		-allocates memory for the errorbuffer.
����������������������������������������������������������������������������*/
int Aquarium(int StartupMsg)
{
	__asm{
		push	eax

	      	mov   	ah, 0                     //Init screen number
	       	mov   	al, 3h                   //
	       	int   	10h                       //Do it!

		pop	eax
	}//asm

	if ( StartupMsg == TRUE )
	{
		printf("Aquarium ---- Demoengine by Cyberfish/TLS 1999\n\n");
		printf("Featuring -IMS v0.6\n");
		printf("          -ZLib v1.1.2\n");
		printf("          -PTC v0.72\n");
		printf("	  -IJG jpeg-reader\n\n");
	}//if

//---- Setting FPU control-data...
	__asm{

		push	eax

                fstcw  FPU_Original             ;Setting controlword-values
                fldcw  FPU_24
                mov    ax, FPU_24
                or     FPU_Round, ax
                or     FPU_Chop, ax
                or     FPU_RoundUp, ax
                or     FPU_RoundDown, ax

		pop	eax

	}//asm

	SYS_QuitMsg_ = new char[SYS_QuitMsgSize];//4k for error-messages.
	if ( SYS_QuitMsg_ == NULL )
	{
		printf("Bubelibub. Couldn't fill aquarium\n");
		return 0;
	}//if

	memset(SYS_QuitMsg_, 0, SYS_QuitMsgSize);

	return 1;
}//Aquarium