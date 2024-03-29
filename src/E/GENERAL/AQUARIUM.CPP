/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : AQUARIUM.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 23.05.1998
쿌ction       : Some small routines for the engine.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
#include "w:\general\var.h"
#include <stdio.h>
#include <string.h>

char *SYS_QuitMsg_ 		= NULL;		//Pointer to quitmessage
const int SYS_QuitMsgSize 	= 4096;		//Size of errorbuffer


/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : SYS_Error
쿔D           : SYS
쿗ast update  : 23.05.1998
쿚ptimized    : -
�
쿔n:	Param1: Number of errorstrings
�	Param...: Param1 number of strings
�
쿙otes: If the errorbuffer is full, messages will be discarded.
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
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
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : Aquarium
쿔D           :
쿗ast update  : 23.10.1998
쿚ptimized    : -
�
�
쿙otes: Sets up some stuff for the engine.
�		-clears screen
�		-writes engine-startup-string
�		-sets the fpu-flag-variables
�		-allocates memory for the errorbuffer.
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
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
