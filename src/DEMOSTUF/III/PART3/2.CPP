
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
#include "w:\vectors\vectors.h"
#include "w:\general\txt.h"
#include "w:\render\render.h"

#include "w:\divlibs\ims\binfstd.h"
#include "w:\divlibs\ims\ims.h"
#include "w:\divlibs\ims\itplay.h"

#include "x:\iii\part5\5.h"

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>
#include <math.h>

//---- External global stuff...
extern CNV_Data Canvas;
extern NullObject Timer;
extern MBL_Data MBL;
extern itplayerclass *itplayer;

//---- Datavariables for Part2...
int    	       *Objects02_1 	= NULL;
int	       *Objects02_2	= NULL;
int	       *Objects02_3	= NULL;
int	       *Objects02_4	= NULL;
int    	       *Lights02_1 	= NULL;
int	       *Lights02_2	= NULL;
int	       *Lights02_3	= NULL;
int	       *Lights02_4	= NULL;
SCN_Header     *SCN02_1 	= NULL;
SCN_Header     *SCN02_2 		= NULL;
SCN_Header     *SCN02_3 		= NULL;
SCN_Header     *SCN02_4 		= NULL;

BLN_Data Blend02;

BLN3_Data AddBlend02;


void Part2Timing(void);
void Part2MusicSync(void);
void WaterFallThingyPlugin(void);

NullObject SyncNull02;
TIM_Data SyncData02 = {
				0, 0,
				5, 0,
				 640000,	//fade...
				 670976,	//scene 2
				1006464,	//scene 3
				1341962,	//scene 4
				2012938,
				0,
				0,
				0,
				0,
				0,
				0,
				0,

			};


void *Tmp1 = NULL;
void *Tmp2 = NULL;
void *Tmp3 = NULL;
void *Tmp4 = NULL;
void *Tmp5 = NULL;

int InitPart2(void)
{
	int C = 0;


//********************************* FIRST SCENE *************************
//---- Setting up blending...
	Blend02.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend02.BLN_Plugins[C] = -1;

	Blend02.BLN_R = 0.0;
	Blend02.BLN_G = 0.0;
	Blend02.BLN_B = 0.0;

	Blend02.BLN_Opacity0 = 0.0;
	Blend02.BLN_Opacity1 = 0.0;

	Blend02.BLN_StartFlag = 1;
	Blend02.BLN_StartTime = 0;
	Blend02.BLN_FadingTime = 65536*200;



//---- Loading scene...

	SCN02_1 = SCN_Load("x:\\demo\\watrfall\\scenes\\r1.scn");
	if ( SCN02_1 == NULL )
	{
		SYS_Error(1, "InitPart2: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects02_1 = new int[12 + SCN02_1->NrObjects];	//8 without fog
	if ( !Objects02_1 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects02_1[0] = (int)&Timer;
	Objects02_1[1] = (int)&SRT_OReset;
	Objects02_1[2] = (int)SCN02_1;
	Objects02_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN02_1->NrObjects; C++ )
		Objects02_1[C + 4] = (int)SCN02_1->Objects[C].VEC;

	Objects02_1[C + 4] = (int)&SRT_OSort;
	Objects02_1[C + 5] = (int)&SRT_ORun;
	Objects02_1[C + 6] = (int)&MBL;
	Objects02_1[C + 7] = (int)0;
	Objects02_1[C + 8] = (int)&Canvas;
	Objects02_1[C + 9] = (int)&KEY_OPressed;
	Objects02_1[C +10] = (int)&SyncNull02;
	Objects02_1[C +11] = -1;

//---- Creating lightlist...
	Lights02_1 = new int[SCN02_1->NrLights + 1];
	if ( !Lights02_1 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN02_1->NrLights; C++ )
		Lights02_1[C] = (int)SCN02_1->Lights[C].LGT;

	Lights02_1[C] = -1;


//---- Fixing different stuff...
	Tmp1 = SCN02_1->Objects[55].VEC->H_Materials[1].M_MapOffset;

	Tmp2 = SCN02_1->Objects[5].VEC->H_Materials[1].M_MapOffset;
	Tmp3 = SCN02_1->Objects[7].VEC->H_Materials[1].M_MapOffset;
	Tmp4 = SCN02_1->Objects[44].VEC->H_Materials[1].M_MapOffset;
	Tmp5 = SCN02_1->Objects[45].VEC->H_Materials[1].M_MapOffset;

	SCN02_1->Envelopes[0].Value = &SCN02_1->Objects[88].VEC->H_Materials[1].M_Transparency;
	SCN02_1->Envelopes[1].Value = &SCN02_1->Objects[89].VEC->H_Materials[1].M_Transparency;



//**************************** SECOND SCENE ******************************

//---- Loading scene...

	SCN02_2 = SCN_Load("x:\\demo\\watrfall\\scenes\\r2.scn");
	if ( SCN02_2 == NULL )
	{
		SYS_Error(1, "InitPart2: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects02_2 = new int[12 + SCN02_2->NrObjects];	//8 without fog
	if ( !Objects02_2 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects02_2[0] = (int)&Timer;
	Objects02_2[1] = (int)&SRT_OReset;
	Objects02_2[2] = (int)SCN02_2;
	Objects02_2[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN02_2->NrObjects; C++ )
		Objects02_2[C + 4] = (int)SCN02_2->Objects[C].VEC;

	Objects02_2[C + 4] = (int)&SRT_OSort;
	Objects02_2[C + 5] = (int)&SRT_ORun;
	Objects02_2[C + 6] = (int)&MBL;
	Objects02_2[C + 7] = (int)&Blend02;
	Objects02_2[C + 8] = (int)&Canvas;
	Objects02_2[C + 9] = (int)&KEY_OPressed;
	Objects02_2[C +10] = (int)&SyncNull02;
	Objects02_2[C +11] = -1;

//---- Creating lightlist...
	Lights02_2 = new int[SCN02_2->NrLights + 1];
	if ( !Lights02_2 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN02_2->NrLights; C++ )
		Lights02_2[C] = (int)SCN02_2->Lights[C].LGT;

	Lights02_2[C] = -1;



//**************************** THIRD SCENE ******************************

//---- Loading scene...

	SCN02_3 = SCN_Load("x:\\demo\\watrfall\\scenes\\r3.scn");
	if ( SCN02_3 == NULL )
	{
		SYS_Error(1, "InitPart2: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects02_3 = new int[13 + SCN02_3->NrObjects];	//8 without fog
	if ( !Objects02_3 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects02_3[0] = (int)&Timer;
	Objects02_3[1] = (int)&SRT_OReset;
	Objects02_3[2] = (int)SCN02_3;
	Objects02_3[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN02_3->NrObjects; C++ )
		Objects02_3[C + 4] = (int)SCN02_3->Objects[C].VEC;

	Objects02_3[C + 4] = (int)&SRT_OSort;
	Objects02_3[C + 5] = (int)&SRT_ORun;
	Objects02_3[C + 6] = (int)&MBL;
	Objects02_3[C + 7] = (int)&Blend02;
	Objects02_3[C + 8] = (int)0;
	Objects02_3[C + 9] = (int)&Canvas;
	Objects02_3[C +10] = (int)&KEY_OPressed;
	Objects02_3[C +11] = (int)&SyncNull02;
	Objects02_3[C +12] = -1;


//---- Creating lightlist...
	Lights02_3 = new int[SCN02_3->NrLights + 1];
	if ( !Lights02_3 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN02_3->NrLights; C++ )
		Lights02_3[C] = (int)SCN02_3->Lights[C].LGT;

	Lights02_3[C] = -1;


//---- r6tyuijkopl‘
	AddBlend02.BLN3_StartTime = 670976;
	AddBlend02.BLN3_FadingTime = 65536*3;

	AddBlend02.BLN3_Opacity0 = 1.0;
	AddBlend02.BLN3_Opacity1 = 0.0;
	AddBlend02.BLN3_BlendR = 155;
	AddBlend02.BLN3_BlendG = 155;
	AddBlend02.BLN3_BlendB = 155;

	SCN02_3->Envelopes[0].Value = &SCN02_3->Objects[88].VEC->H_Materials[1].M_Transparency;
	SCN02_3->Envelopes[1].Value = &SCN02_3->Objects[89].VEC->H_Materials[1].M_Transparency;



//**************************** 4th SCENE ******************************

//---- Loading scene...

	SCN02_4 = SCN_Load("x:\\demo\\watrfall\\scenes\\r4.scn");
	if ( SCN02_4 == NULL )
	{
		SYS_Error(1, "InitPart2: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects02_4 = new int[13 + SCN02_4->NrObjects];	//8 without fog
	if ( !Objects02_4 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects02_4[0] = (int)&Timer;
	Objects02_4[1] = (int)&SRT_OReset;
	Objects02_4[2] = (int)SCN02_4;
	Objects02_4[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN02_4->NrObjects; C++ )
		Objects02_4[C + 4] = (int)SCN02_4->Objects[C].VEC;

	Objects02_4[C + 4] = (int)&SRT_OSort;
	Objects02_4[C + 5] = (int)&SRT_ORun;
	Objects02_4[C + 6] = (int)&MBL;
	Objects02_4[C + 7] = (int)&AddBlend02;
	Objects02_4[C + 8] = (int)0;
	Objects02_4[C + 9] = (int)&Canvas;
	Objects02_4[C +10] = (int)&KEY_OPressed;
	Objects02_4[C +11] = (int)&SyncNull02;
	Objects02_4[C +12] = -1;


//---- Creating lightlist...
	Lights02_4 = new int[SCN02_4->NrLights + 1];
	if ( !Lights02_4 )
	{
		SYS_Error(1, "InitPart2: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN02_4->NrLights; C++ )
		Lights02_4[C] = (int)SCN02_4->Lights[C].LGT;

	Lights02_4[C] = -1;


//---- svada...
	SCN02_4->Envelopes[0].Value = &SCN02_4->Objects[88].VEC->H_Materials[1].M_Transparency;
	SCN02_4->Envelopes[1].Value = &SCN02_4->Objects[89].VEC->H_Materials[1].M_Transparency;
	SCN02_4->Envelopes[3].Value = &SCN02_4->Objects[91].VEC->H_Materials[1].M_Transparency;


//---- Setting up sync...
	SyncNull02.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull02.Plugins[C] = -1;
	SyncNull02.Plugins[0] = (int)&Part2Timing;
	SyncNull02.Plugins[1] = (int)&SyncData02;

	SyncNull02.Plugins[2] = (int)&Part2MusicSync;
	SyncNull02.Plugins[3] = 0;

	SyncNull02.Plugins[4] = (int)&WaterFallThingyPlugin;
	SyncNull02.Plugins[5] = (int)SCN02_1;

	return 1;
}//InitPart2


void UnloadPart2(void)
{
	SCN02_1->Objects[55].VEC->H_Materials[1].M_MapOffset = Tmp1;

	SCN02_1->Objects[5].VEC->H_Materials[1].M_MapOffset = Tmp2;
	SCN02_1->Objects[7].VEC->H_Materials[1].M_MapOffset = Tmp3;
	SCN02_1->Objects[44].VEC->H_Materials[1].M_MapOffset = Tmp4;
	SCN02_1->Objects[45].VEC->H_Materials[1].M_MapOffset = Tmp5;

	delete [] Objects02_1;
	delete [] Lights02_1;
	delete SCN02_1;

	delete [] Objects02_2;
	delete [] Lights02_2;
	delete SCN02_2;

	delete [] Objects02_3;
	delete [] Lights02_3;
	delete SCN02_3;

	delete [] Objects02_4;
	delete [] Lights02_4;
	delete SCN02_4;
}//UnloadPart2



void Part2Timing(void)
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
				case 0: Blend02.BLN_StartTime = 640000 + Timer->StartTime;
					Blend02.BLN_Opacity0 = 0.0;
					Blend02.BLN_Opacity1 = 1.0;
					Blend02.BLN_FadingTime = 33000;
					Blend02.BLN_StartFlag = 1;
					break;

				case 1: SYS_OListOffs = Objects02_2;
					SYS_LListOffs = Lights02_2;
					SCN02_2->StartTime = 670976 + Timer->StartTime;
					SyncNull02.Plugins[5] = (int)SCN02_2;

					Blend02.BLN_StartTime = 670976 + Timer->StartTime;
					Blend02.BLN_Opacity0 = 1.0;
					Blend02.BLN_Opacity1 = 0.0;
					Blend02.BLN_FadingTime = 60000;
					Blend02.BLN_StartFlag = 1;

					break;

				case 2: SYS_OListOffs = Objects02_3;
					SYS_LListOffs = Lights02_3;
					SCN02_3->StartTime = 1006464 + Timer->StartTime;
					SyncNull02.Plugins[5] = (int)SCN02_3;

					Blend02.BLN_StartTime = 1006464 + Timer->StartTime;
					Blend02.BLN_Opacity0 = 1.0;
					Blend02.BLN_Opacity1 = 0.0;
					Blend02.BLN_FadingTime = 60000;
					Blend02.BLN_StartFlag = 1;


					break;


				case 3: SYS_OListOffs = Objects02_4;
					SYS_LListOffs = Lights02_4;
					SCN02_4->StartTime = 1341952 + Timer->StartTime;
					SyncNull02.Plugins[5] = (int)SCN02_4;

					AddBlend02.BLN3_StartTime = 1341952 + Timer->StartTime;
					AddBlend02.BLN3_FadingTime = 65536*3;

					AddBlend02.BLN3_Opacity0 = 1.0;
					AddBlend02.BLN3_Opacity1 = 0.0;
					AddBlend02.BLN3_BlendR = 155;
					AddBlend02.BLN3_BlendG = 155;
					AddBlend02.BLN3_BlendB = 155;

					break;

				case 4: SYS_OListOffs = Objects05_1;
					SYS_LListOffs = Lights05_1;
					SCN05_1->StartTime = 2012938 + Timer->StartTime;
					SyncData05.StartTime = 2012938 + Timer->StartTime;

					Blend05.BLN_StartTime = 2012938 + Timer->StartTime;
					AddBlend5.BLN3_StartTime = 2012938 + Timer->StartTime;

					SYS_FPClipTop = 0;
					SYS_FPClipBottom = SYS_YRes - 1;

					T09_Init();

					FLR_Priority = 240;
					V08_Priority = 240;

					SyncData02.CurrentKey = -1;


				//----- o›ijasdfoijasdfoijasf
					Blend02.BLN_R = 0.0;
					Blend02.BLN_G = 0.0;
					Blend02.BLN_B = 0.0;

					Blend02.BLN_Opacity0 = 0.0;
					Blend02.BLN_Opacity1 = 0.0;

					Blend02.BLN_StartFlag = 1;
					Blend02.BLN_StartTime = 0;
					Blend02.BLN_FadingTime = 65536*200;

					AddBlend02.BLN3_StartTime = 670976;
					AddBlend02.BLN3_FadingTime = 65536*3;

					AddBlend02.BLN3_Opacity0 = 1.0;
					AddBlend02.BLN3_Opacity1 = 0.0;
					AddBlend02.BLN3_BlendR = 155;
					AddBlend02.BLN3_BlendG = 155;
					AddBlend02.BLN3_BlendB = 155;

					break;


			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part2Timing


void Part2MusicSync(void)
{
  	int time = 0;

//---- Checking for flash...
  	int val = itplayer->getsync(0, time);

	if ( (time != -1) && (val == 00) )
	{

		//AddBlend02.BLN3_StartTime = SYS_TimerTick - time;

	}//if

}//void


void WaterFallThingyPlugin(void)
{
	SCN_Header *SCN = (SCN_Header*)SYS_POffs;

//---- Calculating time...
	int CurrentTime = SYS_TimerTick;
	float Seconds = (float)(CurrentTime - SCN->StartTime)/(float)SYS_TicksPerSec;
	float InFrame = SCN->FirstFrame + Seconds*SCN->FramesPerSecond;


	float TempMatrix[12];
	FLR_Data *FLR = NULL;

//---- fading flares...
	SCN_GetObjectMatrix(InFrame, &SCN->Objects[10], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[10].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[12], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[12].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[13], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[13].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[15], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[15].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[16], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[16].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[17], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[17].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[32], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[32].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[33], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[33].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[34], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[34].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[35], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[35].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[36], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[36].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/250))*1.00;



//---- Moving watermap's offset...
	VEC_Material *M = &SCN->Objects[55].VEC->H_Materials[1];
	byte *P = (byte*)Tmp1;

	int s = 10000/TXT_ScalingFactor;
	float t = (float)(InFrame - ((int)(InFrame/s))*s)/s;

	P += ((int)(t*M->M_MapHeigth*0.48))*80*4;
	//P += (byte)Tmp1;
	M->M_MapOffset = P;


//---- Moving waterfalls' mapoffset...
	M = &SCN->Objects[5].VEC->H_Materials[1];
	P = (byte*)Tmp2;
	s = 4000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.48))*100*2/((int)(1/TXT_ScalingFactor));
	M->M_MapOffset = P;

	M = &SCN->Objects[7].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 3000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.48))*100*4/((int)(1/TXT_ScalingFactor));;
	M->M_MapOffset = P;

	M = &SCN->Objects[44].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 4000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.48))*100*4/((int)(1/TXT_ScalingFactor));;
	M->M_MapOffset = P;

	M = &SCN->Objects[45].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 3000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.48))*100*4/((int)(1/TXT_ScalingFactor));;
	M->M_MapOffset = P;


//---- fading flares...
	SCN_GetObjectMatrix(InFrame, &SCN->Objects[56], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[56].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[58], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[58].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[59], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[59].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[60], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[60].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[61], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[61].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[62], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[62].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[68], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[68].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[69], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[69].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[70], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[70].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[71], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[71].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[72], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[72].VEC->H_Plugins[1];
	FLR->RealIntensity = 1 - (-TempMatrix[7]/200);


}//WaterFallThingyPlugin



