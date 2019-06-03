/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : SORT.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 05.01.1999
쿌ction       : External definitions for sorting-routines.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef SRT_included
#define SRT_included

#include "w:\general\var.h"

extern "C" 	void ASM_CALL	SRT_RReset(void);		//Routine
extern "C" 	void ASM_CALL 	SRT_RSort(void);		//Routine
extern "C" 	void ASM_CALL  	SRT_RRun(void);		//Routine

extern "C" 	int 		SRT_OReset;     		//Object
extern "C" 	int 		SRT_OSort;      		//Object
extern "C" 	int 		SRT_ORun;       		//Object

extern "C"	int    	       *SRT_Start1;
extern "C"	int    	       *SRT_Start2;
extern "C"	int    	       *SRT_End1;
extern "C"	int    	       *SRT_End2;



#endif
