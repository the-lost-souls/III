/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : RENDER.H
�Author       : Cyberfish/TLS & Toe/DU
�Last update  : 27.12.98
�Action       : Definitions for all rendering routines
�
�Notes :
����������������������������������������������������������������������������*/

#ifndef REN_included
#define REN_included

extern "C" 	void ASM_CALL T02_Call(void);
extern "C" 	void ASM_CALL T02_Init(void);

extern "C" 	void ASM_CALL T03_Call(void);
extern "C" 	void ASM_CALL T03_Init(void);

extern "C" 	void ASM_CALL T04_Call(void);
extern "C" 	void ASM_CALL T04_Init(void);

extern "C" 	void ASM_CALL T05_Call(void);
extern "C" 	void ASM_CALL T05_Init(void);

extern "C" 	void ASM_CALL T06_Call(void);
extern "C" 	void ASM_CALL T06_Init(void);

extern "C" 	void ASM_CALL T07_Call(void);
extern "C" 	void ASM_CALL T07_Init(void);

extern "C" 	void ASM_CALL T08_Call(void);
extern "C" 	void ASM_CALL T08_Init(void);

extern "C" 	void ASM_CALL T09_Call(void);
extern "C" 	void ASM_CALL T09_Init(void);
extern "C"	void *T09_DBuf;
extern "C"	int T09_SetupFlag;

extern "C" 	void ASM_CALL T10_Call(void);
extern "C" 	void ASM_CALL T10_Init(void);
#endif



