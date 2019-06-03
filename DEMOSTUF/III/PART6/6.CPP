
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

#include "x:\iii\part7\7.h"

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

//---- Datavariables for Part6...
int    	       *Objects06_1 	= NULL;
int    	       *Objects06_2 	= NULL;
int    	       *Lights06_1 	= NULL;
int    	       *Lights06_2 	= NULL;
SCN_Header     *SCN06_1 	= NULL;
SCN_Header     *SCN06_2 	= NULL;

void Part6Timing(void);
void Part6MusicSync(void);

BLN_Data Blend06;
INV_Data INV6;


NullObject SyncNull06;
TIM_Data SyncData06 = {
				0, 0,
				2, 0,
				 670976,	//0: scene 2
				1341952,
				0,
				0,
				0,

			};


int InitPart6(void)
{
	int C = 0;

//---- Setting up blending...
	Blend06.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend06.BLN_Plugins[C] = -1;

	Blend06.BLN_R = 0.0;
	Blend06.BLN_G = 0.0;
	Blend06.BLN_B = 0.0;

	Blend06.BLN_Opacity0 = 1.0;
	Blend06.BLN_Opacity1 = 0.0;

	Blend06.BLN_StartFlag = 1;
	Blend06.BLN_StartTime = 0;
	Blend06.BLN_FadingTime = 65536*2;


//---- svada...
	INV6.INV_Type = 18;
	for ( C = 0; C < 16; C++ ) INV6.INV_Plugins[C] = -1;

	INV6.INV_RMul = 1.0;
	INV6.INV_GMul = 1.0;
	INV6.INV_BMul = 1.0;
	INV6.INV_StartFlag = 1;
	INV6.INV_FadingTime = 1000;

	INV6.INV_Opacity0 = 1.0;
	INV6.INV_Opacity1 = 1.0;


//********************************* FIRST SCENE *************************

//---- Loading scene...
	SCN06_1 = SCN_Load("x:\\demo\\test\\scene\\p1.scn");
	if ( SCN06_1 == NULL )
	{
		SYS_Error(1, "InitPart6: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects06_1 = new int[12 + SCN06_1->NrObjects];	//8 without fog
	if ( !Objects06_1 )
	{
		SYS_Error(1, "InitPart6: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects06_1[0] = (int)&Timer;
	Objects06_1[1] = (int)&SRT_OReset;
	Objects06_1[2] = (int)SCN06_1;
	Objects06_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN06_1->NrObjects; C++ )
		Objects06_1[C + 4] = (int)SCN06_1->Objects[C].VEC;

	Objects06_1[C + 4] = (int)&SRT_OSort;
	Objects06_1[C + 5] = (int)&SRT_ORun;
	Objects06_1[C + 6] = (int)&INV6;
	Objects06_1[C + 7] = (int)&Blend06;
	Objects06_1[C + 8] = (int)&Canvas;
	Objects06_1[C + 9] = (int)&KEY_OPressed;
	Objects06_1[C +10] = (int)&SyncNull06;
	Objects06_1[C +11] = -1;

//---- Creating lightlist...
	Lights06_1 = new int[SCN06_1->NrLights + 1];
	if ( !Lights06_1 )
	{
		SYS_Error(1, "InitPart6: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN06_1->NrLights; C++ )
		Lights06_1[C] = (int)SCN06_1->Lights[C].LGT;

	Lights06_1[C] = -1;

	SCN06_1->Objects[5].VEC->H_Materials[1].M_Transparency = 0.001;
	SCN06_1->Envelopes[0].Value = &SCN06_1->Objects[5].VEC->H_Materials[1].M_Transparency;

//********************************* FIRST SCENE *************************

//---- Loading scene...
	SCN06_2 = SCN_Load("x:\\demo\\test\\p2.scn");
	if ( SCN06_2 == NULL )
	{
		SYS_Error(1, "InitPart6: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects06_2 = new int[12 + SCN06_2->NrObjects];	//8 without fog
	if ( !Objects06_2 )
	{
		SYS_Error(1, "InitPart6: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects06_2[0] = (int)&Timer;
	Objects06_2[1] = (int)&SRT_OReset;
	Objects06_2[2] = (int)SCN06_2;
	Objects06_2[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN06_2->NrObjects; C++ )
		Objects06_2[C + 4] = (int)SCN06_2->Objects[C].VEC;

	Objects06_2[C + 4] = (int)&SRT_OSort;
	Objects06_2[C + 5] = (int)&SRT_ORun;
	Objects06_2[C + 6] = (int)&INV6;
	Objects06_2[C + 7] = (int)&Blend06;
	Objects06_2[C + 8] = (int)&Canvas;
	Objects06_2[C + 9] = (int)&KEY_OPressed;
	Objects06_2[C +10] = (int)&SyncNull06;
	Objects06_2[C +11] = -1;

//---- Creating lightlist...
	Lights06_2 = new int[SCN06_2->NrLights + 1];
	if ( !Lights06_2 )
	{
		SYS_Error(1, "InitPart6: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN06_2->NrLights; C++ )
		Lights06_2[C] = (int)SCN06_2->Lights[C].LGT;

	Lights06_2[C] = -1;


	SCN06_2->Envelopes[0].Value = &SCN06_2->Objects[2].VEC->H_Materials[1].M_Transparency;



//---- Setting up sync...
	SyncNull06.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull06.Plugins[C] = -1;
	SyncNull06.Plugins[0] = (int)&Part6Timing;
	SyncNull06.Plugins[1] = (int)&SyncData06;

	return 1;
}//InitPart6


void UnloadPart6(void)
{
	delete [] Objects06_1;
	delete [] Lights06_1;
	delete SCN06_1;

	delete [] Objects06_2;
	delete [] Lights06_2;
	delete SCN06_2;

}//UnloadPart6


void Part6Timing(void)
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
				case 0: SYS_OListOffs = Objects06_2;
					SYS_LListOffs = Lights06_2;
					SCN06_2->StartTime = 670976 + Timer->StartTime;
					break;

				case 1: SYS_OListOffs = Objects07_1;
					SYS_LListOffs = Lights07_1;
					SCN07_1->StartTime = 1341952 + Timer->StartTime;
					SyncData07.StartTime = SCN07_1->StartTime;

					SyncData06.CurrentKey = -1;


				//-----
				//---- Setting up blending...
					Blend06.BLN_R = 0.0;
					Blend06.BLN_G = 0.0;
					Blend06.BLN_B = 0.0;

					Blend06.BLN_Opacity0 = 1.0;
					Blend06.BLN_Opacity1 = 0.0;

					Blend06.BLN_StartFlag = 1;
					Blend06.BLN_StartTime = 0;
					Blend06.BLN_FadingTime = 65536*2;


				//---- svada...
					INV6.INV_RMul = 1.0;
					INV6.INV_GMul = 1.0;
					INV6.INV_BMul = 1.0;
					INV6.INV_StartFlag = 1;
					INV6.INV_FadingTime = 1000;

					INV6.INV_Opacity0 = 1.0;
					INV6.INV_Opacity1 = 1.0;


					break;




			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part6Timing

