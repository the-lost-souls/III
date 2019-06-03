
#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\vectors\scn_file.h"
#include "w:\vectors\scn_run.h"
#include "w:\general\sort.h"
#include "w:\vectors\lights.h"
#include "w:\general\canvas.h"
#include "w:\general\main.h"
#include "w:\pak\pak.h"
#include "w:\gfx\gfx.h"
#include "w:\general\2d.h"
#include "w:\general\filters.h"
#include "w:\general\stars.h"
#include "w:\plugins\flare.h"

#include "w:\divlibs\ims\binfstd.h"
#include "w:\divlibs\ims\ims.h"
#include "w:\divlibs\ims\itplay.h"

#include "x:\iii\part6\6.h"

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>
#include <math.h>

//---- External global stuff...
extern CNV_Data Canvas;
extern NullObject Timer;
extern itplayerclass *itplayer;
extern MBL_Data MBL;

//---- Datavariables for Part5...
int    	       *Objects05_1 	= NULL;
int    	       *Objects05_2 	= NULL;
int    	       *Objects05_3 	= NULL;
int    	       *Objects05_4 	= NULL;
int    	       *Objects05_5 	= NULL;
int    	       *Objects05_6 	= NULL;
int    	       *Objects05_7 	= NULL;
int    	       *Objects05_8 	= NULL;
int    	       *Objects05_9 	= NULL;
int    	       *Objects05_10	= NULL;
int    	       *Lights05_1 	= NULL;
int    	       *Lights05_2 	= NULL;
int    	       *Lights05_3 	= NULL;
int    	       *Lights05_4 	= NULL;
int    	       *Lights05_5 	= NULL;
int    	       *Lights05_6 	= NULL;
int    	       *Lights05_7 	= NULL;
int    	       *Lights05_8 	= NULL;
int    	       *Lights05_9 	= NULL;
int    	       *Lights05_10	= NULL;
SCN_Header     *SCN05_1 	= NULL;
SCN_Header     *SCN05_2 	= NULL;
SCN_Header     *SCN05_3 	= NULL;
SCN_Header     *SCN05_4 	= NULL;
SCN_Header     *SCN05_5 	= NULL;
SCN_Header     *SCN05_6 	= NULL;
SCN_Header     *SCN05_7 	= NULL;
SCN_Header     *SCN05_8 	= NULL;
SCN_Header     *SCN05_9 	= NULL;
SCN_Header     *SCN05_10	= NULL;


void Part5Timing(void);
void Part5MusicSync(void);

BLN_Data Blend05;
BLN3_Data AddBlend5;
INV_Data INV1;


NullObject SyncNull05;
TIM_Data SyncData05 = {
				0, 0,
				14, 0,
				 167744,	//0: scene 2
				 293552,	//1: scene 3
				 314520,	//2: scene 4
			         335488,	//3: scene 5
				 554400,	//4: fade out scene 5
				 587104,	//5: scene 6
				 620976,	//6: fade out scene 6
				 670976,	//7: scene 7
				 805952,	//8: fade out scene 7
				 838720,	//9: scene 8
				 964528,	//10: scene 9
				 985496,
				 1006464,	//11: scene 10
				 1341952,	//12: next part
				0,
				0,
				0,

			};


int InitPart5(void)
{
	int C = 0;

//---- Setting up blending...
	Blend05.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend05.BLN_Plugins[C] = -1;

	Blend05.BLN_R = 0.0;
	Blend05.BLN_G = 0.0;
	Blend05.BLN_B = 0.0;

	Blend05.BLN_Opacity0 = 0.0;
	Blend05.BLN_Opacity1 = 1.0;

	Blend05.BLN_StartFlag = 1;
	Blend05.BLN_StartTime = 0;
	Blend05.BLN_FadingTime = 65536*3;


//---- svada...
	AddBlend5.BLN3_StartTime = 0;
	AddBlend5.BLN3_FadingTime = 65536;

	AddBlend5.BLN3_Opacity0 = 1.0;
	AddBlend5.BLN3_Opacity1 = 0.0;
	AddBlend5.BLN3_BlendR = 255;
	AddBlend5.BLN3_BlendG = 255;
	AddBlend5.BLN3_BlendB = 255;

	INV1.INV_Type = 18;
	for ( C = 0; C < 16; C++ ) INV1.INV_Plugins[C] = -1;

	INV1.INV_RMul = 1.0;
	INV1.INV_GMul = 1.0;
	INV1.INV_BMul = 1.0;
	INV1.INV_StartFlag = 1;
	INV1.INV_FadingTime = 2000000000;
	INV1.INV_Opacity0 = 1;
	INV1.INV_Opacity1 = 1;


//********************************* FIRST SCENE *************************

//---- Loading scene...
	SCN05_1 = SCN_Load("x:\\demo\\frames\\k01.scn");
	if ( SCN05_1 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_1 = new int[12 + SCN05_1->NrObjects];	//8 without fog
	if ( !Objects05_1 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_1[0] = (int)&Timer;
	Objects05_1[1] = (int)&SRT_OReset;
	Objects05_1[2] = (int)SCN05_1;
	Objects05_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_1->NrObjects; C++ )
		Objects05_1[C + 4] = (int)SCN05_1->Objects[C].VEC;

	Objects05_1[C + 4] = (int)&SRT_OSort;
	Objects05_1[C + 5] = (int)&SRT_ORun;
	Objects05_1[C + 6] = (int)&Blend05;
	Objects05_1[C + 7] = (int)&AddBlend5;
	Objects05_1[C + 8] = (int)&Canvas;
	Objects05_1[C + 9] = (int)&KEY_OPressed;
	Objects05_1[C +10] = (int)&SyncNull05;
	Objects05_1[C +11] = -1;

//---- Creating lightlist...
	Lights05_1 = new int[SCN05_1->NrLights + 1];
	if ( !Lights05_1 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_1->NrLights; C++ )
		Lights05_1[C] = (int)SCN05_1->Lights[C].LGT;

	Lights05_1[C] = -1;



//********************************* 2nd SCENE *************************

//---- Loading scene...
	SCN05_2 = SCN_Load("x:\\demo\\frames\\k02.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_2 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_2 = new int[12 + SCN05_2->NrObjects];	//8 without fog
	if ( !Objects05_2 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_2[0] = (int)&Timer;
	Objects05_2[1] = (int)&SRT_OReset;
	Objects05_2[2] = (int)SCN05_2;
	Objects05_2[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_2->NrObjects; C++ )
		Objects05_2[C + 4] = (int)SCN05_2->Objects[C].VEC;

	Objects05_2[C + 4] = (int)&SRT_OSort;
	Objects05_2[C + 5] = (int)&SRT_ORun;
	Objects05_2[C + 6] = (int)&Blend05;
	Objects05_2[C + 7] = (int)0;
	Objects05_2[C + 8] = (int)&Canvas;
	Objects05_2[C + 9] = (int)&KEY_OPressed;
	Objects05_2[C +10] = (int)&SyncNull05;
	Objects05_2[C +11] = -1;

//---- Creating lightlist...
	Lights05_2 = new int[SCN05_2->NrLights + 1];
	if ( !Lights05_2 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_2->NrLights; C++ )
		Lights05_2[C] = (int)SCN05_2->Lights[C].LGT;

	Lights05_2[C] = -1;




//********************************* 3rd SCENE *************************

//---- Loading scene...
	SCN05_3 = SCN_Load("x:\\demo\\frames\\k03.scn");
	if ( SCN05_3 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_3 = new int[12 + SCN05_3->NrObjects];	//8 without fog
	if ( !Objects05_3 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_3[0] = (int)&Timer;
	Objects05_3[1] = (int)&SRT_OReset;
	Objects05_3[2] = (int)SCN05_3;
	Objects05_3[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_3->NrObjects; C++ )
		Objects05_3[C + 4] = (int)SCN05_3->Objects[C].VEC;

	Objects05_3[C + 4] = (int)&SRT_OSort;
	Objects05_3[C + 5] = (int)&SRT_ORun;
	Objects05_3[C + 6] = (int)0;
	Objects05_3[C + 7] = (int)0;
	Objects05_3[C + 8] = (int)&Canvas;
	Objects05_3[C + 9] = (int)&KEY_OPressed;
	Objects05_3[C +10] = (int)&SyncNull05;
	Objects05_3[C +11] = -1;

//---- Creating lightlist...
	Lights05_3 = new int[SCN05_3->NrLights + 1];
	if ( !Lights05_3 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_3->NrLights; C++ )
		Lights05_3[C] = (int)SCN05_3->Lights[C].LGT;

	Lights05_3[C] = -1;




//********************************* 4th SCENE *************************

//---- Loading scene...
	SCN05_4 = SCN_Load("x:\\demo\\frames\\k04.scn");
	if ( SCN05_4 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_4 = new int[12 + SCN05_4->NrObjects];	//8 without fog
	if ( !Objects05_4 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_4[0] = (int)&Timer;
	Objects05_4[1] = (int)&SRT_OReset;
	Objects05_4[2] = (int)SCN05_4;
	Objects05_4[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_4->NrObjects; C++ )
		Objects05_4[C + 4] = (int)SCN05_4->Objects[C].VEC;

	Objects05_4[C + 4] = (int)&SRT_OSort;
	Objects05_4[C + 5] = (int)&SRT_ORun;
	Objects05_4[C + 6] = (int)0;
	Objects05_4[C + 7] = (int)0;
	Objects05_4[C + 8] = (int)&Canvas;
	Objects05_4[C + 9] = (int)&KEY_OPressed;
	Objects05_4[C +10] = (int)&SyncNull05;
	Objects05_4[C +11] = -1;

//---- Creating lightlist...
	Lights05_4 = new int[SCN05_4->NrLights + 1];
	if ( !Lights05_4 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_4->NrLights; C++ )
		Lights05_4[C] = (int)SCN05_4->Lights[C].LGT;

	Lights05_4[C] = -1;


//********************************* 5th SCENE *************************

//---- Loading scene...
	SCN05_5 = SCN_Load("x:\\demo\\frames\\k05.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_5 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_5 = new int[12 + SCN05_5->NrObjects];	//8 without fog
	if ( !Objects05_5 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_5[0] = (int)&Timer;
	Objects05_5[1] = (int)&SRT_OReset;
	Objects05_5[2] = (int)SCN05_5;
	Objects05_5[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_5->NrObjects; C++ )
		Objects05_5[C + 4] = (int)SCN05_5->Objects[C].VEC;

	Objects05_5[C + 4] = (int)&SRT_OSort;
	Objects05_5[C + 5] = (int)&SRT_ORun;
	Objects05_5[C + 6] = (int)&AddBlend5;
	Objects05_5[C + 7] = (int)0;
	Objects05_5[C + 8] = (int)&Canvas;
	Objects05_5[C + 9] = (int)&KEY_OPressed;
	Objects05_5[C +10] = (int)&SyncNull05;
	Objects05_5[C +11] = -1;

//---- Creating lightlist...
	Lights05_5 = new int[SCN05_5->NrLights + 1];
	if ( !Lights05_5 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_5->NrLights; C++ )
		Lights05_5[C] = (int)SCN05_5->Lights[C].LGT;

	Lights05_5[C] = -1;


//********************************* 6th SCENE *************************

//---- Loading scene...
	SCN05_6 = SCN_Load("x:\\demo\\frames\\k06.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_6 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_6 = new int[12 + SCN05_6->NrObjects];	//8 without fog
	if ( !Objects05_6 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_6[0] = (int)&Timer;
	Objects05_6[1] = (int)&SRT_OReset;
	Objects05_6[2] = (int)SCN05_6;
	Objects05_6[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_6->NrObjects; C++ )
		Objects05_6[C + 4] = (int)SCN05_6->Objects[C].VEC;

	Objects05_6[C + 4] = (int)&SRT_OSort;
	Objects05_6[C + 5] = (int)&SRT_ORun;
	Objects05_6[C + 6] = (int)&INV1;
	Objects05_6[C + 7] = (int)&AddBlend5;
	Objects05_6[C + 8] = (int)&Canvas;
	Objects05_6[C + 9] = (int)&KEY_OPressed;
	Objects05_6[C +10] = (int)&SyncNull05;
	Objects05_6[C +11] = -1;

//---- Creating lightlist...
	Lights05_6 = new int[SCN05_6->NrLights + 1];
	if ( !Lights05_6 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_6->NrLights; C++ )
		Lights05_6[C] = (int)SCN05_6->Lights[C].LGT;

	Lights05_6[C] = -1;



//********************************* 7th SCENE *************************

//---- Loading scene...
	SCN05_7 = SCN_Load("x:\\demo\\frames\\k07.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_7 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_7 = new int[12 + SCN05_7->NrObjects];	//8 without fog
	if ( !Objects05_7 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_7[0] = (int)&Timer;
	Objects05_7[1] = (int)&SRT_OReset;
	Objects05_7[2] = (int)SCN05_7;
	Objects05_7[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_7->NrObjects; C++ )
		Objects05_7[C + 4] = (int)SCN05_7->Objects[C].VEC;

	Objects05_7[C + 4] = (int)&SRT_OSort;
	Objects05_7[C + 5] = (int)&SRT_ORun;
	Objects05_7[C + 6] = (int)&Blend05;
	Objects05_7[C + 7] = (int)0;
	Objects05_7[C + 8] = (int)&Canvas;
	Objects05_7[C + 9] = (int)&KEY_OPressed;
	Objects05_7[C +10] = (int)&SyncNull05;
	Objects05_7[C +11] = -1;

//---- Creating lightlist...
	Lights05_7 = new int[SCN05_7->NrLights + 1];
	if ( !Lights05_7 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_7->NrLights; C++ )
		Lights05_7[C] = (int)SCN05_7->Lights[C].LGT;

	Lights05_7[C] = -1;


//********************************* 8th SCENE *************************

//---- Loading scene...
	SCN05_8 = SCN_Load("x:\\demo\\frames\\k08.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_8 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_8 = new int[12 + SCN05_8->NrObjects];	//8 without fog
	if ( !Objects05_8 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_8[0] = (int)&Timer;
	Objects05_8[1] = (int)&SRT_OReset;
	Objects05_8[2] = (int)SCN05_8;
	Objects05_8[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_8->NrObjects; C++ )
		Objects05_8[C + 4] = (int)SCN05_8->Objects[C].VEC;

	Objects05_8[C + 4] = (int)&SRT_OSort;
	Objects05_8[C + 5] = (int)&SRT_ORun;
	Objects05_8[C + 6] = (int)&Blend05;
	Objects05_8[C + 7] = (int)0;
	Objects05_8[C + 8] = (int)&Canvas;
	Objects05_8[C + 9] = (int)&KEY_OPressed;
	Objects05_8[C +10] = (int)&SyncNull05;
	Objects05_8[C +11] = -1;

//---- Creating lightlist...
	Lights05_8 = new int[SCN05_8->NrLights + 1];
	if ( !Lights05_8 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_8->NrLights; C++ )
		Lights05_8[C] = (int)SCN05_8->Lights[C].LGT;

	Lights05_8[C] = -1;


//********************************* 9th SCENE *************************

//---- Loading scene...
	SCN05_9 = SCN_Load("x:\\demo\\frames\\k09.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_9 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_9 = new int[12 + SCN05_9->NrObjects];	//8 without fog
	if ( !Objects05_9 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_9[0] = (int)&Timer;
	Objects05_9[1] = (int)&SRT_OReset;
	Objects05_9[2] = (int)SCN05_9;
	Objects05_9[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_9->NrObjects; C++ )
		Objects05_9[C + 4] = (int)SCN05_9->Objects[C].VEC;

	Objects05_9[C + 4] = (int)&SRT_OSort;
	Objects05_9[C + 5] = (int)&SRT_ORun;
	Objects05_9[C + 6] = (int)0;
	Objects05_9[C + 7] = (int)&INV1;
	Objects05_9[C + 8] = (int)&Canvas;
	Objects05_9[C + 9] = (int)&KEY_OPressed;
	Objects05_9[C +10] = (int)&SyncNull05;
	Objects05_9[C +11] = -1;

//---- Creating lightlist...
	Lights05_9 = new int[SCN05_9->NrLights + 1];
	if ( !Lights05_9 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_9->NrLights; C++ )
		Lights05_9[C] = (int)SCN05_9->Lights[C].LGT;

	Lights05_9[C] = -1;


//********************************* 10th SCENE *************************

//---- Loading scene...
	SCN05_10 = SCN_Load("x:\\demo\\frames\\k10.scn", SCN_FILE_FlagDefault, SCN05_1);
	if ( SCN05_10 == NULL )
	{
		SYS_Error(1, "InitPart5: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05_10 = new int[12 + SCN05_10->NrObjects];	//8 without fog
	if ( !Objects05_10 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05_10[0] = (int)&Timer;
	Objects05_10[1] = (int)&SRT_OReset;
	Objects05_10[2] = (int)SCN05_10;
	Objects05_10[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05_10->NrObjects; C++ )
		Objects05_10[C + 4] = (int)SCN05_10->Objects[C].VEC;

	Objects05_10[C + 4] = (int)&SRT_OSort;
	Objects05_10[C + 5] = (int)&SRT_ORun;
	Objects05_10[C + 6] = (int)&Blend05;
	Objects05_10[C + 7] = (int)0;
	Objects05_10[C + 8] = (int)&Canvas;
	Objects05_10[C + 9] = (int)&KEY_OPressed;
	Objects05_10[C +10] = (int)&SyncNull05;
	Objects05_10[C +11] = -1;

//---- Creating lightlist...
	Lights05_10 = new int[SCN05_10->NrLights + 1];
	if ( !Lights05_10 )
	{
		SYS_Error(1, "InitPart5: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05_10->NrLights; C++ )
		Lights05_10[C] = (int)SCN05_10->Lights[C].LGT;

	Lights05_10[C] = -1;




//---- Setting up sync...
	SyncNull05.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull05.Plugins[C] = -1;
	SyncNull05.Plugins[0] = (int)&Part5Timing;
	SyncNull05.Plugins[1] = (int)&SyncData05;

	return 1;
}//InitPart5


void UnloadPart5(void)
{
	delete [] Objects05_1;
	delete [] Lights05_1;
	delete SCN05_1;

	delete [] Objects05_2;
	delete [] Lights05_2;
	delete SCN05_2;

	delete [] Objects05_3;
	delete [] Lights05_3;
	delete SCN05_3;

	delete [] Objects05_4;
	delete [] Lights05_4;
	delete SCN05_4;

	delete [] Objects05_5;
	delete [] Lights05_5;
	delete SCN05_5;

	delete [] Objects05_6;
	delete [] Lights05_6;
	delete SCN05_6;

	delete [] Objects05_7;
	delete [] Lights05_7;
	delete SCN05_7;

	delete [] Objects05_8;
	delete [] Lights05_8;
	delete SCN05_8;

	delete [] Objects05_9;
	delete [] Lights05_9;
	delete SCN05_9;

	delete [] Objects05_10;
	delete [] Lights05_10;
	delete SCN05_10;

}//UnloadPart5


void Part5MusicSync(void)
{
  	int time = 0;

//---- Checking for flash...
  	int val = itplayer->getsync(0, time);

	if ( (time != -1) && (val == 00) )
	{
		Blend05.BLN_StartTime = SYS_TimerTick - time;
		Blend05.BLN_Opacity0 = 0.7;
		Blend05.BLN_Opacity1 = 0.0;
	}//if

}//void


void Part5Timing(void)
{
	TIM_Data *Timer = (TIM_Data*)SYS_POffs;
	if ( Timer->StartFlag == 0 )
	{
		Timer->StartFlag = 1;
		Timer->CurrentKey = 0;
	}//if

	if ( Timer->CurrentKey == Timer->NrKeys ) Timer->CurrentKey = -1;

	if ( Timer->CurrentKey != -1 )
	{
		int Time = SYS_TimerTick - Timer->StartTime;

		if ( Time >= Timer->TimingKeys[Timer->CurrentKey] )
		{
			switch ( Timer->CurrentKey )
			{
				case 0: SYS_OListOffs = Objects05_2;
					SYS_LListOffs = Lights05_2;
					SCN05_2->StartTime = 167744 + Timer->StartTime;

					Blend05.BLN_StartTime = 167744 + Timer->StartTime;
					Blend05.BLN_FadingTime = 65536*2;

					break;

				case 1: SYS_OListOffs = Objects05_3;
					SYS_LListOffs = Lights05_3;
					SCN05_3->StartTime = 293552 + Timer->StartTime;

					break;

				case 2: SYS_OListOffs = Objects05_4;
					SYS_LListOffs = Lights05_4;
					SCN05_4->StartTime = 314520 + Timer->StartTime;

					break;

				case 3: SYS_OListOffs = Objects05_5;
					SYS_LListOffs = Lights05_5;

					SCN05_5->StartTime = 335488 + Timer->StartTime;

					AddBlend5.BLN3_StartTime = 335488 + Timer->StartTime;
					AddBlend5.BLN3_FadingTime = 32768;

					AddBlend5.BLN3_Opacity0 = 1.0;
					AddBlend5.BLN3_Opacity1 = 0.0;

					break;

				case 4: AddBlend5.BLN3_StartTime = 554400 + Timer->StartTime;
					AddBlend5.BLN3_FadingTime = 32768;

					AddBlend5.BLN3_Opacity0 = 0.0;
					AddBlend5.BLN3_Opacity1 = 1.0;


					break;

				case 5: SYS_OListOffs = Objects05_6;
					SYS_LListOffs = Lights05_6;
					SCN05_6->StartTime = 587104 + Timer->StartTime;

					AddBlend5.BLN3_StartTime = 587104 + Timer->StartTime;
					AddBlend5.BLN3_FadingTime = 32768;

					AddBlend5.BLN3_Opacity0 = 1.0;
					AddBlend5.BLN3_Opacity1 = 0.0;

					Blend05.BLN_Opacity0 = 0.0;
					Blend05.BLN_Opacity1 = 0.0;

					break;

				case 6: Blend05.BLN_Opacity0 = 0.0;
					Blend05.BLN_Opacity1 = 1.0;

					Blend05.BLN_R = 0.0;
					Blend05.BLN_G = 0.0;
					Blend05.BLN_B = 0.0;

					Blend05.BLN_FadingTime = 50000;

					Blend05.BLN_StartTime = 620976 + Timer->StartTime;

					break;

				case 7: SYS_OListOffs = Objects05_7;
					SYS_LListOffs = Lights05_7;
					SCN05_7->StartTime =  670976 + Timer->StartTime;

					Blend05.BLN_StartTime = 670976 + Timer->StartTime;
					Blend05.BLN_FadingTime = 32768;

					Blend05.BLN_Opacity0 = 1.0;
					Blend05.BLN_Opacity1 = 0.0;

					break;

				case 8: Blend05.BLN_Opacity0 = 0.0;
					Blend05.BLN_Opacity1 = 1.0;

					Blend05.BLN_FadingTime = 32768;
					Blend05.BLN_StartTime = 805952 + Timer->StartTime;

					break;

				case 9: SYS_OListOffs = Objects05_8;
					SYS_LListOffs = Lights05_8;
					SCN05_8->StartTime =  838720 + Timer->StartTime;

					Blend05.BLN_StartTime = 838720 + Timer->StartTime;
					Blend05.BLN_FadingTime = 32768;

					Blend05.BLN_Opacity0 = 1.0;
					Blend05.BLN_Opacity1 = 0.0;

					break;

				case 10: SYS_OListOffs = Objects05_9;
					SYS_LListOffs = Lights05_9;
					SCN05_9->StartTime =  964528 + Timer->StartTime;

					break;

				case 11:INV1.INV_FadingTime = 1;
					INV1.INV_Opacity0 = 0.0;
					INV1.INV_Opacity1 = 0.0;
					break;

				case 12: SYS_OListOffs = Objects05_10;
					SYS_LListOffs = Lights05_10;
					SCN05_10->StartTime =  1006464 + Timer->StartTime;

					Blend05.BLN_StartTime = 1006464 + Timer->StartTime;
					Blend05.BLN_FadingTime = 65536*2;

					Blend05.BLN_Opacity0 = 1.0;
					Blend05.BLN_Opacity1 = 0.0;

					break;

				case 13: SYS_OListOffs = Objects06_1;
					SYS_LListOffs = Lights06_1;
					SCN06_1->StartTime =  1341952 + Timer->StartTime;
					SyncData06.StartTime = SCN06_1->StartTime;

					Blend06.BLN_StartTime = 1341952 + Timer->StartTime;

					SyncData05.CurrentKey = -1;

				//--------
					Blend05.BLN_R = 0.0;
					Blend05.BLN_G = 0.0;
					Blend05.BLN_B = 0.0;

					Blend05.BLN_Opacity0 = 0.0;
					Blend05.BLN_Opacity1 = 1.0;

					Blend05.BLN_StartFlag = 1;
					Blend05.BLN_StartTime = 0;
					Blend05.BLN_FadingTime = 65536*3;


					AddBlend5.BLN3_StartTime = 0;
					AddBlend5.BLN3_FadingTime = 65536;

					AddBlend5.BLN3_Opacity0 = 1.0;
					AddBlend5.BLN3_Opacity1 = 0.0;
					AddBlend5.BLN3_BlendR = 255;
					AddBlend5.BLN3_BlendG = 255;
					AddBlend5.BLN3_BlendB = 255;

					INV1.INV_RMul = 1.0;
					INV1.INV_GMul = 1.0;
					INV1.INV_BMul = 1.0;
					INV1.INV_StartFlag = 1;
					INV1.INV_FadingTime = 2000000000;
					INV1.INV_Opacity0 = 1;
					INV1.INV_Opacity1 = 1;


					break;



			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part5Timing

