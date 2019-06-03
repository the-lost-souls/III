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

//---- Datavariables for Part8...
int    	       *Objects08_1 	= NULL;
int    	       *Objects08_2 	= NULL;
int    	       *Objects08_3 	= NULL;
int    	       *Objects08_4 	= NULL;
int    	       *Objects08_5 	= NULL;
int    	       *Objects08_6 	= NULL;
int    	       *Lights08_1 	= NULL;
int    	       *Lights08_2 	= NULL;
int    	       *Lights08_3 	= NULL;
int    	       *Lights08_4 	= NULL;
int    	       *Lights08_5 	= NULL;
int    	       *Lights08_6 	= NULL;
SCN_Header     *SCN08_1 	= NULL;
SCN_Header     *SCN08_2 	= NULL;
SCN_Header     *SCN08_3 	= NULL;
SCN_Header     *SCN08_4 	= NULL;
SCN_Header     *SCN08_5 	= NULL;

void Part8Timing(void);
void Part8MusicSync(void);
void Rotate2D(float X0, float Y0, float Angle, float &X1, float &Y1);
void MapCoordPlugin(void);

BLN_Data Blend08;
INV_Data INV8;
BLN3_Data AddBlend08;


NullObject SyncNull08;
TIM_Data SyncData08 = {
				0, 0,
				7, 0,
				 335488,	//0: fade scene 1
				 670976,	//1: scene 2
				1006464, 	//2: scene 3
				1341952,
				2012928,
				2537216 ,
				2012928 + 65536*20,
				0,
				0,

			};



int InitPart8(void)
{
	int C = 0;

//---- Setting up blending...
	Blend08.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend08.BLN_Plugins[C] = -1;

	Blend08.BLN_R = 0.0;
	Blend08.BLN_G = 0.0;
	Blend08.BLN_B = 0.0;

	Blend08.BLN_Opacity0 = 0.0;
	Blend08.BLN_Opacity1 = 0.0;

	Blend08.BLN_StartFlag = 1;
	Blend08.BLN_StartTime = 0;
	Blend08.BLN_FadingTime = 65536;


//---- r6tyuijkopl‘
	AddBlend08.BLN3_StartTime = 0;
	AddBlend08.BLN3_FadingTime = 65536*3;

	AddBlend08.BLN3_Opacity0 = 0.0;
	AddBlend08.BLN3_Opacity1 = 0.0;
	AddBlend08.BLN3_BlendR = 255;
	AddBlend08.BLN3_BlendG = 255;
	AddBlend08.BLN3_BlendB = 255;



//---- svada...
	INV8.INV_Type = 18;
	for ( C = 0; C < 16; C++ ) INV8.INV_Plugins[C] = -1;

	INV8.INV_RMul = 1.0;
	INV8.INV_GMul = 1.0;
	INV8.INV_BMul = 1.0;
	INV8.INV_StartFlag = 0;
	INV8.INV_FadingTime = 2000000000;
	INV8.INV_Opacity0 = 0.0;
	INV8.INV_Opacity1 = 0.0;


//********************************* FIRST SCENE *************************

//---- Loading scene...
	SCN08_1 = SCN_Load("x:\\demo\\worm\\scenes\\w1.scn");
	if ( SCN08_1 == NULL )
	{
		SYS_Error(1, "InitPart8: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08_1 = new int[12 + SCN08_1->NrObjects];	//8 without fog
	if ( !Objects08_1 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08_1[0] = (int)&Timer;
	Objects08_1[1] = (int)&SRT_OReset;
	Objects08_1[2] = (int)SCN08_1;
	Objects08_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08_1->NrObjects; C++ )
		Objects08_1[C + 4] = (int)SCN08_1->Objects[C].VEC;

	Objects08_1[C + 4] = (int)&SRT_OSort;
	Objects08_1[C + 5] = (int)&SRT_ORun;
	Objects08_1[C + 6] = (int)&MBL;
	Objects08_1[C + 7] = (int)0;
	Objects08_1[C + 8] = (int)&Canvas;
	Objects08_1[C + 9] = (int)&KEY_OPressed;
	Objects08_1[C +10] = (int)&SyncNull08;
	Objects08_1[C +11] = -1;

//---- Creating lightlist...
	Lights08_1 = new int[SCN08_1->NrLights + 1];
	if ( !Lights08_1 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08_1->NrLights; C++ )
		Lights08_1[C] = (int)SCN08_1->Lights[C].LGT;

	Lights08_1[C] = -1;


//********************************* 2. SCENE *************************

//---- Loading scene...
	SCN08_2 = SCN_Load("x:\\demo\\worm\\scenes\\w2.scn");
	if ( SCN08_2 == NULL )
	{
		SYS_Error(1, "InitPart8: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08_2 = new int[12 + SCN08_2->NrObjects];	//8 without fog
	if ( !Objects08_2 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08_2[0] = (int)&Timer;
	Objects08_2[1] = (int)&SRT_OReset;
	Objects08_2[2] = (int)SCN08_2;
	Objects08_2[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08_2->NrObjects; C++ )
		Objects08_2[C + 4] = (int)SCN08_2->Objects[C].VEC;

	Objects08_2[C + 4] = (int)&SRT_OSort;
	Objects08_2[C + 5] = (int)&SRT_ORun;
	Objects08_2[C + 6] = (int)&MBL;
	Objects08_2[C + 7] = (int)0;
	Objects08_2[C + 8] = (int)&Canvas;
	Objects08_2[C + 9] = (int)&KEY_OPressed;
	Objects08_2[C +10] = (int)&SyncNull08;
	Objects08_2[C +11] = -1;

//---- Creating lightlist...
	Lights08_2 = new int[SCN08_2->NrLights + 1];
	if ( !Lights08_2 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08_2->NrLights; C++ )
		Lights08_2[C] = (int)SCN08_2->Lights[C].LGT;

	Lights08_2[C] = -1;


//********************************* 3. SCENE *************************

//---- Loading scene...
	SCN08_3 = SCN_Load("x:\\demo\\worm\\scenes\\w3.scn");
	if ( SCN08_3 == NULL )
	{
		SYS_Error(1, "InitPart8: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08_3 = new int[12 + SCN08_3->NrObjects];	//8 without fog
	if ( !Objects08_3 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08_3[0] = (int)&Timer;
	Objects08_3[1] = (int)&SRT_OReset;
	Objects08_3[2] = (int)SCN08_3;
	Objects08_3[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08_3->NrObjects; C++ )
		Objects08_3[C + 4] = (int)SCN08_3->Objects[C].VEC;

	Objects08_3[C + 4] = (int)&SRT_OSort;
	Objects08_3[C + 5] = (int)&SRT_ORun;
	Objects08_3[C + 6] = (int)&MBL;
	Objects08_3[C + 7] = (int)&Blend08;
	Objects08_3[C + 8] = (int)&Canvas;
	Objects08_3[C + 9] = (int)&KEY_OPressed;
	Objects08_3[C +10] = (int)&SyncNull08;
	Objects08_3[C +11] = -1;

//---- Creating lightlist...
	Lights08_3 = new int[SCN08_3->NrLights + 1];
	if ( !Lights08_3 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08_3->NrLights; C++ )
		Lights08_3[C] = (int)SCN08_3->Lights[C].LGT;

	Lights08_3[C] = -1;


//********************************* 4. SCENE *************************

//---- Loading scene...
	SCN08_4 = SCN_Load("x:\\demo\\worm\\scenes\\w4.scn");
	if ( SCN08_4 == NULL )
	{
		SYS_Error(1, "InitPart8: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08_4 = new int[13 + SCN08_4->NrObjects];	//8 without fog
	if ( !Objects08_4 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08_4[0] = (int)&Timer;
	Objects08_4[1] = (int)&SRT_OReset;
	Objects08_4[2] = (int)SCN08_4;
	Objects08_4[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08_4->NrObjects; C++ )
		Objects08_4[C + 4] = (int)SCN08_4->Objects[C].VEC;

	Objects08_4[C + 4] = (int)&SRT_OSort;
	Objects08_4[C + 5] = (int)&SRT_ORun;
	Objects08_4[C + 6] = (int)&MBL;
	Objects08_4[C + 7] = (int)&Blend08;
	Objects08_4[C + 8] = (int)&AddBlend08;
	Objects08_4[C + 9] = (int)&Canvas;
	Objects08_4[C +10] = (int)&KEY_OPressed;
	Objects08_4[C +11] = (int)&SyncNull08;
	Objects08_4[C +12] = -1;

//---- Creating lightlist...
	Lights08_4 = new int[SCN08_4->NrLights + 1];
	if ( !Lights08_4 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08_4->NrLights; C++ )
		Lights08_4[C] = (int)SCN08_4->Lights[C].LGT;

	Lights08_4[C] = -1;



//********************************* 5. SCENE *************************

//---- Loading scene...
	SCN08_5 = SCN_Load("x:\\demo\\test\\scene\\tls_fade.scn");
	if ( SCN08_5 == NULL )
	{
		SYS_Error(1, "InitPart8: Error loading scenes\\hall01.scn.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08_5 = new int[13 + SCN08_5->NrObjects];	//8 without fog
	if ( !Objects08_5 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08_5[0] = (int)&Timer;
	Objects08_5[1] = (int)&SRT_OReset;
	Objects08_5[2] = (int)SCN08_5;
	Objects08_5[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08_5->NrObjects; C++ )
		Objects08_5[C + 4] = (int)SCN08_5->Objects[C].VEC;

	Objects08_5[C + 4] = (int)&SRT_OSort;
	Objects08_5[C + 5] = (int)&SRT_ORun;
	Objects08_5[C + 6] = (int)&MBL;
	Objects08_5[C + 7] = (int)&Blend08;
	Objects08_5[C + 8] = (int)&AddBlend08;
	Objects08_5[C + 9] = (int)&Canvas;
	Objects08_5[C +10] = (int)&KEY_OPressed;
	Objects08_5[C +11] = (int)&SyncNull08;
	Objects08_5[C +12] = -1;

//---- Creating lightlist...
	Lights08_5 = new int[SCN08_5->NrLights + 1];
	if ( !Lights08_5 )
	{
		SYS_Error(1, "InitPart8: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08_5->NrLights; C++ )
		Lights08_5[C] = (int)SCN08_5->Lights[C].LGT;

	Lights08_5[C] = -1;



//---- Setting up sync...
	SyncNull08.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull08.Plugins[C] = -1;
	SyncNull08.Plugins[0] = (int)&Part8Timing;
	SyncNull08.Plugins[1] = (int)&SyncData08;


	return 1;
}//InitPart8


void UnloadPart8(void)
{
	delete [] Objects08_1;
	delete [] Lights08_1;
	delete SCN08_1;

	delete [] Objects08_2;
	delete [] Lights08_2;
	delete SCN08_2;

}//UnloadPart8


void Part8Timing(void)
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
				case 0: SYS_OListOffs = Objects08_2;
					SYS_LListOffs = Lights08_2;
					SCN08_2->StartTime = 335488 + Timer->StartTime;
					break;

				case 1: SYS_OListOffs = Objects08_3;
					SYS_LListOffs = Lights08_3;
					SCN08_3->StartTime = 670976 + Timer->StartTime;

					Blend08.BLN_Opacity0 = 1.0;
					Blend08.BLN_Opacity1 = 0.0;
					Blend08.BLN_StartTime = 670976 + Timer->StartTime;
					break;

				case 2: SYS_OListOffs = Objects08_4;
					SYS_LListOffs = Lights08_4;
					SCN08_4->StartTime = 1006464 + Timer->StartTime;

					Blend08.BLN_Opacity0 = 1.0;
					Blend08.BLN_Opacity1 = 0.0;
					Blend08.BLN_StartTime = 1006464 + Timer->StartTime;


					AddBlend08.BLN3_StartTime = 1677440 + Timer->StartTime;
					AddBlend08.BLN3_FadingTime = 335488;

					AddBlend08.BLN3_Opacity0 = 0.0;
					AddBlend08.BLN3_Opacity1 = 1.0;

					break;


				case 3: Blend08.BLN_Opacity0 = 0.8;
					Blend08.BLN_Opacity1 = 0.0;
					Blend08.BLN_StartTime = 1341952 + Timer->StartTime;

					Blend08.BLN_R = 1.0;
					Blend08.BLN_G = 1.0;
					Blend08.BLN_B = 1.0;
					break;



				case 4: SYS_OListOffs = Objects08_5;
					SYS_LListOffs = Lights08_5;
					SCN08_5->StartTime = 2012928 + Timer->StartTime;

					AddBlend08.BLN3_StartTime = 2012928 + Timer->StartTime;
					AddBlend08.BLN3_FadingTime = 65536*2;

					AddBlend08.BLN3_Opacity0 = 1.0;
					AddBlend08.BLN3_Opacity1 = 0.0;

					SYS_FPClipTop = 0;
					SYS_FPClipBottom = SYS_YRes - 1;

					Blend08.BLN_Opacity0 = 0.0;
					Blend08.BLN_Opacity1 = 0.0;

					Blend08.BLN_R = 0.0;
					Blend08.BLN_G = 0.0;
					Blend08.BLN_B = 0.0;

					break;

				case 5: Blend08.BLN_Opacity0 = 0.0;
					Blend08.BLN_Opacity1 = 1.0;
					Blend08.BLN_StartTime = 2537216 + Timer->StartTime;
					Blend08.BLN_FadingTime = 65536*10;

					break;


				case 6:	SYS_QuitFlag = -2;
					SYS_Error(1, "\"Wow!\"\n\n");
					SYS_Error(1, "III - live tomorrow\n");
					SYS_Error(1, "by The Lost Souls 1999\n");
					SYS_Error(1, "www.tls.no\n");

				//-------a osdfjaso›dfja
					Blend08.BLN_R = 0.0;
					Blend08.BLN_G = 0.0;
					Blend08.BLN_B = 0.0;

					Blend08.BLN_Opacity0 = 0.0;
					Blend08.BLN_Opacity1 = 0.0;

					Blend08.BLN_StartFlag = 1;
					Blend08.BLN_StartTime = 0;
					Blend08.BLN_FadingTime = 65536;


				//---- r6tyuijkopl‘
					AddBlend08.BLN3_StartTime = 0;
					AddBlend08.BLN3_FadingTime = 65536*3;

					AddBlend08.BLN3_Opacity0 = 0.0;
					AddBlend08.BLN3_Opacity1 = 0.0;
					AddBlend08.BLN3_BlendR = 255;
					AddBlend08.BLN3_BlendG = 255;
					AddBlend08.BLN3_BlendB = 255;



				//---- svada...
					INV8.INV_RMul = 1.0;
					INV8.INV_GMul = 1.0;
					INV8.INV_BMul = 1.0;
					INV8.INV_StartFlag = 0;
					INV8.INV_FadingTime = 2000000000;
					INV8.INV_Opacity0 = 0.0;
					INV8.INV_Opacity1 = 0.0;

					break;




			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part8Timing

