/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : SORT.H
�Author       : Cyberfish/TLS
�Last update  : 05.01.1999
�Action       : External definitions for sorting-routines.
�
�Notes :
����������������������������������������������������������������������������*/

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