
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

#include "w:\divlibs\ims\binfstd.h"
#include "w:\divlibs\ims\ims.h"
#include "w:\divlibs\ims\itplay.h"

#include "x:\iii\part2\1.h"
#include "x:\iii\part3\2.h"

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>

int C;

int EndTime = 0;

MBL_Data MBL;


itplayerclass itplayer;
void TimerPlugin(void);

//!!!Public Canvas-data!!!
CNV_Data Canvas;
NullObject Timer = {1, (int)&TimerPlugin, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0};

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	SYS_XRes = 320;
	SYS_YRes = 240;


	if ( Aquarium() == NULL ) return;

/*	PAK_FileInfo *PAK = PAK_Open("heart.tls");
	if ( PAK == NULL )
	{
		SYS_Error(1, "main: Error opening archive.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if*/

	if ( !MBL.Init() )
	{
		SYS_Error(1, "main: Not enough memory for MBL_Data.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	printf("Loading...\n");
	if ( !InitPart1() )
	{
		SYS_Error(1, "main: Error initializing part1.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	if ( !InitPart2() )
	{
		SYS_Error(1, "main: Error initializing part2.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if



//------------------- STARTING IMS ------------------

  	imsinitstruct is;
  	imsFillDefaults(is);
  	is.bufsize=65536; // 1sec buffer
  	is.pollmin=61440; // use polling only after 0.9375sec
//  	is.usequiet = 1;

  	if (!imsInit(is))
  	{
    		SYS_Error(1, "main: Could not init IMS.\n");
		printf(SYS_QuitMsg_);
    		return;
  	}//if

	//Loading module...
  	itplayerclass::module mod;

  	sbinfile fil;
  	if (!fil.open("x:\\iii\\part2\\whammer7.it", fil.openro))
//  	if (!fil.open("x:\\iii\\part2\\wham8.it", fil.openro))
  	{
    		SYS_Error(1, "main: Could not open .it\n");
		printf(SYS_QuitMsg_);
    		return;
  	}
  	//cputs("loading\r\n");
  	if ( C = mod.load(fil))
  	{
		printf("IMSError: %d\n", C);
    		SYS_Error(1, "main: Could not load .it\n");
		printf(SYS_QuitMsg_);
    		return;
  	}
  	fil.close();

  	//cputs("uploading\r\n");
  	if (!itplayer.loadsamples(mod))
	{
    		SYS_Error(1, "main: Could not upload samples\n");
		printf(SYS_QuitMsg_);
    		return;
  	}


  	//cputs("playing\r\n");
  	if (!itplayer.play(mod, 64))
  	{
    		SYS_Error(1, "main: Could not play .it\n");
		printf(SYS_QuitMsg_);
    		return;
  	}
	itplayer.setpos(10, 0);

//------------------- DONE IMSSTUFF -----------------


//---- ›alsdijfa›lsdjf
	if ( CNV_Setup() == 0 )
	{
		SYS_Error(1, "main: Error running CNV_Setup.\n");
		printf(SYS_QuitMsg_);
		return;
	}


	SCN01->StartTime = 0;
	SyncData01.StartTime = SCN01->StartTime;


	SYS_OListOffs = Objects01;
	SYS_LListOffs = Lights01;

//---- Running mainloop...

	__asm{
		pushad
		pushfd

		call	MN_MainLoop

		popfd
		popad

	}

	EndTime = SYS_TimerTick;

/*
	FILE *RAWFile = NULL;

	RAW_FILE_Header RAW;
 	RAWFile = fopen("shot.raw", "wb");
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

  	itplayer.stop();
  	mod.free();//*/


  	imsClose();

error:

	float FPS = (float)(EndTime - SCN01->StartTime)/(float)SYS_TicksPerSec;

	UnloadPart1();


	printf(SYS_QuitMsg_);
}//main



void TimerPlugin(void)
{
	SYS_TimerTick = itplayer.gettime();// + 1341952 + 1006464;
}//TimerPlugin
