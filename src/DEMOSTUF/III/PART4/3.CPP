
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

#include "x:\iii\part2\1.h"

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>
#include <math.h>

//---- External global stuff...
extern CNV_Data Canvas;
extern NullObject Timer;
extern itplayerclass itplayer;
extern MBL_Data MBL;

//---- Datavariables for Part3...
int    	       *Objects03_1 	= NULL;
int    	       *Lights03_1 	= NULL;
SCN_Header     *SCN03_1 	= NULL;


void Part3Timing(void);
void Part3MusicSync(void);

NullObject SyncNull03;
TIM_Data SyncData03 = {
				0, 0,
				1, 0,
				 2012928,	//fade...
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,

			};


int InitPart3(void)
{
	int C = 0;


//********************************* FIRST SCENE *************************


//---- Loading scene...

	SCN03_1 = SCN_Load("x:\\sinus\\s2.scn");
	if ( SCN03_1 == NULL )
	{
		SYS_Error(1, "InitPart3: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects03_1 = new int[12 + SCN03_1->NrObjects];	//8 without fog
	if ( !Objects03_1 )
	{
		SYS_Error(1, "InitPart3: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects03_1[0] = (int)&Timer;
	Objects03_1[1] = (int)&SRT_OReset;
	Objects03_1[2] = (int)SCN03_1;
	Objects03_1[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN03_1->NrObjects; C++ )
		Objects03_1[C + 4] = (int)SCN03_1->Objects[C].VEC;

	Objects03_1[C + 4] = (int)&SRT_OSort;
	Objects03_1[C + 5] = (int)&SRT_ORun;
	Objects03_1[C + 6] = (int)0;
	Objects03_1[C + 7] = (int)0;
	Objects03_1[C + 8] = (int)&Canvas;
	Objects03_1[C + 9] = (int)&KEY_OPressed;
	Objects03_1[C +10] = (int)&SyncNull03;
	Objects03_1[C +11] = -1;

//---- Creating lightlist...
	Lights03_1 = new int[SCN03_1->NrLights + 1];
	if ( !Lights03_1 )
	{
		SYS_Error(1, "InitPart3: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN03_1->NrLights; C++ )
		Lights03_1[C] = (int)SCN03_1->Lights[C].LGT;

	Lights03_1[C] = -1;


//---- Setting up sync...
	SyncNull03.Type = 1;
	for ( C = 0; C < 16; C++ ) SyncNull03.Plugins[C] = -1;
	SyncNull03.Plugins[0] = (int)&Part3Timing;
	SyncNull03.Plugins[1] = (int)&SyncData03;

	return 1;
}//InitPart3


void UnloadPart3(void)
{
	delete [] Objects03_1;
	delete [] Lights03_1;
	delete SCN03_1;

}//UnloadPart3



void Part3Timing(void)
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
				case 0: SCN01->StartTime = 2012928;
					SyncData01.StartTime = SCN01->StartTime;

					SYS_OListOffs = Objects01;
					SYS_LListOffs = Lights01;

					break;


			}//switch
			Timer->CurrentKey++;
		}

	}//if


}//Part3Timing

