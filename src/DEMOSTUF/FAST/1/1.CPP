
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
extern itplayerclass itplayer;
extern MBL_Data MBL;

//---- Datavariables for part1...
int    	       *Objects01 	= NULL;
int	       *Objects02	= NULL;
int	       *Objects03	= NULL;
int    	       *Objects04 	= NULL;
int	       *Objects05	= NULL;
int	       *Objects06	= NULL;
int    	       *Objects07 	= NULL;
int	       *Objects08	= NULL;
int	       *Objects09	= NULL;
int	       *Objects10	= NULL;
int    	       *Objects11 	= NULL;
int	       *Objects12	= NULL;
int	       *Objects13	= NULL;
int    	       *Objects14 	= NULL;
int	       *Objects15	= NULL;
int	       *Objects16	= NULL;
int    	       *Objects17 	= NULL;
int	       *Objects18	= NULL;
int	       *Objects19	= NULL;
int    	       *Lights01 	= NULL;
int	       *Lights02	= NULL;
int	       *Lights03	= NULL;
int    	       *Lights04 	= NULL;
int	       *Lights05	= NULL;
int	       *Lights06	= NULL;
int    	       *Lights07 	= NULL;
int	       *Lights08	= NULL;
int	       *Lights09	= NULL;
int    	       *Lights10 	= NULL;
int    	       *Lights11 	= NULL;
int	       *Lights12	= NULL;
int	       *Lights13	= NULL;
int    	       *Lights14 	= NULL;
int	       *Lights15	= NULL;
int	       *Lights16	= NULL;
int    	       *Lights17 	= NULL;
int	       *Lights18	= NULL;
int	       *Lights19	= NULL;
SCN_Header     *SCN01 		= NULL;
SCN_Header     *SCN02 		= NULL;
SCN_Header     *SCN03 		= NULL;
SCN_Header     *SCN04 		= NULL;
SCN_Header     *SCN05 		= NULL;
SCN_Header     *SCN06 		= NULL;
SCN_Header     *SCN07 		= NULL;
SCN_Header     *SCN08 		= NULL;
SCN_Header     *SCN09 		= NULL;
SCN_Header     *SCN10 		= NULL;
SCN_Header     *SCN11 		= NULL;
SCN_Header     *SCN12 		= NULL;
SCN_Header     *SCN13 		= NULL;
SCN_Header     *SCN14 		= NULL;
SCN_Header     *SCN15 		= NULL;
SCN_Header     *SCN16 		= NULL;
SCN_Header     *SCN17 		= NULL;
SCN_Header     *SCN18 		= NULL;
SCN_Header     *SCN19 		= NULL;

BLN_Data Blend1;

BLN3_Data AddBlend1;
BLN3_Data AddBlend2;


void Part1Timing(void);
void Part1MusicSync(void);
NullObject SyncNull;
TIM_Data SyncData01 = {
				0, 0,
				18, 0,
				436992,
				436992*2,	//galaxe starter
				436992*3,	//2 sec (p†)
				436992*4,	//2.5 sec (av)
				436992*5,	//2.5 sec (av)
				436992*7 + 218496*0,	//2.5 sec (av)
				436992*7 + 218496*1,	//2.5 sec (av)
				436992*7 + 218496*2,	//2.5 sec (av)
				436992*7 + 218496*3,	//2.5 sec (av)
				436992*9,
				436992*14,
				436992*16,
				436992*17,
				436992*18,
				436992*19,
				436992*20,
				436992*21,
				436992*22,

			};



int InitPart1(void)
{
	int C = 0;

	FLR_Priority = 239;
	V08_Priority = 240;



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



//********************************* FIRST SCENE *************************
//---- Loading scene...

	SCN01 = SCN_Load("x:\\fast\\scenes\\bilde.scn");
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
	Objects01[C + 6] = (int)&AddBlend1;
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


//********************************* 2. SCENE *************************
//---- Loading scene...

	SCN02 = SCN_Load("x:\\fast\\scenes\\credits.scn");
	if ( SCN02 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
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
	Objects02[C + 6] = (int)&AddBlend1;
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


//********************************* 3. SCENE *************************
//---- Loading scene...

	SCN03 = SCN_Load("x:\\fast\\scenes\\a03.scn");
	if ( SCN03 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects03 = new int[12 + SCN03->NrObjects];	//8 without fog
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
	Objects03[C + 6] = (int)&AddBlend1;
	Objects03[C + 7] = (int)0;//&Blend1;
	Objects03[C + 8] = (int)&Canvas;
	Objects03[C + 9] = (int)&KEY_OPressed;
	Objects03[C +10] = (int)&SyncNull;
	Objects03[C +11] = -1;

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

//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN04 = SCN_Load("x:\\fast\\scenes\\a04.scn");
	if ( SCN04 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects04 = new int[12 + SCN04->NrObjects];	//8 without fog
	if ( !Objects04 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects04[0] = (int)&Timer;
	Objects04[1] = (int)&SRT_OReset;
	Objects04[2] = (int)SCN04;
	Objects04[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN04->NrObjects; C++ )
		Objects04[C + 4] = (int)SCN04->Objects[C].VEC;

	Objects04[C + 4] = (int)&SRT_OSort;
	Objects04[C + 5] = (int)&SRT_ORun;
	Objects04[C + 6] = (int)&AddBlend1;
	Objects04[C + 7] = (int)0;//&Blend1;
	Objects04[C + 8] = (int)&Canvas;
	Objects04[C + 9] = (int)&KEY_OPressed;
	Objects04[C +10] = (int)&SyncNull;
	Objects04[C +11] = -1;

//---- Creating lightlist...
	Lights04 = new int[SCN04->NrLights + 1];
	if ( !Lights04 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN04->NrLights; C++ )
		Lights04[C] = (int)SCN04->Lights[C].LGT;

	Lights04[C] = -1;



//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN05 = SCN_Load("x:\\fast\\scenes\\a02.scn");
	if ( SCN05 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects05 = new int[12 + SCN05->NrObjects];	//8 without fog
	if ( !Objects05 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects05[0] = (int)&Timer;
	Objects05[1] = (int)&SRT_OReset;
	Objects05[2] = (int)SCN05;
	Objects05[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN05->NrObjects; C++ )
		Objects05[C + 4] = (int)SCN05->Objects[C].VEC;

	Objects05[C + 4] = (int)&SRT_OSort;
	Objects05[C + 5] = (int)&SRT_ORun;
	Objects05[C + 6] = (int)&AddBlend1;
	Objects05[C + 7] = (int)0;//&Blend1;
	Objects05[C + 8] = (int)&Canvas;
	Objects05[C + 9] = (int)&KEY_OPressed;
	Objects05[C +10] = (int)&SyncNull;
	Objects05[C +11] = -1;

//---- Creating lightlist...
	Lights05 = new int[SCN05->NrLights + 1];
	if ( !Lights05 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN05->NrLights; C++ )
		Lights05[C] = (int)SCN05->Lights[C].LGT;

	Lights05[C] = -1;



//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN06 = SCN_Load("x:\\fast\\scenes\\a06.scn");
	if ( SCN06 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects06 = new int[12 + SCN06->NrObjects];	//8 without fog
	if ( !Objects06 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects06[0] = (int)&Timer;
	Objects06[1] = (int)&SRT_OReset;
	Objects06[2] = (int)SCN06;
	Objects06[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN06->NrObjects; C++ )
		Objects06[C + 4] = (int)SCN06->Objects[C].VEC;

	Objects06[C + 4] = (int)&SRT_OSort;
	Objects06[C + 5] = (int)&SRT_ORun;
	Objects06[C + 6] = (int)&AddBlend1;
	Objects06[C + 7] = (int)0;//&Blend1;
	Objects06[C + 8] = (int)&Canvas;
	Objects06[C + 9] = (int)&KEY_OPressed;
	Objects06[C +10] = (int)&SyncNull;
	Objects06[C +11] = -1;

//---- Creating lightlist...
	Lights06 = new int[SCN06->NrLights + 1];
	if ( !Lights06 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN06->NrLights; C++ )
		Lights06[C] = (int)SCN06->Lights[C].LGT;

	Lights06[C] = -1;


//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN07 = SCN_Load("x:\\fast\\scenes\\a01.scn");
	if ( SCN07 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}




//---- Creating objectlist...
	Objects07 = new int[12 + SCN07->NrObjects];	//8 without fog
	if ( !Objects07 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects07[0] = (int)&Timer;
	Objects07[1] = (int)&SRT_OReset;
	Objects07[2] = (int)SCN07;
	Objects07[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN07->NrObjects; C++ )
		Objects07[C + 4] = (int)SCN07->Objects[C].VEC;

	Objects07[C + 4] = (int)&SRT_OSort;
	Objects07[C + 5] = (int)&SRT_ORun;
	Objects07[C + 6] = (int)&AddBlend1;
	Objects07[C + 7] = (int)0;//&Blend1;
	Objects07[C + 8] = (int)&Canvas;
	Objects07[C + 9] = (int)&KEY_OPressed;
	Objects07[C +10] = (int)&SyncNull;
	Objects07[C +11] = -1;

//---- Creating lightlist...
	Lights07 = new int[SCN07->NrLights + 1];
	if ( !Lights07 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN07->NrLights; C++ )
		Lights07[C] = (int)SCN07->Lights[C].LGT;

	Lights07[C] = -1;


//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN08 = SCN_Load("x:\\fast\\scenes\\tunnel.scn");
	if ( SCN08 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects08 = new int[12 + SCN08->NrObjects];	//8 without fog
	if ( !Objects08 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects08[0] = (int)&Timer;
	Objects08[1] = (int)&SRT_OReset;
	Objects08[2] = (int)SCN08;
	Objects08[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN08->NrObjects; C++ )
		Objects08[C + 4] = (int)SCN08->Objects[C].VEC;

	Objects08[C + 4] = (int)&SRT_OSort;
	Objects08[C + 5] = (int)&SRT_ORun;
	Objects08[C + 6] = (int)&AddBlend1;
	Objects08[C + 7] = (int)0;//&Blend1;
	Objects08[C + 8] = (int)&Canvas;
	Objects08[C + 9] = (int)&KEY_OPressed;
	Objects08[C +10] = (int)&SyncNull;
	Objects08[C +11] = -1;

//---- Creating lightlist...
	Lights08 = new int[SCN08->NrLights + 1];
	if ( !Lights08 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN08->NrLights; C++ )
		Lights08[C] = (int)SCN08->Lights[C].LGT;

	Lights08[C] = -1;


//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN09 = SCN_Load("x:\\fast\\scenes\\nn.scn");
	if ( SCN09 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects09 = new int[12 + SCN09->NrObjects];	//8 without fog
	if ( !Objects09 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects09[0] = (int)&Timer;
	Objects09[1] = (int)&SRT_OReset;
	Objects09[2] = (int)SCN09;
	Objects09[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN09->NrObjects; C++ )
		Objects09[C + 4] = (int)SCN09->Objects[C].VEC;

	Objects09[C + 4] = (int)&SRT_OSort;
	Objects09[C + 5] = (int)&SRT_ORun;
	Objects09[C + 6] = (int)&AddBlend1;
	Objects09[C + 7] = (int)0;//&Blend1;
	Objects09[C + 8] = (int)&Canvas;
	Objects09[C + 9] = (int)&KEY_OPressed;
	Objects09[C +10] = (int)&SyncNull;
	Objects09[C +11] = -1;

//---- Creating lightlist...
	Lights09 = new int[SCN09->NrLights + 1];
	if ( !Lights09 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN09->NrLights; C++ )
		Lights09[C] = (int)SCN09->Lights[C].LGT;

	Lights09[C] = -1;



//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN10 = SCN_Load("x:\\fast\\scenes\\a09.scn");
	if ( SCN10 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects10 = new int[12 + SCN10->NrObjects];	//8 without fog
	if ( !Objects10 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects10[0] = (int)&Timer;
	Objects10[1] = (int)&SRT_OReset;
	Objects10[2] = (int)SCN10;
	Objects10[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN10->NrObjects; C++ )
		Objects10[C + 4] = (int)SCN10->Objects[C].VEC;

	Objects10[C + 4] = (int)&SRT_OSort;
	Objects10[C + 5] = (int)&SRT_ORun;
	Objects10[C + 6] = (int)&AddBlend1;
	Objects10[C + 7] = (int)0;//&Blend1;
	Objects10[C + 8] = (int)&Canvas;
	Objects10[C + 9] = (int)&KEY_OPressed;
	Objects10[C +10] = (int)&SyncNull;
	Objects10[C +11] = -1;

//---- Creating lightlist...
	Lights10 = new int[SCN10->NrLights + 1];
	if ( !Lights10 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN10->NrLights; C++ )
		Lights10[C] = (int)SCN10->Lights[C].LGT;

	Lights10[C] = -1;


//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN11 = SCN_Load("x:\\fast\\scenes\\a08.scn");
	if ( SCN11 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects11 = new int[12 + SCN11->NrObjects];	//8 without fog
	if ( !Objects11 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects11[0] = (int)&Timer;
	Objects11[1] = (int)&SRT_OReset;
	Objects11[2] = (int)SCN11;
	Objects11[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN11->NrObjects; C++ )
		Objects11[C + 4] = (int)SCN11->Objects[C].VEC;

	Objects11[C + 4] = (int)&SRT_OSort;
	Objects11[C + 5] = (int)&SRT_ORun;
	Objects11[C + 6] = (int)&AddBlend1;
	Objects11[C + 7] = (int)0;//&Blend1;
	Objects11[C + 8] = (int)&Canvas;
	Objects11[C + 9] = (int)&KEY_OPressed;
	Objects11[C +10] = (int)&SyncNull;
	Objects11[C +11] = -1;

//---- Creating lightlist...
	Lights11 = new int[SCN11->NrLights + 1];
	if ( !Lights11 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN11->NrLights; C++ )
		Lights11[C] = (int)SCN11->Lights[C].LGT;

	Lights11[C] = -1;


//********************************* 4. SCENE *************************
//---- Loading scene...

	SCN12 = SCN_Load("x:\\fast\\scenes\\oioi.scn");
	if ( SCN12 == NULL )
	{
		SYS_Error(1, "InitPart1: Error loading scene.\n");
		return 0;
	}



//---- Creating objectlist...
	Objects12 = new int[12 + SCN12->NrObjects];	//8 without fog
	if ( !Objects12 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for objectlist.\n");
		return 0;

	}//if

	Objects12[0] = (int)&Timer;
	Objects12[1] = (int)&SRT_OReset;
	Objects12[2] = (int)SCN12;
	Objects12[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN12->NrObjects; C++ )
		Objects12[C + 4] = (int)SCN12->Objects[C].VEC;

	Objects12[C + 4] = (int)&SRT_OSort;
	Objects12[C + 5] = (int)&SRT_ORun;
	Objects12[C + 6] = (int)&AddBlend1;
	Objects12[C + 7] = (int)0;//&Blend1;
	Objects12[C + 8] = (int)&Canvas;
	Objects12[C + 9] = (int)&KEY_OPressed;
	Objects12[C +10] = (int)&SyncNull;
	Objects12[C +11] = -1;

//---- Creating lightlist...
	Lights12 = new int[SCN12->NrLights + 1];
	if ( !Lights12 )
	{
		SYS_Error(1, "InitPart1: Not enough memory for lightlist.\n");
		return 0;
	}//if

	for ( C = 0; C < SCN12->NrLights; C++ )
		Lights12[C] = (int)SCN12->Lights[C].LGT;

	Lights12[C] = -1;




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

	return 1;
}//InitPart1


void UnloadPart1(void)
{
	delete [] Objects01;
	delete [] Lights01;
	delete SCN01;

/*	delete [] Objects02;
	delete [] Lights02;
	delete SCN02;

	delete [] Objects03;
	delete [] Lights03;
	delete SCN03;*/
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
					SCN02->StartTime = 436992*1;
					break;

				case 1: SYS_OListOffs = Objects03;
					SYS_LListOffs = Lights03;
					SCN03->StartTime = 436992*2;
					break;
				case 2: SYS_OListOffs = Objects04;
					SYS_LListOffs = Lights04;
					SCN04->StartTime = 436992*3;
					break;

				case 3: SYS_OListOffs = Objects05;
					SYS_LListOffs = Lights05;
					SCN05->StartTime = 436992*4;
					break;
				case 4: SYS_OListOffs = Objects06;
					SYS_LListOffs = Lights06;
					SCN06->StartTime = 436992*5;
					break;

				case 5: SYS_OListOffs = Objects03;
					SYS_LListOffs = Lights03;
					SCN03->StartTime = 436992*7;
					break;
				case 6: SYS_OListOffs = Objects07;
					SYS_LListOffs = Lights07;
					SCN07->StartTime = 436992*7 + 218496*1;
					break;
				case 7: SYS_OListOffs = Objects04;
					SYS_LListOffs = Lights04;
					SCN04->StartTime = 436992*7 + 218496*2;
					break;
				case 8: SYS_OListOffs = Objects05;
					SYS_LListOffs = Lights05;
					SCN05->StartTime = 436992*7 + 218496*3;
					break;
				case 9: SYS_OListOffs = Objects08;
					SYS_LListOffs = Lights08;
					SCN08->StartTime = 436992*9;
					break;

				case 10: SYS_OListOffs = Objects09;
					SYS_LListOffs = Lights09;
					SCN09->StartTime = 436992*14;
					break;

				case 11: SYS_OListOffs = Objects10;
					SYS_LListOffs = Lights10;
					SCN10->StartTime = 436992*16;
					break;

				case 12: SYS_OListOffs = Objects11;
					SYS_LListOffs = Lights11;
					SCN11->StartTime = 436992*17;
					break;

				case 13: SYS_OListOffs = Objects10;
					SYS_LListOffs = Lights10;
					SCN10->StartTime = 436992*18;
					break;
				case 14: SYS_OListOffs = Objects11;
					SYS_LListOffs = Lights11;
					SCN11->StartTime = 436992*19;
					break;
				case 15: SYS_OListOffs = Objects12;
					SYS_LListOffs = Lights12;
					SCN12->StartTime = 436992*20;
					break;
				case 16: SYS_QuitFlag = -1;
					 SYS_Error(1, "TLS/ELQ Fastintro. Vote for us!\n");
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
  	int val = itplayer.getsync(0, time);

	if ( (time != -1) && (val == 00) )
	{

		AddBlend1.BLN3_StartTime = SYS_TimerTick - time;

	}//if

}//void


