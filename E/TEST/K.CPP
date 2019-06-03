
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

int C;

int EndTime = 0;


static itplayerclass itplayer;
void TimerPlugin(void);
void WaterFallThingyPlugin(void);

SCN_Header *SCN = NULL;

void *Tmp1 = NULL;
void *Tmp2 = NULL;
void *Tmp3 = NULL;
void *Tmp4 = NULL;
void *Tmp5 = NULL;


void main(int ArgC, char *ArgV[], char *EnvP[])
{
	SYS_XRes = 320;
	SYS_YRes = 240;


	if ( Aquarium() == NULL ) return;
	if ( ArgC <= 1 )
	{
		SYS_Error(1, "main: No .scn-file defined.\n");
		printf(SYS_QuitMsg_);
		return;
	}

//	PAK_FileINfo *PAK = PAK_Open("island.pak");
//	if ( PAK == NULL ) goto error;


//------------------- STARTING IMS ------------------

  	imsinitstruct is;
  	imsFillDefaults(is);
  	is.bufsize=65536; // 1sec buffer
  	is.pollmin=61440; // use polling only after 0.9375sec
  	is.usequiet = 1;

  	if (!imsInit(is))
  	{
    		SYS_Error(1, "main: Could not init IMS.\n");
		printf(SYS_QuitMsg_);
    		return;
  	}//if

	//Loading module...
  	itplayerclass::module mod;

  	sbinfile fil;
  	if (!fil.open("x:\\timer.it", fil.openro))
//  	if (!fil.open("c:\\frityr\\pissant.it", fil.openro))
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

//------------------- DONE IMSSTUFF -----------------
	CNV_Data Canvas;

	NullObject Timer = {	1,
				(int)&TimerPlugin, 0,
				(int)&WaterFallThingyPlugin, 0,
				-1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0};


//---- Setting up fog
	FOG_Data Fog;
	Fog.FOG_Type = 17;
	Fog.FOG_Plugins[0] = -1;

	Fog.FOG_R = 0.8;
	Fog.FOG_G = 0.6;
	Fog.FOG_B = 0.4;

	Fog.FOG_Distance = 16;

	Canvas.Flags = CNV_FlagOptimizeZBuf;
	//SYS_ZBufFP = 2147483640.0;


//---- Setting up motion blur...
	MBL_Data MBLDings;
	if ( !MBLDings.Init() )
	{
		printf(SYS_QuitMsg_);
		return;
	}

//---- Setting up additive blending...
	BLN2_Data BLN2;
	if ( !BLN2.Init() )
	{
		printf(SYS_QuitMsg_);
		return;
	}

	STR_StarField Stars;



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
		printf(SYS_QuitMsg_);
		return;
	}


//---- Creating objectlist...
	int *Objects = new int[11 + SCN->NrObjects];	//8 without fog

	Objects[0] = (int)&Timer;
	Objects[1] = (int)&SRT_OReset;
	Objects[2] = (int)SCN;
	Objects[3] = (int)&LIT_Init;

	for ( C = 0; C < SCN->NrObjects; C++ )
		Objects[C + 4] = (int)SCN->Objects[C].VEC;

	Objects[C + 4] = (int)&SRT_OSort;
	Objects[C + 5] = (int)&SRT_ORun;
	Objects[C + 6] = (int)&MBLDings;
	Objects[C + 7] = (int)0;//&Stars;
	Objects[C + 8] = (int)&Canvas;
	Objects[C + 9] = (int)&KEY_OPressed;
	Objects[C +10] = -1;

//---- Creating lightlist...
	int *Lights = new int[SCN->NrLights + 1];

	for ( C = 0; C < SCN->NrLights; C++ )
		Lights[C] = (int)SCN->Lights[C].LGT;

	Lights[C] = -1;

	SYS_LListOffs = Lights;
	SYS_OListOffs = Objects;


//---- Fixing different stuff...
	Tmp1 = SCN->Objects[55].VEC->H_Materials[1].M_MapOffset;

	Tmp2 = SCN->Objects[5].VEC->H_Materials[1].M_MapOffset;
	Tmp3 = SCN->Objects[7].VEC->H_Materials[1].M_MapOffset;
	Tmp4 = SCN->Objects[44].VEC->H_Materials[1].M_MapOffset;
	Tmp5 = SCN->Objects[45].VEC->H_Materials[1].M_MapOffset;


//---- Setting up starfield
	Stars.SetRange(-500, -500, -500, 500, 500, 500);
	Stars.SetSpeed(0, -1, 0);
	if ( !Stars.Init(5000, SCN) )
	{
		SYS_Error(1, "main: Error setting up starfield.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if


//---- ›alsdijfa›lsdjf
	if ( CNV_Setup() == 0 )
	{
		SYS_Error(1, "main: Error running CNV_Setup.\n");
		printf(SYS_QuitMsg_);
		return;
	}

	SYS_FPClipTop = SYS_YRes*0.15;
	SYS_FPClipBottom = SYS_YRes*0.85;


	SCN->StartTime = itplayer.gettime();

//---- Running mainloop...

	__asm{
		pushad
		pushfd

		call	MN_MainLoop

		popfd
		popad

	}

	EndTime = SYS_TimerTick;


/*	FILE *RAWFile = NULL;

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

		__asm int 3


		if ( fwrite(&RAW, 32, 1, RAWFile) < 1 )
		{
			SYS_Error(1, "main: Error dumping raw-file.\n");
		}

		__asm{
			push	ebp
			call	SRT_RRun
			pop	ebp
		}

		SYS_OOffs = (void*)&MBLDings;
		MBL_Process();


		__asm{
			pushad
			pushfd

			mov	esi, SYS_DBufOffs
			mov	edi, SYS_ZBufOffs	//using zbuf as tmpbuf

			mov	ecx, SYS_XRes
			imul	ecx, SYS_YRes

		Loop32_24:
			mov	bl, [esi]

			mov	[edi + 2], bl		//saving B

			mov	bl, [esi + 1]
			mov	[edi + 1], bl

			mov	bl, [esi + 2]
			mov	[edi + 0], bl

			add	esi, 4
			add	edi, 3
			dec	ecx
			jnz	Loop32_24

			popfd
			popad

		}//asm

		if ( fwrite(SYS_ZBufOffs, SYS_XRes*SYS_YRes*3, 1, RAWFile) < 1 )
			SYS_Error(1, "main: Error writing raw-dump.\n");
		fclose(RAWFile);
	}//if
	else SYS_Error(1, "main: Error opening file for raw-dump.\n");//*/





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

	SCN->Objects[55].VEC->H_Materials[1].M_MapOffset = Tmp1;

	SCN->Objects[5].VEC->H_Materials[1].M_MapOffset = Tmp2;
	SCN->Objects[7].VEC->H_Materials[1].M_MapOffset = Tmp3;
	SCN->Objects[44].VEC->H_Materials[1].M_MapOffset = Tmp4;
	SCN->Objects[45].VEC->H_Materials[1].M_MapOffset = Tmp5;


error:

	float FPS = (float)(EndTime - SCN->StartTime)/(float)SYS_TicksPerSec;

	delete SCN;

	printf(SYS_QuitMsg_);

	printf("Number of frames: %d\n", SYS_NrFrames);
	printf("FPS: %f\n", (float)SYS_NrFrames/FPS);
}//main



void TimerPlugin(void)
{
	SYS_TimerTick = itplayer.gettime()*4;
}//TimerPlugin



void WaterFallThingyPlugin(void)
{
//---- Calculating time...
	int CurrentTime = SYS_TimerTick;
	float Seconds = (float)(CurrentTime - SCN->StartTime)/(float)SYS_TicksPerSec;
	float InFrame = SCN->FirstFrame + Seconds*SCN->FramesPerSecond;


	float TempMatrix[12];
	FLR_Data *FLR = NULL;

//---- fading flares...
	SCN_GetObjectMatrix(InFrame, &SCN->Objects[10], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[10].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[12], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[12].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[13], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[13].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[15], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[15].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[16], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[16].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[17], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[17].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[32], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[32].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[33], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[33].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[34], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[34].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[35], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[35].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[36], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[36].VEC->H_Plugins[1];
	FLR->RealIntensity = (1 - (TempMatrix[7]/200))*1.00;



//---- Moving watermap's offset...
	VEC_Material *M = &SCN->Objects[55].VEC->H_Materials[1];
	byte *P = (byte*)Tmp1;

	int s = 10000;
	float t = (float)(InFrame - ((int)(InFrame/s))*s)/s;

	P += ((int)(t*M->M_MapHeigth*0.5))*80*4;
	//P += (byte)Tmp1;
	M->M_MapOffset = P;


//---- Moving waterfalls' mapoffset...
	M = &SCN->Objects[5].VEC->H_Materials[1];
	P = (byte*)Tmp2;
	s = 2000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.5))*100*2;
	M->M_MapOffset = P;

	M = &SCN->Objects[7].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 1500;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.5))*100*4;
	M->M_MapOffset = P;

	M = &SCN->Objects[44].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 2000;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.5))*100*4;
	M->M_MapOffset = P;

	M = &SCN->Objects[45].VEC->H_Materials[1];
	P = (byte*)Tmp3;
	s = 1500;
	t = (float)(InFrame - ((int)(InFrame/s))*s)/s;
	P += ((int)(t*M->M_MapHeigth*0.5))*100*4;
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

	SCN_GetObjectMatrix(InFrame, &SCN->Objects[67], TempMatrix);
	FLR = (FLR_Data*)SCN->Objects[67].VEC->H_Plugins[1];
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


}//WaterFallThingyPlugin

