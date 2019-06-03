/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : MAIN.H
쿌uthor       : Cyberfish/TLS & Toe/DU
쿗ast update  : 11.09.98
쿌ction       : External definitions for main.asm
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef MAIN_included
#define MAIN_included

#include "w:\general\var.h"

extern "C" 	void ASM_CALL  	MN_MainLoop(void); 	//Main-looproutine
extern "C" 	void ASM_CALL 	KEY_RPressed(void);	//Keypressed routine
extern "C"	void ASM_CALL	ZERO_Object(void);	//Zeroobject
extern "C" 	int  		KEY_OPressed;       	//Keypressed object
extern "C" 	void ASM_CALL 	BRK_RFrames(void); 	//Keypressed routine
extern "C" 	int  		BRK_OFrames;	    	//Keypressed object

#endif
