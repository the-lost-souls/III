
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
#include "w:\general\txt.h"

#include "w:\divlibs\ims\binfstd.h"
#include "w:\divlibs\ims\ims.h"
#include "w:\divlibs\ims\itplay.h"

#include "x:\iii\part4\3.h"
#include "x:\iii\part2\1.h"
#include "x:\iii\part3\2.h"
#include "x:\iii\part5\5.h"
#include "x:\iii\part6\6.h"
#include "x:\iii\part7\7.h"
#include "x:\iii\part8\8.h"

#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>

int C;

itplayerclass *itplayer;
int LoopFlag = 0;

void TimerPlugin(void);
int TimerPhuck = 0;

//!!!Public Canvas-data!!!
CNV_Data Canvas;
MBL_Data MBL;
NullObject Timer = {1, (int)&TimerPlugin, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0};

//190974

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	if ( Aquarium(FALSE) == NULL ) return;

//	SYS_QuitFlag = -2;

	static itplayerclass itplayer_STATIC;
	itplayer = &itplayer_STATIC;
  	imsinitstruct is;
  	imsFillDefaults(is);
  	is.bufsize=65536; // 1sec buffer
  	is.pollmin=61440; // use polling only after 0.9375sec


	SYS_XRes = 320;
	SYS_YRes = 240;

	C = 1;
	while ( C <= ArgC )
	{
		if ( strcmp(ArgV[C], "400x300") == 0 )
		{
			SYS_XRes = 400;
			SYS_YRes = 300;
		}//if
		if ( strcmp(ArgV[C], "512x384") == 0 )
		{
			SYS_XRes = 512;
			SYS_YRes = 384;
		}//if
		if ( strcmp(ArgV[C], "640x480") == 0 )
		{
			SYS_XRes = 640;
			SYS_YRes = 480;
		}//if
		if ( strcmp(ArgV[C], "800x600") == 0 )
		{
			SYS_XRes = 800;
			SYS_YRes = 600;
		}//if
		if ( strcmp(ArgV[C], "1024x768") == 0 )
		{
			SYS_XRes = 1024;
			SYS_YRes = 768;
		}//if
		if ( strcmp(ArgV[C], "32") == 0 )
		{
			TXT_ScalingFlag = 1;
		}//if
		if ( strcmp(ArgV[C], "shutup") == 0 )
		{
			is.usequiet=1;
		}//if
		if ( strcmp(ArgV[C], "loop") == 0 )
		{
			LoopFlag = 1;
		}//if

		if ( (strcmp(ArgV[C], "?") == 0)
		|| (strcmp(ArgV[C], "-?") == 0)
		|| (strcmp(ArgV[C], "/?") == 0)
		|| (strcmp(ArgV[C], "-h") == 0)
		|| (strcmp(ArgV[C], "-H") == 0)
		|| (strcmp(ArgV[C], "/h") == 0)
		|| (strcmp(ArgV[C], "/H") == 0))
		{
			printf("\"III - live tomorrow\"\nThe Lost Souls 1999\n\n\n");
			printf("\nCommand line arguments:\n");
			printf("  400x300  = run demo in 400x300 mode\n");
			printf("  512x384  = run demo in 512x384 mode\n");
			printf("  640x480  = run demo in 640x480 mode\n");
			printf("  800x600  = run demo in 800x600 mode\n");
			printf(" 1024x768  = run demo in 1024x768 mode\n");
			printf("       32  = mess up textures so demo run with 32M free mem.\n");
			printf(" shutup    = no music\n");
			printf(" loop      = loop\n");
			return;
		}//if
		C++;

	}//while

	SYS_Error(1, "\n\n");

	PAK_FileInfo *PAK = PAK_Open("iii.tls");
	if ( PAK == NULL )
	{
		SYS_Error(1, "main: Error opening archive.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if*/


//------------------- STARTING IMS ------------------

  	if (!imsInit(is))
  	{
    		SYS_Error(1, "main: Could not init IMS.\n");
		printf(SYS_QuitMsg_);
    		return;
  	}//if

	printf("\"III - live tomorrow\"\nThe Lost Souls 1999\n\n\n");

	if ( is.usequiet ) printf("Running demo without music...\n");
	if ( LoopFlag ) printf("Looping demo...\n");
	if ( TXT_ScalingFlag ) printf("Running demo in 32M mode...\n");
	printf("Trying to run demo in %dx%dx32... (-h for options)\n", SYS_XRes, SYS_YRes);

	cerr << "\r" << "Loading [.        ]" << "\t";

	//Loading module...
  	itplayerclass::module mod;

  	sbinfile fil;
  	if (!fil.open("whammer9.it", fil.openro))
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
  	if (!itplayer_STATIC.loadsamples(mod))
	{
    		SYS_Error(1, "main: Could not upload samples\n");
		printf(SYS_QuitMsg_);
    		return;
  	}



//------------ Loading parts...


	if ( !MBL.Init() )
	{
		SYS_Error(1, "main: Not enough memory for MBL_Data.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	cerr << "\r" << "Loading [..       ]" << "\t";
	if ( !InitPart3() )
	{
		SYS_Error(1, "main: Error initializing part3.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	cerr << "\r" << "Loading [...      ]" << "\t";

	if ( !InitPart1() )
	{
		SYS_Error(1, "main: Error initializing part1.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	cerr << "\r" << "Loading [....     ]" << "\t";

	if ( !InitPart2() )
	{
		SYS_Error(1, "main: Error initializing part2.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	cerr << "\r" << "Loading [.....    ]" << "\t";

	if ( !InitPart5() )
	{
		SYS_Error(1, "main: Error initializing part5.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if
	cerr << "\r" << "Loading [......   ]" << "\t";

	if ( !InitPart6() )
	{
		SYS_Error(1, "main: Error initializing part6.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if
	cerr << "\r" << "Loading [.......  ]" << "\t";

	if ( !InitPart7() )
	{
		SYS_Error(1, "main: Error initializing part7.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if
	cerr << "\r" << "Loading [........ ]" << "\t";

	if ( !InitPart8() )
	{
		SYS_Error(1, "main: Error initializing part8.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if
	cerr << "\r" << "Loading [.........]" << "\t";

//------------------- Playing... ------------------

  	//cputs("playing\r\n");
  	if (!itplayer_STATIC.play(mod, 64))
  	{
    		SYS_Error(1, "main: Could not play .it\n");
		printf(SYS_QuitMsg_);
    		return;
  	}

//------------------- DONE IMSSTUFF -----------------


//---- ›alsdijfa›lsdjf
	Canvas.Flags = CNV_FlagOptimizeZBuf;
	if ( CNV_Setup() == 0 )
	{
		SYS_Error(1, "main: Error running CNV_Setup.\n");
		printf(SYS_QuitMsg_);
		return;
	}

LoopPoint:

	SCN03_1->StartTime = 0;
	SyncData03.StartTime = SCN03_1->StartTime;

	SYS_OListOffs = Objects03_1;
	SYS_LListOffs = Lights03_1;



//---- Running mainloop...
	__asm{
		pushad
		pushfd

		call	MN_MainLoop

		popfd
		popad

	}

	if ( (LoopFlag == 1) && (SYS_QuitFlag == -2) )
	{

		SYS_TimerTick = 0;
		SYS_QuitFlag = 0;
		SyncData01.CurrentKey = 0;
		SyncData02.CurrentKey = 0;
		SyncData03.CurrentKey = 0;
		SyncData05.CurrentKey = 0;
		SyncData06.CurrentKey = 0;
		SyncData07.CurrentKey = 0;
		SyncData08.CurrentKey = 0;

		itplayer_STATIC.setpos(0, 0);
		TimerPhuck = itplayer_STATIC.gettime();

		goto LoopPoint;
	}//if
  	itplayer_STATIC.stop();


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


  	mod.free();//*/
  	imsClose();



error:

	UnloadPart3();
	UnloadPart1();
	UnloadPart2();
	UnloadPart5();
	UnloadPart6();
	UnloadPart7();
	UnloadPart8();


	printf(SYS_QuitMsg_);
}//main



void TimerPlugin(void)
{
	SYS_TimerTick = itplayer->gettime() - TimerPhuck;// + 1341952 + 1006464;
}//TimerPlugin
