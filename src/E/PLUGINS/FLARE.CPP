/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLARE.CPP
³Author       : Cyberfish/TLS
³Last update  : 22.01.1999
³Action       : Loads and runs a flare.
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define Aquarium_ENGINE

#include <math.h>

#include "w:\general\sort.h"
#include "w:\plugins\flare.h"
#include "w:\general\var.h"
#include "w:\vectors\vec_run.h"
#include "w:\general\txt.h"
#include "w:\vectors\lights.h"


uint32 FLR_Priority = 240;

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLR_Data::FLR_Data
³ID           : FLR
³Last update  : 22.01.1999
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

FLR_Data::FLR_Data(void)
{
	Type		= 10;
	Priority	= 240;

	Scale		= 1.0;

	R		= 1.0;
	G 		= 1.0;
	B		= 1.0;

	Intensity	= 1.0;
	RealIntensity	= 1.0;

	MapWidth	= 0;
	MapHeigth	= 0;
	Map		= NULL;

}//FLR_Data

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLR_Data::~FLR_Data
³ID           : FLR
³Last update  : 22.01.1999
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FLR_Data::~FLR_Data(void)
{
	TXT_Unload(Map);
}//destructor



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLR_Setup
³ID           : FLR
³Last update  : 22.01.1999
³Action	      : Sets up a flare with the right map.
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

int FLR_Setup(FLR_Data *FLR, const char *MapFile)
{
//---- Loading map...
	int MapWidth, MapHeigth;
	TXT_Data *Tmp = TXT_Load(MapFile, 0);
	if ( !Tmp )
	{
		SYS_Error(1, "FLR_Setup: Error loading texture for map.\n");
		SYS_Error(3, "  ( ", MapFile, " )  \n");
		return 0;
	}
	FLR->Map = Tmp->Texture;

	FLR->MapWidth  = (float)(Tmp->Width - 1);
	FLR->MapHeigth = (float)(Tmp->Heigth - 1);

	return 1;
}//FLR_Setup


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLR_ObjectPlugin
³ID           : FLR
³Last update  : 22.01.1999
³Action	      : Lensflare pluginroutine for objects.
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FLR_ObjectPlugin(void)
{
	FLR_Data *Data = (FLR_Data*)SYS_POffs;
	VEC_Header *VEC = (VEC_Header*)SYS_PCaller;

//---- Z-value...
	if ( VEC->H_Matrix[11] < SYS_ZClip ) return;
	float InvZ = 1/VEC->H_Matrix[11];
	Data->SortValue = 0xFFFFFFFF - (int)((InvZ + 1)*SYS_ZBufFP);
	Data->SortValue = 0xFFFFFFFF - (int)((InvZ + 1)*SYS_ZBufFP);


//---- Setting position/vertices...
	float XMin = VEC->H_Matrix[3];
	XMin -= ((float)Data->MapWidth*0.5)*Data->Scale;
	XMin *= SYS_XPerspective;
	XMin *= InvZ;
	XMin += SYS_XCenter;

	float XMax = VEC->H_Matrix[3];
	XMax += ((float)Data->MapWidth*0.5)*Data->Scale;
	XMax *= SYS_XPerspective;
	XMax *= InvZ;
	XMax += SYS_XCenter;

	float YMin = VEC->H_Matrix[7];
	YMin += ((float)Data->MapHeigth*0.5)*Data->Scale;
	YMin *= SYS_YPerspective;
	YMin *= InvZ;
	YMin += SYS_YCenter;

	float YMax = VEC->H_Matrix[7];
	YMax -= ((float)Data->MapHeigth*0.5)*Data->Scale;
	YMax *= SYS_YPerspective;
	YMax *= InvZ;
	YMax += SYS_YCenter;


	if ( XMax < SYS_FPClipLeft ) return;
	if ( XMin > SYS_FPClipRight ) return;
	if ( YMax < SYS_FPClipTop ) return;
	if ( YMin > SYS_FPClipBottom ) return;

	//Upper left
	Data->XMin = XMin;
	Data->YMin = YMin;

	//Lower right
	Data->XMax = XMax;
	Data->YMax = YMax;


//---- Calculating intensity...
	float X = VEC->H_Matrix[3];
	X *= SYS_XPerspective;
	X *= InvZ;
	X += SYS_XCenter;
	Data->X = (int)X;

	float Y = VEC->H_Matrix[7];
	Y *= SYS_YPerspective;
	Y *= InvZ;
	Y += SYS_YCenter;
	Data->Y = (int)Y;

	Data->Z = (int)(InvZ*SYS_ZBufFP) + SYS_ZBufAdd;

	Data->Intensity = Data->RealIntensity;
//---- Calculating intensity (fade camera-angle)
	if ( Data->Flags & FLR_FlagFadeOffScreen )
	{
		float VectorX = VEC->H_Matrix[ 3];
		float VectorY = VEC->H_Matrix[ 7];
		float VectorZ = VEC->H_Matrix[11];

		float VectorLength = VectorX*VectorX + VectorY*VectorY + VectorZ*VectorZ;
		VectorLength = sqrt(VectorLength);

		float Cosine = (VectorZ/VectorLength);
		float Angle = acos(Cosine);
		Cosine = cos(Angle*2);

		Data->Intensity *= 2*Cosine*Cosine*Cosine*Cosine*Cosine;
		//Angle/(0.5*Pi)*1.0;


	}//if

//---- Calculating intensity "fade with light"
	if ( (Data->Flags & FLR_FlagFadeWithLight) && (VEC->H_Matrix[11] > 0) )
	{
		float CVectorX = VEC->H_Matrix[ 3];
		float CVectorY = VEC->H_Matrix[ 7];
		float CVectorZ = VEC->H_Matrix[11];

		float LVectorX = VEC->H_Matrix[ 2];
		float LVectorY = VEC->H_Matrix[ 6];
		float LVectorZ = VEC->H_Matrix[10];

		float DotProduct = LVectorX*CVectorX + LVectorY*CVectorY + LVectorZ*CVectorZ;
		if ( DotProduct < 0 )
		{
			float CVectorLength = CVectorX*CVectorX + CVectorY*CVectorY + CVectorZ*CVectorZ;
			CVectorLength = sqrt(CVectorLength);

			float Cosine = (DotProduct/CVectorLength);
			float Angle = acos(-Cosine);
			Cosine = cos(Angle);

			Data->Intensity *= Cosine*Cosine*Cosine;
		}//if
		else Data->Intensity = 0;

	}//if

//---- Inserting flare into sorting tables...
	int Radix = 255 - (Data->SortValue & 0xFF);

	Face *Tmp = (Face*)SRT_End1[Radix];
	Tmp->F_NextOffset = (int)Data;
	SRT_End1[Radix] = (int)Data;
	//Tmp->F_Priority = FLR_Priority;

}//FLR_ObjectPlugin




/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : FLR_LightPlugin
³ID           : FLR
³Last update  : 22.01.1999
³Action	      : Lensflare pluginroutine for lights.
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FLR_LightPlugin(void)
{
	FLR_Data *Data = (FLR_Data*)SYS_POffs;
	LGT_Header *LGT = (LGT_Header*)SYS_PCaller;

//---- Z-value...
	if ( LGT->L_Matrix[11] < SYS_ZClip ) return;
	float InvZ = 1/LGT->L_Matrix[11];
	Data->SortValue = 0xFFFFFFFF - (int)((InvZ + 1)*SYS_ZBufFP);
	Data->SortValue = 0xFFFFFFFF - (int)((InvZ + 1)*SYS_ZBufFP);


//---- Setting position/vertices...
	float XMin = LGT->L_Matrix[3];
	XMin -= ((float)Data->MapWidth*0.5)*Data->Scale;
	XMin *= SYS_XPerspective;
	XMin *= InvZ;
	XMin += SYS_XCenter;

	float XMax = LGT->L_Matrix[3];
	XMax += ((float)Data->MapWidth*0.5)*Data->Scale;
	XMax *= SYS_XPerspective;
	XMax *= InvZ;
	XMax += SYS_XCenter;

	float YMin = LGT->L_Matrix[7];
	YMin += ((float)Data->MapHeigth*0.5)*Data->Scale;
	YMin *= SYS_YPerspective;
	YMin *= InvZ;
	YMin += SYS_YCenter;

	float YMax = LGT->L_Matrix[7];
	YMax -= ((float)Data->MapHeigth*0.5)*Data->Scale;
	YMax *= SYS_YPerspective;
	YMax *= InvZ;
	YMax += SYS_YCenter;


	if ( XMax < SYS_FPClipLeft ) return;
	if ( XMin > SYS_FPClipRight ) return;
	if ( YMax < SYS_FPClipTop ) return;
	if ( YMin > SYS_FPClipBottom ) return;

	//Upper left
	Data->XMin = XMin;
	Data->YMin = YMin;

	//Lower right
	Data->XMax = XMax;
	Data->YMax = YMax;


//---- Calculating position
	float X = LGT->L_Matrix[3];
	X *= SYS_XPerspective;
	X *= InvZ;
	X += SYS_XCenter;
	Data->X = (int)X;

	float Y = LGT->L_Matrix[7];
	Y *= SYS_YPerspective;
	Y *= InvZ;
	Y += SYS_YCenter;
	Data->Y = (int)Y;

	Data->Z = (int)(InvZ*SYS_ZBufFP) + SYS_ZBufAdd;


//---- Calculating intensity "fade off screen"
	if ( Data->Flags & FLR_FlagFadeOffScreen )
	{
		float LVectorX = LGT->L_Matrix[ 3];
		float LVectorY = LGT->L_Matrix[ 7];
		float LVectorZ = LGT->L_Matrix[11];

		float VectorLength = LVectorX*LVectorX + LVectorY*LVectorY + LVectorZ*LVectorZ;
		VectorLength = sqrt(VectorLength);

		float Cosine = LVectorZ/VectorLength;
		float Angle = 2*Pi - acos(Cosine);

		Data->Intensity = Angle/(2*Pi)*3.0;

	}//if



//---- Inserting flare into sorting tables...
	int Radix = 255 - (Data->SortValue & 0xFF);

	Face *Tmp = (Face*)SRT_End1[Radix];
	Tmp->F_NextOffset = (int)Data;
	SRT_End1[Radix] = (int)Data;

}//FLR_LightPlugin
