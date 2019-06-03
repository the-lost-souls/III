
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
#include "w:\general\txt.h"

#include "w:\divlibs\ims\binfstd.h"
#include "w:\divlibs\ims\ims.h"
#include "w:\divlibs\ims\itplay.h"

#include "x:\iii\part8\8.h"

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

//---- Datavariables for Part7...
int    	       *Objects07_1 	= NULL;
int    	       *Objects07_2 	= NULL;
int    	       *Objects07_3 	= NULL;
int    	       *Objects07_4 	= NULL;
int    	       *Objects07_5 	= NULL;
int    	       *Objects07_6 	= NULL;
int    	       *Lights07_1 	= NULL;
int    	       *Lights07_2 	= NULL;
int    	       *Lights07_3 	= NULL;
int    	       *Lights07_4 	= NULL;
int    	       *Lights07_5 	= NULL;
int    	       *Lights07_6 	= NULL;
SCN_Header     *SCN07_1 	= NULL;
SCN_Header     *SCN07_2 	= NULL;
SCN_Header     *SCN07_3 	= NULL;
SCN_Header     *SCN07_4 	= NULL;
SCN_Header     *SCN07_5 	= NULL;

void Part7Timing(void);
void Part7MusicSync(void);
void Rotate2D(float X0, float Y0, float Angle, float &X1, float &Y1);
void MapCoordPlugin(void);

BLN_Data Blend07;
INV_Data INV7;


NullObject SyncNull07;
TIM_Data SyncData07 = {
				0, 0,
				9, 0,
				 605440,	//0: fade scene 1
				 670976,	//1: scene 2
				1341952, 	//2: scene 3
				2012928,	//3: scene 4
				2683904,	//4: scene 5
				3522624,	//5: invert scene 5
				3606496,
				3648432,	//6: flash1
				3690368,
				0,
				0,

			};

typedef struct
{
	float	*MapCoords;
	float	Angle;
	float	UCenter1, VCenter1, UCenter2, VCenter2;
	int	StartTime;
}MapCoordPluginData;

MapCoordPluginData MapCoordData;

uint32 MirrorMapWidth = 512;
uint32 MirrorMapHeigth = 512;



int InitPart7(void)
{
	int C = 0;

//---- Setting up blending...
	Blend07.BLN_Type = 16;
	for ( C = 0; C < 16; C++) Blend07.BLN_Plugins[C] = -1;

	Blend07.BLN_R = 0.0;
	Blend07.BLN_G = 0.0;
	Blend07.BLN_B = 0.0;

	Blend07.BLN_Opacity0 = 0.0;
	Blend07.BLN_Opacity1 = 0.0;

	Blend07.BLN_StartFlag = 1;
	Blend07.BLN_StartTime = 0;
	Blend07.BLN_FadingTime = 65536;


//---- svada...
	INV7.INV_Type = 18;
	for ( C = 0; C < 16; C++ ) INV7.INV_Plugins[C] = -1;

	INV7.INV_RMul = 1.0;
	INV7.INV_GMul = 1.0;
	INV7.INV_BMul = 1.0;
	INV7.INV_StartFlag = 0;
	INV7.INV_FadingTime = 2000000000;
	INV7.INV_Opacity0 = 0.0;
	INV7.INV_Opacity1 = 0.0;


//********************************* FIRST SCENE *************************

//---- Loading scene...
	SCN07_1 = SCN_Load("x:\\demo\\room\\scenes\\l02.scn");
	if ( SCN07_1 == NULL )
	{
		SYS_Error(1, "InitPart7: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects07_1 = new int[12 + SCN07_1->NrObjects];	//8 without fog
	if ( !Objects07_1 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07_1[0] = (int)&Timer;
	Objects07_1[1] = (int)&SRT_OReset;
	Objects07_1[2] = (int)SCN07_1;
	Objects07_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07_1->NrObjects; C++ )
		Objects07_1[C + 4] = (int)SCN07_1->Objects[C].VEC;

	Objects07_1[C + 4] = (int)&SRT_OSort;
	Objects07_1[C + 5] = (int)&SRT_ORun;
	Objects07_1[C + 6] = (int)&Blend07;
	Objects07_1[C + 7] = (int)0;
	Objects07_1[C + 8] = (int)&Canvas;
	Objects07_1[C + 9] = (int)&KEY_OPressed;
	Objects07_1[C +10] = (int)&SyncNull07;
	Objects07_1[C +11] = -1;

//---- Creating lightlist...
	Lights07_1 = new int[SCN07_1->NrLights + 1];
	if ( !Lights07_1 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07_1->NrLights; C++ )
		Lights07_1[C] = (int)SCN07_1->Lights[C].LGT;

	Lights07_1[C] = -1;

	SCN07_1->Envelopes[0].Value = &SCN07_1->Objects[51].VEC->H_Materials[1].M_Transparency;

//------------------------ SETTING UP TWIRLS... -----------------------------

	VEC_Header *VEC = SCN07_1->Objects[16].VEC;

//---- Mapcoord-plugin-stuff...
	MapCoordData.MapCoords = new float[VEC->H_NrVertices*2];
	if ( MapCoordData.MapCoords == 0 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for twirl-plugin.\n");
		return 0;
	}//if

	MirrorMapWidth = VEC->H_Materials[1].M_MapWidth;
	MirrorMapHeigth = VEC->H_Materials[1].M_MapHeigth;


	VEC_Face    *FaceP 	= VEC->H_Faces;
	VEC_Vertice *VertP 	= NULL;

	for ( C = 0; C < VEC->H_NrFaces; C++ )
	{
		VertP = FaceP[C].F_V0Offs;
		VertP->V_U = FaceP[C].F_U0;
		VertP->V_V = FaceP[C].F_V0;

		VertP = FaceP[C].F_V1Offs;
		VertP->V_U = FaceP[C].F_U1;
		VertP->V_V = FaceP[C].F_V1;

		VertP = FaceP[C].F_V2Offs;
		VertP->V_U = FaceP[C].F_U2;
		VertP->V_V = FaceP[C].F_V2;
	}//for

	VertP = VEC->H_Vertices;
	for ( C = 0; C < VEC->H_NrVertices; C++ )
	{
		float U = VertP[C].V_U/MirrorMapWidth;
		float V = VertP[C].V_V/MirrorMapHeigth;
		U -= 0.5;
		V -= 0.5;

		U = U*0.707106781187;
		V = V*0.707106781187;

		MapCoordData.MapCoords[C*2 + 0] = U;
		MapCoordData.MapCoords[C*2 + 1] = V;
	}//for

	MapCoordData.Angle = 0;
	MapCoordData.UCenter1 = 0.1;
	MapCoordData.VCenter1 = 0.0;
	MapCoordData.UCenter2 = -0.2;
	MapCoordData.VCenter2 = -0.3;
	MapCoordData.StartTime = 0;

	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_1->Objects[39].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_1->Objects[40].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_1->Objects[41].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;


//********************************* 2. SCENE *************************

//---- Loading scene...
	SCN07_2 = SCN_Load("x:\\demo\\room\\scenes\\l03.scn");
	if ( SCN07_2 == NULL )
	{
		SYS_Error(1, "InitPart7: Error loading scenes\\hall01.scn.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects07_2 = new int[12 + SCN07_2->NrObjects];	//8 without fog
	if ( !Objects07_2 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07_2[0] = (int)&Timer;
	Objects07_2[1] = (int)&SRT_OReset;
	Objects07_2[2] = (int)SCN07_2;
	Objects07_2[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07_2->NrObjects; C++ )
		Objects07_2[C + 4] = (int)SCN07_2->Objects[C].VEC;

	Objects07_2[C + 4] = (int)&SRT_OSort;
	Objects07_2[C + 5] = (int)&SRT_ORun;
	Objects07_2[C + 6] = (int)&Blend07;
	Objects07_2[C + 7] = (int)0;
	Objects07_2[C + 8] = (int)&Canvas;
	Objects07_2[C + 9] = (int)&KEY_OPressed;
	Objects07_2[C +10] = (int)&SyncNull07;
	Objects07_2[C +11] = -1;

//---- Creating lightlist...
	Lights07_2 = new int[SCN07_2->NrLights + 1];
	if ( !Lights07_2 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07_2->NrLights; C++ )
		Lights07_2[C] = (int)SCN07_2->Lights[C].LGT;

	Lights07_2[C] = -1;

	//Twirls for second scene...
	VEC = SCN07_2->Objects[16].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_2->Objects[39].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_2->Objects[40].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_2->Objects[41].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;


//********************************* 3. SCENE *************************

//---- Loading scene...
	SCN07_3 = SCN_Load("x:\\demo\\room\\scenes\\l04.scn", SCN_FILE_FlagDefault, SCN07_2);
	if ( SCN07_3 == NULL )
	{
		SYS_Error(1, "InitPart7: Error loading scenes\\hall01.scn.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects07_3 = new int[12 + SCN07_3->NrObjects];	//8 without fog
	if ( !Objects07_3 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07_3[0] = (int)&Timer;
	Objects07_3[1] = (int)&SRT_OReset;
	Objects07_3[2] = (int)SCN07_3;
	Objects07_3[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07_3->NrObjects; C++ )
		Objects07_3[C + 4] = (int)SCN07_3->Objects[C].VEC;

	Objects07_3[C + 4] = (int)&SRT_OSort;
	Objects07_3[C + 5] = (int)&SRT_ORun;
	Objects07_3[C + 6] = (int)&Blend07;
	Objects07_3[C + 7] = (int)0;
	Objects07_3[C + 8] = (int)&Canvas;
	Objects07_3[C + 9] = (int)&KEY_OPressed;
	Objects07_3[C +10] = (int)&SyncNull07;
	Objects07_3[C +11] = -1;

//---- Creating lightlist...
	Lights07_3 = new int[SCN07_3->NrLights + 1];
	if ( !Lights07_3 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07_3->NrLights; C++ )
		Lights07_3[C] = (int)SCN07_3->Lights[C].LGT;

	Lights07_3[C] = -1;

/*	//Twirls for second scene...
	VEC = SCN07_3->Objects[16].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_3->Objects[39].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_3->Objects[40].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_3->Objects[41].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;*/


//********************************* 4. SCENE *************************

//---- Loading scene...
	SCN07_4 = SCN_Load("x:\\demo\\room\\scenes\\l05.scn", SCN_FILE_FlagDefault, SCN07_2);
	if ( SCN07_4 == NULL )
	{
		SYS_Error(1, "InitPart7: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects07_4 = new int[12 + SCN07_4->NrObjects];	//8 without fog
	if ( !Objects07_4 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07_4[0] = (int)&Timer;
	Objects07_4[1] = (int)&SRT_OReset;
	Objects07_4[2] = (int)SCN07_4;
	Objects07_4[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07_4->NrObjects; C++ )
		Objects07_4[C + 4] = (int)SCN07_4->Objects[C].VEC;

	Objects07_4[C + 4] = (int)&SRT_OSort;
	Objects07_4[C + 5] = (int)&SRT_ORun;
	Objects07_4[C + 6] = (int)&Blend07;
	Objects07_4[C + 7] = (int)0;
	Objects07_4[C + 8] = (int)&Canvas;
	Objects07_4[C + 9] = (int)&KEY_OPressed;
	Objects07_4[C +10] = (int)&SyncNull07;
	Objects07_4[C +11] = -1;

//---- Creating lightlist...
	Lights07_4 = new int[SCN07_4->NrLights + 1];
	if ( !Lights07_4 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07_4->NrLights; C++ )
		Lights07_4[C] = (int)SCN07_4->Lights[C].LGT;

	Lights07_4[C] = -1;

/*	//Twirls for second scene...
	VEC = SCN07_4->Objects[16].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_4->Objects[39].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_4->Objects[40].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_4->Objects[41].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;*/





//********************************* 5. SCENE *************************

//---- Loading scene...
	SCN07_5 = SCN_Load("x:\\demo\\room\\scenes\\l06.scn");
	if ( SCN07_5 == NULL )
	{
		SYS_Error(1, "InitPart7: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects07_5 = new int[12 + SCN07_5->NrObjects];	//8 without fog
	if ( !Objects07_5 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07_5[0] = (int)&Timer;
	Objects07_5[1] = (int)&SRT_OReset;
	Objects07_5[2] = (int)SCN07_5;
	Objects07_5[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07_5->NrObjects; C++ )
		Objects07_5[C + 4] = (int)SCN07_5->Objects[C].VEC;

	Objects07_5[C + 4] = (int)&SRT_OSort;
	Objects07_5[C + 5] = (int)&SRT_ORun;
	Objects07_5[C + 6] = (int)&Blend07;
	Objects07_5[C + 7] = (int)0;
	Objects07_5[C + 8] = (int)&Canvas;
	Objects07_5[C + 9] = (int)&KEY_OPressed;
	Objects07_5[C +10] = (int)&SyncNull07;
	Objects07_5[C +11] = -1;

//---- Creating lightlist...
	Lights07_5 = new int[SCN07_5->NrLights + 1];
	if ( !Lights07_5 )
	{
		SYS_Error(1, "InitPart7: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07_5->NrLights; C++ )
		Lights07_5[C] = (int)SCN07_5->Lights[C].LGT;

	Lights07_5[C] = -1;

	SCN07_5->Envelopes[0].Value = &SCN07_5->Objects[51].VEC->H_Materials[1].M_Transparency;


	//Twirls for second scene...
	VEC = SCN07_5->Objects[16].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_5->Objects[39].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_5->Objects[40].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;

	VEC = SCN07_5->Objects[41].VEC;
	VEC->H_Plugins[0] = (int)&MapCoordPlugin;
	VEC->H_Plugins[1] = (int)&MapCoordData;
	VEC->H_Plugins[2] = -1;




//---- Setting up sync...
	SyncNull07.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull07.Plugins[C] = -1;
	SyncNull07.Plugins[0] = (int)&Part7Timing;
	SyncNull07.Plugins[1] = (int)&SyncData07;


	return 1;
}//InitPart7


void UnloadPart7(void)
{
	delete [] Objects07_1;
	delete [] Lights07_1;
	delete SCN07_1;

	delete [] Objects07_2;
	delete [] Lights07_2;
	delete SCN07_2;

	delete [] Objects07_3;
	delete [] Lights07_3;
	delete SCN07_3;

	delete [] Objects07_4;
	delete [] Lights07_4;
	delete SCN07_4;

	delete [] Objects07_5;
	delete [] Lights07_5;
	delete SCN07_5;

}//UnloadPart7



void Part7Timing(void)
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
				case 0: Blend07.BLN_Opacity0 = 0.0;
					Blend07.BLN_Opacity1 = 1.0;
					Blend07.BLN_StartTime = 605440 + Timer->StartTime;
					break;

				case 1: SYS_OListOffs = Objects07_2;
					SYS_LListOffs = Lights07_2;
					SCN07_2->StartTime = 670976 + Timer->StartTime;

					Blend07.BLN_Opacity0 = 1.0;
					Blend07.BLN_Opacity1 = 0.0;
					Blend07.BLN_StartTime = 670976 + Timer->StartTime;
					break;

				case 2: SYS_OListOffs = Objects07_3;
					SYS_LListOffs = Lights07_3;
					SCN07_3->StartTime = 1341952 + Timer->StartTime;

					Blend07.BLN_Opacity0 = 1.0;
					Blend07.BLN_Opacity1 = 0.0;
					Blend07.BLN_StartTime = 1341952 + Timer->StartTime;

					break;

				case 3: SYS_OListOffs = Objects07_4;
					SYS_LListOffs = Lights07_4;
					SCN07_4->StartTime = 2012928 + Timer->StartTime;

					Blend07.BLN_Opacity0 = 1.0;
					Blend07.BLN_Opacity1 = 0.0;
					Blend07.BLN_StartTime = 2012928 + Timer->StartTime;

					break;

				case 4: SYS_OListOffs = Objects07_5;
					SYS_LListOffs = Lights07_5;
					SCN07_5->StartTime = 2683904 + Timer->StartTime;

					Blend07.BLN_Opacity0 = 1.0;
					Blend07.BLN_Opacity1 = 0.0;
					Blend07.BLN_StartTime = 2683904 + Timer->StartTime;

					break;

				case 5: INV7.INV_StartTime = 3522624 + Timer->StartTime;
					INV7.INV_StartFlag = 1;
					INV7.INV_FadingTime = 41936;

					INV7.INV_Opacity0 = 0.0;
					INV7.INV_Opacity1 = 1.0;
					INV7.INV_RMul = 0.7;
					INV7.INV_GMul = 0.7;
					INV7.INV_BMul = 1.0;
					break;

				case 6: Blend07.BLN_R = 1.0;
					Blend07.BLN_G = 1.0;
					Blend07.BLN_B = 1.0;
					Blend07.BLN_Opacity0 = 1;
					Blend07.BLN_Opacity1 = 0;

					Blend07.BLN_StartTime = 3606496 + Timer->StartTime;
					Blend07.BLN_FadingTime = 16384;
					break;

				case 7: Blend07.BLN_StartTime = 3648432 + Timer->StartTime;
					break;

				case 8: SCN08_1->StartTime = 3690638 + Timer->StartTime;
					SyncData08.StartTime = SCN08_1->StartTime;

					SYS_OListOffs = Objects08_1;
					SYS_LListOffs = Lights08_1;

					SYS_FPClipTop = SYS_YRes*0.15;
					SYS_FPClipBottom = SYS_YRes*0.85;

					SyncData07.CurrentKey = -1;

				//---- asdfjaos›idjfawoi›fj

					Blend07.BLN_R = 0.0;
					Blend07.BLN_G = 0.0;
					Blend07.BLN_B = 0.0;

					Blend07.BLN_Opacity0 = 0.0;
					Blend07.BLN_Opacity1 = 0.0;

					Blend07.BLN_StartFlag = 1;
					Blend07.BLN_StartTime = 0;
					Blend07.BLN_FadingTime = 65536;


				//---- svada...

					INV7.INV_RMul = 1.0;
					INV7.INV_GMul = 1.0;
					INV7.INV_BMul = 1.0;
					INV7.INV_StartFlag = 0;
					INV7.INV_FadingTime = 2000000000;
					INV7.INV_Opacity0 = 0.0;
					INV7.INV_Opacity1 = 0.0;


					break;




			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part7Timing





//********************* ADDITIONAL STUFF FOR 2D-TWIRLTHINGY *****************
void Rotate2D(float X0, float Y0, float Angle, float &X1, float &Y1)
{
	float XOut, YOut;
	__asm{
		fld	Angle
		fcos
		fld	Angle
		fsin

		fld	X0
		fmul    st, st(2)
		fld	Y0
		fmul	st, st(2)
		fsubp	st(1), st
		fstp	XOut

		fmul    X0
		fxch	st(1)
		fmul	Y0
		faddp	st(1), st
		fstp	YOut

	}//asm
	X1 = XOut;
	Y1 = YOut;

//	X1 = X0*cos(Angle) - Y0*sin(Angle);
//	Y1 = Y0*cos(Angle) + X0*sin(Angle);
}//Rotate2D



void MapCoordPlugin(void)
{
	int C1, C2;
	VEC_Header *VEC = (VEC_Header*)SYS_PCaller;
	VEC_Vertice *Vert;
	VEC_Face *FaceP;
	MapCoordPluginData *Data = (MapCoordPluginData*)SYS_POffs;


//---- Fixing mappingcoords...
	Data->Angle = (((float)(SYS_TimerTick - Data->StartTime))/SYS_TicksPerSec)*0.30;


	Vert = VEC->H_Vertices;
	C2 = VEC->H_NrVertices;
	for ( C1 = 0; C1 < C2; C1++)
	{
		float U, V, Distance, UCenter, VCenter;

		U = Data->MapCoords[C1*2 + 0];
		V = Data->MapCoords[C1*2 + 1];

		U = U + sin(U*10)/20;
		V = V + sin(V*10)/20;

		Rotate2D(U, V, -Data->Angle*0.80, U, V);

	//---- Swirl 1
		Rotate2D(Data->UCenter1, Data->VCenter1, -Data->Angle, UCenter, VCenter);
		U -= UCenter;
		V -= VCenter;
		Distance = sqrt((U*U) + (V*V)); //max = 0.5

		float Angle = (0.40 - Distance);
		if ( Angle < 0 ) Angle = 0;

		Angle = Angle*(2*Pi/5);


		Rotate2D(	U,
				V,
				Angle,
				U,
				V);

		U += UCenter;
		V += VCenter;

	//---- Swirl 2
		Rotate2D(Data->UCenter2, Data->VCenter2, 0.65*Data->Angle, UCenter, VCenter);
		U -= UCenter;
		V -= VCenter;
		Distance = sqrt((U*U) + (V*V)); //max = 0.5

		Angle = (0.30 - Distance);
		if ( Angle < 0 ) Angle = 0;

		//Angle = Angle*Pi*6*sin(Data->Angle*0.134);
		Angle = Angle*(2*Pi/6);


		Rotate2D(	U,
				V,
				Angle,
				U,
				V);

		U += UCenter;
		V += VCenter;


	//---- Transforming...

		if ( U < -0.5 ) U = -0.5;
		if ( U > 0.5 ) U = -0.5;
		if ( V < -0.5 ) V = -0.5;
		if ( V > 0.5 ) V = -0.5;

		U = (U + 0.5)*MirrorMapWidth;
		V = (V + 0.5)*MirrorMapHeigth;
		Vert[C1].V_U = U;
		Vert[C1].V_V = V;
	}//for

//---- Copying mapping coords...
	FaceP = VEC->H_Faces;
	C2 = VEC->H_NrFaces;
	for ( C1 = 0; C1 < C2; C1++ )
	{
		Vert = FaceP[C1].F_V0Offs;
		FaceP[C1].F_U0 = Vert->V_U;
		FaceP[C1].F_V0 = Vert->V_V;
		Vert = FaceP[C1].F_V1Offs;
		FaceP[C1].F_U1 = Vert->V_U;
		FaceP[C1].F_V1 = Vert->V_V;
		Vert = FaceP[C1].F_V2Offs;
		FaceP[C1].F_U2 = Vert->V_U;
		FaceP[C1].F_V2 = Vert->V_V;
	}//for

}//DrawPlugin

