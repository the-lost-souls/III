/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : LISTS.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 27.12.1998
쿌ction       : The definitions of the different routine-lists...
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include "w:\general\var.h"
#include "w:\vectors\vectors.h"
#include "w:\general\main.h"
#include "w:\general\canvas.h"
#include "w:\vectors\lights.h"
#include "w:\render\render.h"
#include "w:\general\sort.h"
#include "w:\general\2d.h"
#include "w:\vectors\scn_run.h"
#include "w:\plugins\flare.h"
#include "w:\general\filters.h"
#include "w:\general\stars.h"

extern "C" void *SYS_ORoutines[];	//Object routines
extern "C" void *SYS_PRoutines[];	//Part routines
extern "C" void *SYS_LRoutines[];	//Light routines
//extern "C" void *SYS_SRoutines[];	//Setup routines

void *SYS_ORoutines[] = { 	NULL,
				&ZERO_Object,		//01 Nullobject
				&KEY_RPressed,		//02 Keypressed?
				&V01,			//03 Tmapping, no shading
				&CNV_Update,          	//04 Update screen
				&V03,			//05 Flatshaded, no tmap
				&SRT_RReset,		//06 Reset sortingtables
				&SRT_RSort,		//07 Sort parts
				&SRT_RRun,		//08 Run parts
				&LIT_InitP,		//09 Run lights' plugins
				&V04,			//10 Flatshading, tmap
				&BRK_RFrames,		//11 Break after n frames
				&V05,			//12 Transparent, flatshad.
				&V06,			//13 Env.map, no shading
				&V07,			//14 Env.map, flatshading
				&PIC,			//15 Show a picture
				&BLN,			//16 Blend the screen
				&FOG,			//17 Fog (using z-buffer)
				&INV,			//18 Invert the colors
				&SCN_RUN,		//19 Run a scene
				&V02,			//20 Mixed
				&V08, 			//21 Glass
				&V09,			//22 Transparent map
				&V10,			//23 Transparent environment map
				&V11,			//24 Transparent additive map
				&V12,			//25 Transparent additive envmap
				&MBL_Process,		//26 Motion Blur filter
				&BLN2_Process,		//27 Additive blending (map)
				&STR_StarField::STR_Run,//28 Starfield
				&BLN3_Process,		//29 Additive blending


};

void *SYS_PRoutines[] = {	NULL,
				&T02_Call,		//01  2 Perspective tmap
				&T04_Call,		//02  4 Flatfilled
				&T05_Call,		//03  5 Perspective + flat
				&T06_Call,		//04  6 Transparent + flat
				&T07_Call,		//05  7 Linear tmap, no shad.
				&T08_Call,		//06  8 Linear tmap, flatshad.
				&T09_Call,		//07  9 Transparent + bumpmap
				&T03_Call,		//08  3 Transparent perspective tmap
				&T10_Call,		//09 10 Transparent additive map, linear
				&FLR_Draw,		//10    Draw a lensflare
};


void *SYS_LRoutines[] = {	NULL,
				&L01,			//Distant light
				&L02,			//Point light
				&L03,			//Spotlight
};

//void *SYS_SRoutines[] = {	NULL,


