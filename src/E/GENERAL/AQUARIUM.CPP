/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : AQUARIUM.CPP
³Author       : Cyberfish/TLS
³Last update  : 23.05.1998
³Action       : Some small routines for the engine.
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include "w:\general\var.h"
#include <stdio.h>
#include <string.h>

char *SYS_QuitMsg_ 		= NULL;		//Pointer to quitmessage
const int SYS_QuitMsgSize 	= 4096;		//Size of errorbuffer


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SYS_Error
³ID           : SYS
³Last update  : 23.05.1998
³Optimized    : -
³
³In:	Param1: Number of errorstrings
³	Param...: Param1 number of strings
³
³Notes: If the errorbuffer is full, messages will be discarded.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : Aquarium
³ID           :
³Last update  : 23.10.1998
³Optimized    : -
³
³
³Notes: Sets up some stuff for the engine.
³		-clears screen
³		-writes engine-startup-string
³		-sets the fpu-flag-variables
³		-allocates memory for the errorbuffer.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
