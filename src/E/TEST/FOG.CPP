#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\vectors\scn_file.h"
#include "w:\vectors\scn_run.h"
#include "w:\divlibs\midas112\include\midasdll.h"
#include "w:\general\sort.h"
#include "w:\vectors\lights.h"
#include "w:\general\canvas.h"
#include "w:\general\main.h"
#include "w:\pak\pak.h"
#include "w:\gfx\gfx.h"
#include "w:\general\2d.h"
#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>

extern "C" void MIDAS_CALL TimerTick();

int *Objects;
int *Lights;
int C;
SCN_Header *SCN;
PAK_FileInfo *PAK = NULL;
FILE *RAWFile = NULL;

RAW_FILE_Header RAW;

FOG_Data Fog;


CNV_Data Canvas;
int EndTime = 0;

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	SYS_XRes = 320;
	SYS_YRes = 240;




	if ( Aquarium() == NULL ) return;
	if ( ArgC <= 1 )
	{
		SYS_Error(1, "main: No .scn-file defined.\n");
		goto error;
	}

//---- Setting up MIDAS-timer-stuff...
	if ( MIDASstartup() == FALSE ) goto error;
//	if ( MIDASloadConfig("soundset") == FALSE ) goto error;
	if ( MIDASsetOption(MIDAS_OPTION_FORCE_NO_SOUND, 1) == FALSE) goto error;
	if ( MIDASinit() == FALSE ) goto error;

	if ( MIDASsetTimerCallbacks(SYS_TicksPerSec*1000, FALSE, &TimerTick, NULL, NULL) == FALSE) goto error;

//---- Loading scene...
	char File[200];
	strcpy(File, ArgV[1]);
	C = strlen(File);
	File[C] = '.';
	File[C + 1] = 'S';
	File[C + 2] = 'C';
	File[C + 3] = 'N';
	File[C + 4] = '\0';


	SCN = SCN_Load(File);
	if ( SCN == NULL )
	{
		goto error;
	}


//---- Creating objectlist...
	Objects = new int[9 + SCN->NrObjects];	//8 without fog

	Objects[0] = (int)&SRT_OReset;
	Objects[1] = (int)SCN;
	Objects[2] = (int)&LIT_Init;

	for ( C = 0; C < SCN->NrObjects; C++ )
		Objects[C + 3] = (int)SCN->Objects[C].VEC;

	Objects[C + 3] = (int)&SRT_OSort;
	Objects[C + 4] = (int)&SRT_ORun;
	Objects[C + 5] = (int)&Fog;
	Objects[C + 6] = (int)&Canvas;
	Objects[C + 7] = (int)&KEY_OPressed;
	Objects[C + 8] = -1;

//---- Creating lightlist...
	Lights = new int[SCN->NrLights + 1];

	for ( C = 0; C < SCN->NrLights; C++ )
		Lights[C] = (int)SCN->Lights[C].LGT;

	Lights[C] = -1;

	SYS_LListOffs = Lights;
	SYS_OListOffs = Objects;

//---- Setting up fog
	Fog.FOG_Type = 17;
	Fog.FOG_Plugins[0] = -1;

	Fog.FOG_R = 0.8;
	Fog.FOG_G = 0.6;
	Fog.FOG_B = 0.4;

	Fog.FOG_Distance = 16;


	//Canvas.Flags = CNV_FlagOptimizeZBuf;
	SYS_ZBufFP = 2147483640.0;

//---- ›alsdijfa›lsdjf
	if ( CNV_Setup() == 0 )
	{
		SYS_Error(1, "main: Error running CNV_Setup.\n");
		goto error;
	}
	SCN->StartTime = SYS_TimerTick;

//---- Running mainloop...

	__asm{
		pushad
		pushfd

		call	MN_MainLoop

		popfd
		popad

	}

	EndTime = SYS_TimerTick;

/*	RAWFile = fopen("shot.raw", "wb");
	if ( RAWFile != NULL )
	{
		memset(&RAW, 0, 32);
		RAW.XResHi = SYS_XRes >> 8;
		RAW.XResLo = SYS_XRes & 0xFF;
		RAW.YResHi = SYS_YRes >> 8;
		RAW.YResLo = SYS_YRes & 0xFF;

		RAW.PAD1[0] = 'm';
		RAW.PAD1[1] = 'h';
		RAW.PAD1[2] = 'w';
		RAW.PAD1[3] = 'a';
		RAW.PAD1[4] = 'n';
		RAW.PAD1[5] = 'h';

		RAW.PAD1[6] = 0;
		RAW.PAD1[7] = 4;


		if ( fwrite(&RAW, 32, 1, RAWFile) < 1 )
		{
			SYS_Error(1, "main: Error dumping raw-file.\n");
		}

		__asm{
			pushad
			pushfd

			call	SRT_RRun

			mov	esi, SYS_DBufOffs
			mov	edi, SYS_ZBufOffs	//using zbuf as tmpbuf

			mov	ecx, SYS_XRes
			imul	ecx, SYS_YRes

		Loop16_24:
			mov	ax, [esi]

			mov	ebx, 0x1F
			and	ebx, eax
			shl	ebx, 3
			mov	[edi + 2], bl		//saving B

			mov	ebx, 0x7E0
			and	ebx, eax
			shr	ebx, 5
			shl	ebx, 2
			mov	[edi + 1], bl

			mov	ebx, 0xF800
			and	ebx, eax
			shr	ebx, 11
			shl	ebx, 3
			mov	[edi + 0], bl

			add	esi, 2
			add	edi, 3
			dec	ecx
			jnz	Loop16_24

			popfd
			popad

		}//asm

		if ( fwrite(SYS_ZBufOffs, SYS_XRes*SYS_YRes*3, 1, RAWFile) < 1 )
			SYS_Error(1, "main: Error writing raw-dump.\n");
		fclose(RAWFile);
	}//if
	else SYS_Error(1, "main: Error opening file for raw-dump.\n");*/





//---- Cleaning up...

	__asm{
		push	eax

	      	mov   	ah, 0                    //Init screen number
	       	mov   	al, 3h
	       	int   	10h                      //Do it!

		pop	eax

	}//asm

error:

	MIDASclose();

	float FPS = (float)(EndTime - SCN->StartTime)/(float)SYS_TicksPerSec;

	delete SCN;

	int ErrorCode = MIDASgetLastError();
	char *ErrorString = MIDASgetErrorMessage(ErrorCode);
	printf("MIDAS: ");
	printf(ErrorString);
	printf("\n");
	printf(SYS_QuitMsg_);

	printf("Number of frames: %d\n", SYS_NrFrames);
	printf("FPS: %f\n", (float)SYS_NrFrames/FPS);
}//main
