
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
#include "w:\render\render.h"

#include "x:\iii\part3\2.h"

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

//---- Datavariables for part1...
int    	       *Objects01 	= NULL;
int	       *Objects02	= NULL;
int	       *Objects03	= NULL;
int    	       *Lights01 	= NULL;
int	       *Lights02	= NULL;
int	       *Lights03	= NULL;
SCN_Header     *SCN01 		= NULL;
SCN_Header     *SCN02 		= NULL;
SCN_Header     *SCN03 		= NULL;

BLN_Data Blend1;

BLN3_Data AddBlend1;
BLN3_Data AddBlend2;


void Part1Timing(void);
void Part1MusicSync(void);
NullObject SyncNull;
TIM_Data SyncData01 = {
				0, 0,
				9, 0,
				1174208,
				1341952,	//galaxe starter
				1473024,	//2 sec (p†)
				1505792,	//2.5 sec (av)
				1571328,        //3.5 sec (p†)
				1669632,	//5 sec (av)
				1866240,	//8 sec (p†)
				1931776,	//9 sec (av)
				3354880,	//part2
			};

SCN_Object *Pil0 = NULL;
SCN_Object *Pil1 = NULL;
SCN_Object *Pil2 = NULL;
SCN_Object *Pil3 = NULL;




const float FartA = 200;
const float FartB = 600;
int FramesKjoert = 0;





VEC_Material *M01 = NULL;
VEC_Material *M02 = NULL;
VEC_Material *M03 = NULL;
VEC_Material *M04 = NULL;
VEC_Material *M05 = NULL;
VEC_Material *M06 = NULL;
VEC_Material *M07 = NULL;
VEC_Material *M08 = NULL;
VEC_Material *M09 = NULL;
VEC_Material *M10 = NULL;
VEC_Material *M11 = NULL;
VEC_Material *M12 = NULL;



int InitPart1(void)
{
	int C = 0;


//********************************* FIRST SCENE *************************

//---- Setting up additive blending...
	Blend1.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend1.BLN_Plugins[C] = -1;

	Blend1.BLN_R = 0.0;
	Blend1.BLN_G = 0.0;
	Blend1.BLN_B = 0.0;

	Blend1.BLN_Opacity0 = 1.0;
	Blend1.BLN_Opacity1 = 0.0;

	Blend1.BLN_StartFlag = 1;
	Blend1.BLN_StartTime = 0;
	Blend1.BLN_FadingTime = 65536*200;

//---- Setting up sync...
	SyncNull.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull.Plugins[C] = -1;
	SyncNull.Plugins[0] = (int)&Part1Timing;
	SyncNull.Plugins[1] = (int)&SyncData01;
	SyncNull.Plugins[2] = (int)&Part1MusicSync;


//---- Loading scene...

	SCN01 = SCN_Load("x:\\scene\\t2.scn");
	if ( SCN01 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects01 = new int[12 + SCN01->NrObjects];	//8 without fog
	if ( !Objects01 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects01[0] = (int)&Timer;
	Objects01[1] = (int)&SRT_OReset;
	Objects01[2] = (int)SCN01;
	Objects01[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN01->NrObjects; C++ )
		Objects01[C + 4] = (int)SCN01->Objects[C].VEC;

	Objects01[C + 4] = (int)&SRT_OSort;
	Objects01[C + 5] = (int)&SRT_ORun;
	Objects01[C + 6] = (int)&MBL;
	Objects01[C + 7] = (int)0;//&Blend1;
	Objects01[C + 8] = (int)&Canvas;
	Objects01[C + 9] = (int)&KEY_OPressed;
	Objects01[C +10] = (int)&SyncNull;
	Objects01[C +11] = -1;

//---- Creating lightlist...
	Lights01 = new int[SCN01->NrLights + 1];
	if ( !Lights01 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN01->NrLights; C++ )
		Lights01[C] = (int)SCN01->Lights[C].LGT;

	Lights01[C] = -1;


//---- Setting up transparency-fading for plates...
	SCN_Envelope *TmpEnvelope = &SCN01->Envelopes[0];
	VEC_Header *VECTmp = SCN01->Objects[38].VEC;
	TmpEnvelope->Value = &VECTmp->H_Materials[1].M_Transparency;

	TmpEnvelope = &SCN01->Envelopes[1];
	VECTmp = SCN01->Objects[39].VEC;
	TmpEnvelope->Value = &VECTmp->H_Materials[1].M_Transparency;



//**************************** SECOND SCENE ******************************

//---- Loading scene...

	SCN02 = SCN_Load("x:\\scene\\bilde.scn");
	if ( SCN02 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scenes\\bilde.scn.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects02 = new int[12 + SCN02->NrObjects];	//8 without fog
	if ( !Objects02 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects02[0] = (int)&Timer;
	Objects02[1] = (int)&SRT_OReset;
	Objects02[2] = (int)SCN02;
	Objects02[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN02->NrObjects; C++ )
		Objects02[C + 4] = (int)SCN02->Objects[C].VEC;

	Objects02[C + 4] = (int)&SRT_OSort;
	Objects02[C + 5] = (int)&SRT_ORun;
	Objects02[C + 6] = (int)&MBL;
	Objects02[C + 7] = (int)0;//&Blend1;
	Objects02[C + 8] = (int)&Canvas;
	Objects02[C + 9] = (int)&KEY_OPressed;
	Objects02[C +10] = (int)&SyncNull;
	Objects02[C +11] = -1;

//---- Creating lightlist...
	Lights02 = new int[SCN02->NrLights + 1];
	if ( !Lights02 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN02->NrLights; C++ )
		Lights02[C] = (int)SCN02->Lights[C].LGT;

	Lights02[C] = -1;



//**************************** THIRD SCENE ******************************

//---- Loading scene...

	SCN03 = SCN_Load("x:\\scene\\g3.scn");
	if ( SCN03 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects03 = new int[13 + SCN03->NrObjects];	//8 without fog
	if ( !Objects03 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects03[0] = (int)&Timer;
	Objects03[1] = (int)&SRT_OReset;
	Objects03[2] = (int)SCN03;
	Objects03[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN03->NrObjects; C++ )
		Objects03[C + 4] = (int)SCN03->Objects[C].VEC;

	Objects03[C + 4] = (int)&SRT_OSort;
	Objects03[C + 5] = (int)&SRT_ORun;
	Objects03[C + 6] = (int)&MBL;
	Objects03[C + 7] = (int)&AddBlend1;
	Objects03[C + 8] = (int)&AddBlend2;
	Objects03[C + 9] = (int)&Canvas;
	Objects03[C +10] = (int)&KEY_OPressed;
	Objects03[C +11] = (int)&SyncNull;
	Objects03[C +12] = -1;


//---- Creating lightlist...
	Lights03 = new int[SCN03->NrLights + 1];
	if ( !Lights03 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN03->NrLights; C++ )
		Lights03[C] = (int)SCN03->Lights[C].LGT;

	Lights03[C] = -1;


//---- svada...
	AddBlend1.BLN3_StartTime = 2683904;
	AddBlend1.BLN3_FadingTime = 65536*0.7;

	AddBlend1.BLN3_Opacity0 = 1.0;
	AddBlend1.BLN3_Opacity1 = 0.0;
	AddBlend1.BLN3_BlendR = 155;
	AddBlend1.BLN3_BlendG = 155;
	AddBlend1.BLN3_BlendB = 155;

	AddBlend2.BLN3_StartTime = 3154880;
	AddBlend2.BLN3_FadingTime = 200000;

	AddBlend2.BLN3_Opacity0 = 0.0;
	AddBlend2.BLN3_Opacity1 = 1.0;
	AddBlend2.BLN3_BlendR = 255;
	AddBlend2.BLN3_BlendG = 255;
	AddBlend2.BLN3_BlendB = 255;

	Pil0 = &SCN03->Objects[29];
	Pil1 = &SCN03->Objects[30];
	Pil2 = &SCN03->Objects[31];
	Pil3 = &SCN03->Objects[32];



	return 1;
}//InitPart1


void UnloadPart1(void)
{
	delete [] Objects01;
	delete [] Lights01;
	delete SCN01;

	delete [] Objects02;
	delete [] Lights02;
	delete SCN02;

	delete [] Objects03;
	delete [] Lights03;
	delete SCN03;
}//UnloadPart1



void Part1Timing(void)
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
				case 0: SYS_OListOffs = Objects02;
					SYS_LListOffs = Lights02;
					SCN02->StartTime = 1174208 + Timer->StartTime;
					break;

				case 1: SYS_OListOffs = Objects03;
					SYS_LListOffs = Lights03;
					SCN03->StartTime = 1341952 + Timer->StartTime;

					AddBlend1.BLN3_StartTime = 2683904 + Timer->StartTime;
					AddBlend2.BLN3_StartTime = 3154880 + Timer->StartTime;

					break;


				case 2: SCN03->FramesPerSecond = 600; //p†
					SCN03->StartTime = 1429311 + Timer->StartTime;
					break;

				case 3: SCN03->FramesPerSecond = 200; //av
					SCN03->StartTime = 1276416 + Timer->StartTime;
					break;

				case 4: SCN03->FramesPerSecond = 600; //p†
					SCN03->StartTime = 1473024 + Timer->StartTime;
					break;

				case 5: SCN03->FramesPerSecond = 200; //av
					SCN03->StartTime = 1669632 - 589824 + Timer->StartTime;
					break;

				case 6: SCN03->FramesPerSecond = 600; //p†
					SCN03->StartTime = 1866240 - 262144 + Timer->StartTime;
					break;

				case 7: SCN03->FramesPerSecond = 200; //av
					SCN03->StartTime = 1931776 - 983040 + Timer->StartTime;
					break;

				case 8: SYS_OListOffs = Objects02_1;
					SYS_LListOffs = Lights02_1;

					SCN02_1->StartTime = 3354880 + Timer->StartTime;
					SyncData02.StartTime = SCN02_1->StartTime;

					SYS_FPClipTop = SYS_YRes*0.15;
					SYS_FPClipBottom = SYS_YRes*0.85;

					T09_Init();

					FLR_Priority = 239;
					V08_Priority = 238;

					SyncData01.CurrentKey = -1;

				//---- asdfjasdofij
					Blend1.BLN_R = 0.0;
					Blend1.BLN_G = 0.0;
					Blend1.BLN_B = 0.0;

					Blend1.BLN_Opacity0 = 1.0;
					Blend1.BLN_Opacity1 = 0.0;

					Blend1.BLN_StartFlag = 1;
					Blend1.BLN_StartTime = 0;
					Blend1.BLN_FadingTime = 65536*200;

					AddBlend1.BLN3_StartTime = 2683904;
					AddBlend1.BLN3_FadingTime = 65536*0.7;

					AddBlend1.BLN3_Opacity0 = 1.0;
					AddBlend1.BLN3_Opacity1 = 0.0;
					AddBlend1.BLN3_BlendR = 155;
					AddBlend1.BLN3_BlendG = 155;
					AddBlend1.BLN3_BlendB = 155;

					AddBlend2.BLN3_StartTime = 3154880;
					AddBlend2.BLN3_FadingTime = 200000;

					AddBlend2.BLN3_Opacity0 = 0.0;
					AddBlend2.BLN3_Opacity1 = 1.0;
					AddBlend2.BLN3_BlendR = 255;
					AddBlend2.BLN3_BlendG = 255;
					AddBlend2.BLN3_BlendB = 255;

					break;


			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part1Timing


void Part1MusicSync(void)
{
  	int time = 0;

//---- Checking for flash...
  	int val = itplayer->getsync(0, time);

	if ( (time != -1) && (val == 00) )
	{

		AddBlend1.BLN3_StartTime = SYS_TimerTick - time;

	}//if

//---- asldkfj
  	val = itplayer->getsync(1, time);
	if ( (time != -1) && (val == 01) )
	{
		float seconds = (float)(SYS_TimerTick - time - SCN03->StartTime )/SYS_TicksPerSec;
		int F = seconds*SCN03->FramesPerSecond;

		Pil0->Keyframes[1].Frame = F;
		Pil0->Keyframes[2].Frame = F + 200;

		Pil1->Keyframes[1].Frame = F;
		Pil1->Keyframes[2].Frame = F + 200;

		Pil2->Keyframes[1].Frame = F;
		Pil2->Keyframes[2].Frame = F + 200;

		Pil3->Keyframes[1].Frame = F;
		Pil3->Keyframes[2].Frame = F + 200;

	}//if*/

}//void


