/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCENE.CPP
³Author       : Cyberfish/TLS
³Last update  : 25.01.1999
³Action       : Pluginroutine for setting up all scene-matrixes...
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\vectors\scn_run.h"
#include <math.h>
#include <mem.h>
#include <stdio.h>

void SetRotationMatrix(float *Matrix, float XAng, float YAng, float ZAng);
void ZeroMatrix(float *Matrix);
void MulMatrix(const float *MatrixA, const float *MatrixB, float *MatrixC);
void RotatePoint(float X0,
		 float Y0,
		 float Z0,
		 float XAng,
		 float YAng,
		 float ZAng,
		 float &X1,
		 float &Y1,
		 float &Z1);
float GetAngle(float A, float B);

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LightPlugin01
³Last update  : 28.02.1998
³Action       : Plugin for setting up light-vector
³
³Input variables: SYS_POffs, SYS_PCaller
³
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SceneLightPlugin(void)
{
	Light *LightP = (Light*)SYS_PCaller;

	LightP->L_Falloff = LightP->L_Intensity/LightP->L_MaxRange;

	switch ( LightP->L_Type )
	{
		case 1:	LightP->L_Pos[0] = -LightP->L_Matrix[ 2];
			LightP->L_Pos[1] = -LightP->L_Matrix[ 6];
			LightP->L_Pos[2] = -LightP->L_Matrix[10];
			break;

		case 2: LightP->L_Pos[0] = LightP->L_Matrix[3];
			LightP->L_Pos[1] = LightP->L_Matrix[7];
			LightP->L_Pos[2] = LightP->L_Matrix[11];

		case 3: /*LightP->L_Pos[0] = LightP->L_Matrix[3];
			LightP->L_Pos[1] = LightP->L_Matrix[7];
			LightP->L_Pos[2] = LightP->L_Matrix[11];*/
			break;



	}//switch

}//SceneLightPlugin




void SCN_Motion6(float InFrame, int NrKeyframes, SCN_Keyframe6 *Keyframes, float *Vector)
{

	int K1, K2;
	int C1, C2;
	float h1, h2, h3, h4;
	float t, t2, t3;

	float dd0a, dd0b, ds1a, ds1b;
	float adj0, adj1;
	float d10_XPos, d10_YPos, d10_ZPos, d10_XAng, d10_YAng, d10_ZAng;
	float dd0_XPos, dd0_YPos, dd0_ZPos, dd0_XAng, dd0_YAng, dd0_ZAng;
	float ds1_XPos, ds1_YPos, ds1_ZPos, ds1_XAng, ds1_YAng, ds1_ZAng;

//---- Find object's position and angles...
	if ( NrKeyframes > 1 )
	{
	//---- Find key-segment...
		K2 = 0;
		C1 = NrKeyframes - 1;
		for ( C2 = 0; C2 < C1; C2++ )
			if ( Keyframes[C2].Frame <= InFrame ) K2++;
		K1 = K2 - 1;

	//---- Calculating t = (T - T0)/(T1 - T0)
		t = 	(float)
			(InFrame - (float)Keyframes[K1].Frame)
			/(Keyframes[K2].Frame - Keyframes[K1].Frame);

		d10_XPos = Keyframes[K2].XPos - Keyframes[K1].XPos;
		d10_YPos = Keyframes[K2].YPos - Keyframes[K1].YPos;
		d10_ZPos = Keyframes[K2].ZPos - Keyframes[K1].ZPos;
		d10_XAng = Keyframes[K2].XAng - Keyframes[K1].XAng;
		d10_YAng = Keyframes[K2].YAng - Keyframes[K1].YAng;
		d10_ZAng = Keyframes[K2].ZAng - Keyframes[K1].ZAng;

		if ( Keyframes[K2].Linear == 0 )
		{

			t2 = t*t;
			t3 = t2*t;

		//---- Calculating some magic stuff...
			h1 = 1.0 - (3*t2 - 2*t3);
			h2 = 3*t2 - 2*t3;
			h3 = t3 - 2*t2 + t;
			h4 = t3 - t2;

			dd0a = (1 - Keyframes[K1].Tension)*(1 + Keyframes[K1].Continuity)*(1 + Keyframes[K1].Bias);
			dd0b = (1 - Keyframes[K1].Tension)*(1 - Keyframes[K1].Continuity)*(1 - Keyframes[K1].Bias);
			ds1a = (1 - Keyframes[K2].Tension)*(1 - Keyframes[K2].Continuity)*(1 + Keyframes[K2].Bias);
			ds1b = (1 - Keyframes[K2].Tension)*(1 + Keyframes[K2].Continuity)*(1 - Keyframes[K2].Bias);

			if ( K1 != 0 ) adj0 = (float)(Keyframes[K2].Frame - Keyframes[K1].Frame)/(Keyframes[K2].Frame - Keyframes[K1 - 1].Frame);
			if ( K2 != (NrKeyframes - 1) ) adj1 = (float)(Keyframes[K2].Frame - Keyframes[K1].Frame)/(Keyframes[K2+1].Frame - Keyframes[K1].Frame);


		//---- Calculating motion-values...

			if ( K1 == 0 )
			{
				dd0_XPos = 0.5*(dd0a + dd0b)*d10_XPos;
				dd0_YPos = 0.5*(dd0a + dd0b)*d10_YPos;
				dd0_ZPos = 0.5*(dd0a + dd0b)*d10_ZPos;
				dd0_XAng = 0.5*(dd0a + dd0b)*d10_XAng;
				dd0_YAng = 0.5*(dd0a + dd0b)*d10_YAng;
				dd0_ZAng = 0.5*(dd0a + dd0b)*d10_ZAng;
			}//if
			else
			{
				dd0_XPos = adj0*( dd0a*(Keyframes[K1].XPos - Keyframes[K1 - 1].XPos) + dd0b*d10_XPos );
				dd0_YPos = adj0*( dd0a*(Keyframes[K1].YPos - Keyframes[K1 - 1].YPos) + dd0b*d10_YPos );
				dd0_ZPos = adj0*( dd0a*(Keyframes[K1].ZPos - Keyframes[K1 - 1].ZPos) + dd0b*d10_ZPos );
				dd0_XAng = adj0*( dd0a*(Keyframes[K1].XAng - Keyframes[K1 - 1].XAng) + dd0b*d10_XAng );
				dd0_YAng = adj0*( dd0a*(Keyframes[K1].YAng - Keyframes[K1 - 1].YAng) + dd0b*d10_YAng );
				dd0_ZAng = adj0*( dd0a*(Keyframes[K1].ZAng - Keyframes[K1 - 1].ZAng) + dd0b*d10_ZAng );
			}//else

			if ( K2 == ( NrKeyframes - 1) )
			{
				ds1_XPos = 0.5*(ds1a + ds1b)*d10_XPos;
				ds1_YPos = 0.5*(ds1a + ds1b)*d10_YPos;
				ds1_ZPos = 0.5*(ds1a + ds1b)*d10_ZPos;
				ds1_XAng = 0.5*(ds1a + ds1b)*d10_XAng;
				ds1_YAng = 0.5*(ds1a + ds1b)*d10_YAng;
				ds1_ZAng = 0.5*(ds1a + ds1b)*d10_ZAng;
			}//if
			else
			{
				ds1_XPos = adj1*( ds1b*(Keyframes[K2 + 1].XPos - Keyframes[K2].XPos) + ds1a*d10_XPos );
				ds1_YPos = adj1*( ds1b*(Keyframes[K2 + 1].YPos - Keyframes[K2].YPos) + ds1a*d10_YPos );
				ds1_ZPos = adj1*( ds1b*(Keyframes[K2 + 1].ZPos - Keyframes[K2].ZPos) + ds1a*d10_ZPos );
				ds1_XAng = adj1*( ds1b*(Keyframes[K2 + 1].XAng - Keyframes[K2].XAng) + ds1a*d10_XAng );
				ds1_YAng = adj1*( ds1b*(Keyframes[K2 + 1].YAng - Keyframes[K2].YAng) + ds1a*d10_YAng );
				ds1_ZAng = adj1*( ds1b*(Keyframes[K2 + 1].ZAng - Keyframes[K2].ZAng) + ds1a*d10_ZAng );
			}//else

			Vector[0] = Keyframes[K1].XPos*h1 + Keyframes[K2].XPos*h2 + dd0_XPos*h3 + ds1_XPos*h4;
			Vector[1] = Keyframes[K1].YPos*h1 + Keyframes[K2].YPos*h2 + dd0_YPos*h3 + ds1_YPos*h4;
			Vector[2] = Keyframes[K1].ZPos*h1 + Keyframes[K2].ZPos*h2 + dd0_ZPos*h3 + ds1_ZPos*h4;
			Vector[3] = Keyframes[K1].XAng*h1 + Keyframes[K2].XAng*h2 + dd0_XAng*h3 + ds1_XAng*h4;
			Vector[4] = Keyframes[K1].YAng*h1 + Keyframes[K2].YAng*h2 + dd0_YAng*h3 + ds1_YAng*h4;
			Vector[5] = Keyframes[K1].ZAng*h1 + Keyframes[K2].ZAng*h2 + dd0_ZAng*h3 + ds1_ZAng*h4;
		}//if
		else
		{
			Vector[0] = Keyframes[K1].XPos + d10_XPos*t;
			Vector[1] = Keyframes[K1].YPos + d10_YPos*t;
			Vector[2] = Keyframes[K1].ZPos + d10_ZPos*t;
			Vector[3] = Keyframes[K1].XAng + d10_XAng*t;
			Vector[4] = Keyframes[K1].YAng + d10_YAng*t;
			Vector[5] = Keyframes[K1].ZAng + d10_ZAng*t;
		}//if


	}//if
	else
	{
		Vector[0] = Keyframes[0].XPos;
		Vector[1] = Keyframes[0].YPos;
		Vector[2] = Keyframes[0].ZPos;
		Vector[3] = Keyframes[0].XAng;
		Vector[4] = Keyframes[0].YAng;
		Vector[5] = Keyframes[0].ZAng;
	}//else if only one keyframe...

}//SCN_Motion6


void SCN_Motion1(float InFrame, int NrKeyframes, SCN_Keyframe1 *Keyframes, float *Value)
{
	int K1, K2;
	int C1, C2;
	float h1, h2, h3, h4;
	float t, t2, t3;

	float dd0a, dd0b, ds1a, ds1b;
	float adj0, adj1;
	float d10;
	float dd0;
	float ds1;



//---- Find object's position and angles...
	if ( NrKeyframes > 1 )
	{
	//---- Find key-segment...
		K2 = 0;
		C1 = NrKeyframes - 1;
		for ( C2 = 0; C2 < C1; C2++ )
			if ( Keyframes[C2].Frame <= InFrame ) K2++;
		K1 = K2 - 1;


	//---- Calculating t = (T - T0)/(T1 - T0)
		t = 	(float)
			(InFrame - (float)Keyframes[K1].Frame)
			/(Keyframes[K2].Frame - Keyframes[K1].Frame);

		d10 = Keyframes[K2].Value - Keyframes[K1].Value;

		if ( Keyframes[K2].Linear == 0 )
		{
			t2 = t*t;
			t3 = t2*t;

		//---- Calculating some magic stuff...
			h1 = 1.0 - (3*t2 - 2*t3);
			h2 = 3*t2 - 2*t3;
			h3 = t3 - 2*t2 + t;
			h4 = t3 - t2;

			dd0a = (1 - Keyframes[K1].Tension)*(1 + Keyframes[K1].Continuity)*(1 + Keyframes[K1].Bias);
			dd0b = (1 - Keyframes[K1].Tension)*(1 - Keyframes[K1].Continuity)*(1 - Keyframes[K1].Bias);
			ds1a = (1 - Keyframes[K2].Tension)*(1 - Keyframes[K2].Continuity)*(1 + Keyframes[K2].Bias);
			ds1b = (1 - Keyframes[K2].Tension)*(1 + Keyframes[K2].Continuity)*(1 - Keyframes[K2].Bias);

			if ( K1 != 0 ) adj0 = (float)(Keyframes[K2].Frame - Keyframes[K1].Frame)/(Keyframes[K2].Frame - Keyframes[K1 - 1].Frame);
			if ( K2 != (NrKeyframes - 1) ) adj1 = (float)(Keyframes[K2].Frame - Keyframes[K1].Frame)/(Keyframes[K2+1].Frame - Keyframes[K1].Frame);


		//---- Calculating motion-values...


			if ( K1 == 0 ) dd0 = 0.5*(dd0a + dd0b)*d10;
			else dd0 = adj0*( dd0a*(Keyframes[K1].Value - Keyframes[K1 - 1].Value) + dd0b*d10 );

			if ( K2 == ( NrKeyframes - 1) ) ds1 = 0.5*(ds1a + ds1b)*d10;
			else ds1 = adj1*( ds1b*(Keyframes[K2 + 1].Value - Keyframes[K2].Value) + ds1a*d10);

			*Value = Keyframes[K1].Value*h1 + Keyframes[K2].Value*h2 + dd0*h3 + ds1*h4;
		}//if
		else
			*Value = Keyframes[K1].Value + d10*t;


	}//if
	else *Value = Keyframes[0].Value;

}//SCN_Motion1



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : GetObjectMatrix
³Last update  : 06.03.1998
³Action       : Routine for getting a matrix for a object at frame t
³
³Input variables: Frame, Objectdata, output-matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SCN_GetObjectMatrix(float InFrame, SCN_Object *Object, float *Matrix)
{
	if ( Object->CacheTime1 == InFrame )
	{
		memcpy(Matrix, Object->CacheMatrix1, 12*4);
		return;
	}//if

	if ( Object->CacheTime2 == InFrame )
	{
		memcpy(Matrix, Object->CacheMatrix2, 12*4);
		return;
	}//if


	int C1, C2;
	float XPos, YPos, ZPos, XAng, YAng, ZAng;
	float Vector[6];
	float Frame = InFrame;

	float PivotMatrix[12];
	float RotationMatrix[12];
	float UniverseMatrix[12];
	float ParentMatrix[12];
	float FinalMatrix[12];
	float TempMatrixA[12];
	float TempMatrixB[12];
	ZeroMatrix(ParentMatrix);


//---- Checking EndBehavior
	int MaxFrame = Object->Keyframes[Object->NrKeyframes - 1].Frame;
	if ( Frame > MaxFrame )
	{
		switch ( Object->EndBehavior )
		{
			case 0 : break;
			case 1 : Frame = MaxFrame;
			   	 break;
			case 2 : Frame = Frame - MaxFrame*((int)(Frame/MaxFrame));
				 break;
		}//switch
	}//if

//---- Translate around pivotpoint...
	ZeroMatrix(PivotMatrix);
	PivotMatrix[ 3] = -Object->XPivot;
	PivotMatrix[ 7] = -Object->YPivot;
	PivotMatrix[11] = -Object->ZPivot;

	SCN_Motion6(Frame, Object->NrKeyframes, Object->Keyframes, Vector);
	XPos = Vector[0];
	YPos = Vector[1];
	ZPos = Vector[2];
	XAng = Vector[3];
	YAng = Vector[4];
	ZAng = Vector[5];

//---- TargetObject...
	if ( Object->TargetObject != NULL )
	{

	//---- Getting object's "original" orientation matrix...
		int PathAlignment = Object->PathAlignment;
		Object->PathAlignment = 0;

		SCN_Object *TargetObject = Object->TargetObject;
		Object->TargetObject = NULL;

		ZeroMatrix(TempMatrixA);
		SCN_GetObjectMatrix(InFrame, Object, TempMatrixA);

		Object->PathAlignment = PathAlignment;
		Object->TargetObject = TargetObject;


	//---- Getting TargetObject's orientation matrix...
		ZeroMatrix(TempMatrixB);
		SCN_GetObjectMatrix(InFrame, Object->TargetObject, TempMatrixB);


	//---- Calculating camera's angles...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);


	//---- Setting position according to parentobject...
		ZeroMatrix(ParentMatrix);
		if ( Object->ParentObject != NULL )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}//if

	}//if
	else
//---- Pathalignment...
	if ( Object->PathAlignment != 0 )
	{

	//---- Getting current orientationmatrix...
		float PathAlignment = Object->PathAlignment;
		Object->PathAlignment = 0;

		ZeroMatrix(TempMatrixA);
		SCN_GetObjectMatrix(InFrame, Object, TempMatrixA);


	//---- Getting orientationmatrix in PathAlignment frames...
		ZeroMatrix(TempMatrixB);
		SCN_GetObjectMatrix(InFrame + PathAlignment, Object, TempMatrixB);


	//---- Calculating camera's angle...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);

		Object->PathAlignment = PathAlignment;

	//---- Fixing parentobject-stuff...
		ZeroMatrix(ParentMatrix);
		if ( Object->ParentObject != 0 )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}
	}//if PathAlignment

	if ( (Object->PathAlignment == 0) && (Object->TargetObject == NULL) )
	{
	//---- Matrix from Parentobject
		ZeroMatrix(ParentMatrix);
		if ( Object->ParentObject != 0 )
			SCN_GetObjectMatrix(InFrame, Object->ParentObject, ParentMatrix);
	}//if


	ZeroMatrix(RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, 0, ZAng);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, XAng, 0, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, YAng, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);


//---- Translation back to universe...
	ZeroMatrix(UniverseMatrix);
	UniverseMatrix[ 3] = XPos;
	UniverseMatrix[ 7] = YPos;
	UniverseMatrix[11] = ZPos;

	ZeroMatrix(FinalMatrix);
	MulMatrix(PivotMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(RotationMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(UniverseMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(ParentMatrix, FinalMatrix, FinalMatrix);

	if ( Object->CacheTime1 < Object->CacheTime2 )
	{
		Object->CacheTime1 = InFrame;
		memcpy(Object->CacheMatrix1, FinalMatrix, 12*4);
	}
	else
	{
		Object->CacheTime2 = InFrame;
		memcpy(Object->CacheMatrix2, FinalMatrix, 12*4);
	}

	memcpy(Matrix, FinalMatrix, 12*4);
}//GetObjectMatrix



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_GetLightMatrix
³Last update  : 06.03.1998
³Action       : Routine for getting a matrix for a object at frame t
³
³Input variables: Frame, Objectdata, output-matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SCN_GetLightMatrix(float InFrame, SCN_Light *Light, float *Matrix)
{
	int C1, C2;
	float XPos, YPos, ZPos, XAng, YAng, ZAng;
	float Vector[6];
	float Frame = InFrame;

	float PivotMatrix[12];
	float RotationMatrix[12];
	float UniverseMatrix[12];
	float ParentMatrix[12];
	float FinalMatrix[12];
	float TempMatrixA[12];
	float TempMatrixB[12];
	ZeroMatrix(ParentMatrix);

//---- Checking EndBehavior
	int MaxFrame = Light->Keyframes[Light->NrKeyframes - 1].Frame;
	if ( Frame > MaxFrame )
	{
		switch ( Light->EndBehavior )
		{
			case 0 : break;
			case 1 : Frame = MaxFrame;
			   	 break;
			case 2 : Frame = Frame - MaxFrame*((int)(Frame/MaxFrame));
				 break;
		}//switch
	}//if

	SCN_Motion6(Frame, Light->NrKeyframes, Light->Keyframes, Vector);
	XPos = Vector[0];
	YPos = Vector[1];
	ZPos = Vector[2];
	XAng = Vector[3];
	YAng = Vector[4];
	ZAng = Vector[5];


//---- TargetObject...
	if ( Light->TargetObject != NULL )
	{

	//---- Getting camera's "original" orientation matrix...
		int PathAlignment = Light->PathAlignment;
		Light->PathAlignment = 0;

		SCN_Object *TargetObject = Light->TargetObject;
		Light->TargetObject = NULL;

		ZeroMatrix(TempMatrixA);
		SCN_GetLightMatrix(InFrame, Light, TempMatrixA);

		Light->PathAlignment = PathAlignment;
		Light->TargetObject = TargetObject;


	//---- Getting TargetObject's orientation matrix...
		ZeroMatrix(TempMatrixB);
		SCN_GetObjectMatrix(InFrame, Light->TargetObject, TempMatrixB);


	//---- Calculating camera's angles...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);


	//---- Setting position according to parentobject...
		ZeroMatrix(ParentMatrix);
		if ( Light->ParentObject != NULL )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}//if

	}//if
	else
//---- Pathalignment...
	if ( Light->PathAlignment != 0 )
	{

	//---- Getting current orientationmatrix...
		float PathAlignment = Light->PathAlignment;
		Light->PathAlignment = 0;

		ZeroMatrix(TempMatrixA);
		SCN_GetLightMatrix(InFrame, Light, TempMatrixA);


	//---- Getting orientationmatrix in PathAlignment frames...
		ZeroMatrix(TempMatrixB);
		SCN_GetLightMatrix(InFrame + PathAlignment, Light, TempMatrixB);


	//---- Calculating camera's angle...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);

		Light->PathAlignment = PathAlignment;

	//---- Fixing parentobject-stuff...
		ZeroMatrix(ParentMatrix);
		if ( Light->ParentObject != 0 )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}
	}//if PathAlignment

	if ( (Light->PathAlignment == 0) && (Light->TargetObject == NULL) )
	{

	//---- Matrix from Parentobject
		ZeroMatrix(ParentMatrix);
		if ( Light->ParentObject != 0 )
			SCN_GetObjectMatrix(InFrame, Light->ParentObject, ParentMatrix);
	}//if


	ZeroMatrix(RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, 0, ZAng);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, XAng, 0, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, YAng, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);


//---- Translation back to universe...
	ZeroMatrix(UniverseMatrix);
	UniverseMatrix[ 3] = XPos;
	UniverseMatrix[ 7] = YPos;
	UniverseMatrix[11] = ZPos;

	ZeroMatrix(FinalMatrix);
	MulMatrix(RotationMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(UniverseMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(ParentMatrix, FinalMatrix, FinalMatrix);

	memcpy(Matrix, FinalMatrix, 12*4);
}//SCN_GetLightMatrix



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : GetCameraMatrix
³Last update  : 01.10.1998
³Action       : Routine for getting a matrix for camera at frame t
³
³Input variables: Frame, Cameradata, output-matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SCN_GetCameraMatrix(float InFrame, SCN_Camera *Camera, float *Matrix)
{
	float Vector[6];
	float XPos, YPos, ZPos, XAng, YAng, ZAng;

	float PivotMatrix[12];
	float RotationMatrix[12];
	float UniverseMatrix[12];
	float ParentMatrix[12];
	float FinalMatrix[12];
	float TempMatrixA[12];
	float TempMatrixB[12];

	ZeroMatrix(ParentMatrix);

	float Frame = InFrame;

//---- Checking EndBehavior
	int MaxFrame = Camera->Keyframes[Camera->NrKeyframes - 1].Frame;
	if ( Frame > MaxFrame )
	{
		switch ( Camera->EndBehavior )
		{
			case 0 : break;
			case 1 : Frame = MaxFrame;
			   	 break;
			case 2 : Frame = Frame - MaxFrame*((int)(Frame/MaxFrame));
				 break;
		}//switch
	}//if


	SCN_Motion6(Frame, Camera->NrKeyframes, Camera->Keyframes, Vector);
	XPos = Vector[0];
	YPos = Vector[1];
	ZPos = Vector[2];
	XAng = Vector[3];
	YAng = Vector[4];
	ZAng = Vector[5];


//---- TargetObject...
	if ( Camera->TargetObject != NULL )
	{

  	//---- Getting camera's world-matrix...(creating tmp-object)
		SCN_Object TmpObj;
		TmpObj.EndBehavior 	= Camera->EndBehavior;
		TmpObj.NrKeyframes 	= Camera->NrKeyframes;
		TmpObj.Keyframes 	= Camera->Keyframes;
		TmpObj.ParentObject 	= Camera->ParentObject;

		ZeroMatrix(TempMatrixA);
		SCN_GetObjectMatrix(InFrame, &TmpObj, TempMatrixA);

		TmpObj.Keyframes 	= NULL;
		TmpObj.ParentObject 	= NULL;

	//---- Getting TargetObject's orientation matrix...
		ZeroMatrix(TempMatrixB);
		SCN_GetObjectMatrix(InFrame, Camera->TargetObject, TempMatrixB);

	//---- Calculating camera's angles...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);



	//---- Setting position according to parentobject...
		ZeroMatrix(ParentMatrix);
		if ( Camera->ParentObject != NULL )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}//if

	}//if
	else
//---- Pathalignment...
	if ( Camera->PathAlignment != 0 )
	{

  	//---- Getting camera's world-matrix...(creating tmp-object)
		SCN_Object TmpObj;
		TmpObj.EndBehavior 	= Camera->EndBehavior;
		TmpObj.NrKeyframes 	= Camera->NrKeyframes;
		TmpObj.Keyframes 	= Camera->Keyframes;
		TmpObj.ParentObject 	= Camera->ParentObject;

		ZeroMatrix(TempMatrixA);
		SCN_GetObjectMatrix(InFrame, &TmpObj, TempMatrixA);


	//---- Getting orientationmatrix in PathAlignment frames...
		ZeroMatrix(TempMatrixB);
		SCN_GetObjectMatrix(InFrame + Camera->PathAlignment, &TmpObj, TempMatrixB);

		TmpObj.Keyframes 	= NULL;
		TmpObj.ParentObject 	= NULL;

	//---- Calculating camera's angle...
		float XPos0, YPos0, ZPos0;
		float XPos1, YPos1, ZPos1;
		float XPos2, YPos2, ZPos2;

		XPos1 = TempMatrixA[ 3];
		YPos1 = TempMatrixA[ 7];
		ZPos1 = TempMatrixA[11];

		XPos2 = TempMatrixB[ 3];
		YPos2 = TempMatrixB[ 7];
		ZPos2 = TempMatrixB[11];

		XPos0 = XPos2 - XPos1;
		YPos0 = YPos2 - YPos1;
		ZPos0 = ZPos2 - ZPos1;

		YAng = GetAngle(XPos0, ZPos0);
		RotatePoint(XPos0, YPos0, ZPos0, 0, -YAng, 0, XPos0, YPos0, ZPos0);
		XAng = -GetAngle(YPos0, ZPos0);


	//---- Fixing parentobject-stuff...
		ZeroMatrix(ParentMatrix);
		if ( Camera->ParentObject != 0 )
		{
			XPos = XPos1;
			YPos = YPos1;
			ZPos = ZPos1;
		}
	}//if PathAlignment

	if ( (Camera->PathAlignment == 0) && (Camera->TargetObject == 0) )
	{
	//---- Matrix from Parentobject
		ZeroMatrix(ParentMatrix);
		if ( Camera->ParentObject != 0 )
			SCN_GetObjectMatrix(InFrame, Camera->ParentObject, ParentMatrix);

	}


	ZeroMatrix(RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, 0, ZAng);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, XAng, 0, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);
	ZeroMatrix(TempMatrixA);
	SetRotationMatrix(TempMatrixA, 0, YAng, 0);
	MulMatrix(TempMatrixA, RotationMatrix, RotationMatrix);


//---- Translation back to universe...
	ZeroMatrix(UniverseMatrix);
	UniverseMatrix[ 3] = XPos;
	UniverseMatrix[ 7] = YPos;
	UniverseMatrix[11] = ZPos;

	ZeroMatrix(FinalMatrix);
	MulMatrix(RotationMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(UniverseMatrix, FinalMatrix, FinalMatrix);
	MulMatrix(ParentMatrix, FinalMatrix, FinalMatrix);



//-------------- CONVERTING CAMERA'S MATRIX TO WORLD-MATRIX... ---------------

	ZeroMatrix(PivotMatrix);
	PivotMatrix[ 3] = -FinalMatrix[ 3];
	PivotMatrix[ 7] = -FinalMatrix[ 7];
	PivotMatrix[11] = -FinalMatrix[11];

//---- Rotating a cameravector of [0, 0, 1] to find Y/X-angle
	XPos = FinalMatrix[ 2];
	YPos = FinalMatrix[ 6];
	ZPos = FinalMatrix[10];

	YAng = -GetAngle(XPos, ZPos);

	RotatePoint(XPos, YPos, ZPos, 0, YAng, 0, XPos, YPos, ZPos);
	XAng =  GetAngle(YPos, ZPos);
	RotatePoint(XPos, YPos, ZPos, XAng, 0, 0, XPos, YPos, ZPos);

//---- Rotating a cameravector of [1, 0, 0] to find Z-angle
	XPos = FinalMatrix[ 0];
	YPos = FinalMatrix[ 4];
	ZPos = FinalMatrix[ 8];
	RotatePoint(XPos, YPos, ZPos, 0, YAng, 0, XPos, YPos, ZPos);
	RotatePoint(XPos, YPos, ZPos, XAng, 0, 0, XPos, YPos, ZPos);
	ZAng = -GetAngle(YPos, XPos);

	ZeroMatrix(RotationMatrix);
	SetRotationMatrix(RotationMatrix, 0, YAng, 0);
	MulMatrix(RotationMatrix, PivotMatrix, FinalMatrix);
	ZeroMatrix(RotationMatrix);
	SetRotationMatrix(RotationMatrix, XAng, 0, 0);
	MulMatrix(RotationMatrix, FinalMatrix, FinalMatrix);
	ZeroMatrix(RotationMatrix);
	SetRotationMatrix(RotationMatrix, 0, 0, ZAng);
	MulMatrix(RotationMatrix, FinalMatrix, FinalMatrix);

	memcpy(Matrix, FinalMatrix, 12*4);
}//void GetCameraMatrix


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SceneEnvelope
³Last update  : 27.02.1998
³Action       : Routine for running an envelope
³
³Input variables: Frame, Envelope
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SCN_GetEnvelope(float InFrame, SCN_Envelope *Env)
{
	float Value;
	float Frame = InFrame;

//---- Checking EndBehavior
	int MaxFrame = Env->Keyframes[Env->NrKeyframes - 1].Frame;
	if ( Frame > MaxFrame )
	{
		switch ( Env->EndBehavior )
		{
			case 0 : break;
			case 1 : Frame = MaxFrame;
			   	 break;
			case 2 : Frame = Frame - MaxFrame*((int)(Frame/MaxFrame));
				 break;
		}//switch
	}//if

	SCN_Motion1(Frame, Env->NrKeyframes, Env->Keyframes, &Value);

	*Env->Value = Value;
}//SceneEnvelope



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : ScenePlugin
³Last update  : 25.01.1999
³Action       : Plugin for setting up all object/light/camera-matrixes/envelopes
³
³Input variables: SYS_POffs, SYS_PCaller
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void SCN_RUN(void)
{
	SCN_Header *SCN = (SCN_Header*)SYS_OOffs;

	int CurrentTime = SYS_TimerTick;

	float Seconds = (float)(CurrentTime - SCN->StartTime)/(float)SYS_TicksPerSec;
	float InFrame = SCN->FirstFrame + Seconds*SCN->FramesPerSecond;

	SCN->CurrentFrame = InFrame;

	int C1, C2;

	float TempMatrixA[12];
	float TempMatrixB[12];
	float TempMatrixC[12];


//---- Setting up internal/external matrices for all objects...
	for ( C1 = 0; C1 < SCN->NrObjects; C1++ )
		SCN_GetObjectMatrix(InFrame, &SCN->Objects[C1], SCN->Objects[C1].Matrix);

	SCN_GetCameraMatrix(InFrame, SCN->Camera, SCN->Camera->Matrix);


//---- Calculating matrixes for lights...
	for ( C1 = 0; C1 < SCN->NrLights; C1++ )
	{
		SCN_GetLightMatrix(InFrame, &SCN->Lights[C1], SCN->Lights[C1].Matrix);

		MulMatrix(SCN->Camera->Matrix, SCN->Lights[C1].Matrix, SCN->Lights[C1].LGT->L_Matrix);
	}//for


//---- Translating objects around camera...
	for ( C1 = 0; C1 < SCN->NrObjects; C1++ )
		MulMatrix(SCN->Camera->Matrix, SCN->Objects[C1].Matrix, SCN->Objects[C1].VEC->H_Matrix);

//---- Running envelopes...
	for ( C1 = 0; C1 < SCN->NrEnvelopes; C1++ )
		SCN_GetEnvelope(InFrame, &SCN->Envelopes[C1]);


//---- Setting up zoomfactor (perspective constants...)
	SYS_XPerspective = ((float)SYS_XRes/320)*120*SCN->Camera->ZoomFactor;
	SYS_YPerspective = ((float)SYS_YRes/240)*-120*SCN->Camera->ZoomFactor;

}//SCN_RUN



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SetRotationMatrix
³Last update  : 29.01.1998
³Action       : Sets up a 4x3 (actually 4x4) matrix with rotationformulas
³
³Input variables: Angles, output-matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void SetRotationMatrix(float *Matrix, float XAng, float YAng, float ZAng)
{
	float XSin, XCos, YSin, YCos, ZSin, ZCos;
	XSin = sin(XAng);
	XCos = cos(XAng);
	YSin = sin(YAng);
	YCos = cos(YAng);
	ZSin = sin(ZAng);
	ZCos = cos(ZAng);

	Matrix[ 0] = XSin*ZSin*YSin + ZCos*YCos;
	Matrix[ 1] = ZCos*XSin*YSin - ZSin*YCos;
	Matrix[ 2] = XCos*YSin;
	Matrix[ 4] = ZSin*XCos;
	Matrix[ 5] = ZCos*XCos;
	Matrix[ 6] = -XSin;
	Matrix[ 8] = XSin*ZSin*YCos - ZCos*YSin;
	Matrix[ 9] = ZCos*XSin*YCos + ZSin*YSin;
	Matrix[10] = XCos*YCos;
}//SetRotationMatrix


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : ZeroMatrix
³Last update  : 29.01.1998
³Action       : Zeroes out a 4x3-matrix...
³
³Input variables: matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void ZeroMatrix(float *Matrix)
{
	float const One = 1.0;
	__asm
	{
		pushad
		pushfd

		mov	edi, Matrix
		mov	ecx, 12
		xor	eax, eax
		rep	stosd

		mov	eax, One
		mov	edi, Matrix
		mov	d[edi], eax
		mov	d[edi + 20], eax
		mov	d[edi + 40], eax

		popfd
		popad
	}
}//ZeroMatrix

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : MulMatrix
³Last update  : 29.01.1998
³Action       : Multiplies two 4x3-matrixes
³
³Input variables: MatrixA, MatrixB, output-matrix
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void MulMatrix(const float *MatrixA, const float *MatrixB, float *MatrixC)
{
	float TMatrix[12];

	TMatrix[0] =   MatrixA[0] * MatrixB[ 0]
		     + MatrixA[1] * MatrixB[ 4]
		     + MatrixA[2] * MatrixB[ 8];

	TMatrix[1] =   MatrixA[0] * MatrixB[ 1]
		     + MatrixA[1] * MatrixB[ 5]
		     + MatrixA[2] * MatrixB[ 9];

	TMatrix[2] =   MatrixA[0] * MatrixB[ 2]
		     + MatrixA[1] * MatrixB[ 6]
		     + MatrixA[2] * MatrixB[10];

	TMatrix[3] =   MatrixA[0] * MatrixB[ 3]
		     + MatrixA[1] * MatrixB[ 7]
		     + MatrixA[2] * MatrixB[11]
		     + MatrixA[3];


	TMatrix[4] =   MatrixA[4] * MatrixB[ 0]
		     + MatrixA[5] * MatrixB[ 4]
		     + MatrixA[6] * MatrixB[ 8];

	TMatrix[5] =   MatrixA[4] * MatrixB[ 1]
		     + MatrixA[5] * MatrixB[ 5]
		     + MatrixA[6] * MatrixB[ 9];

	TMatrix[6] =   MatrixA[4] * MatrixB[ 2]
		     + MatrixA[5] * MatrixB[ 6]
		     + MatrixA[6] * MatrixB[10];

	TMatrix[7] =   MatrixA[4] * MatrixB[ 3]
		     + MatrixA[5] * MatrixB[ 7]
		     + MatrixA[6] * MatrixB[11]
		     + MatrixA[7];


	TMatrix[ 8]=   MatrixA[ 8] * MatrixB[ 0]
		     + MatrixA[ 9] * MatrixB[ 4]
		     + MatrixA[10] * MatrixB[ 8];

	TMatrix[ 9]=   MatrixA[ 8] * MatrixB[ 1]
		     + MatrixA[ 9] * MatrixB[ 5]
		     + MatrixA[10] * MatrixB[ 9];

	TMatrix[10]=   MatrixA[ 8] * MatrixB[ 2]
		     + MatrixA[ 9] * MatrixB[ 6]
		     + MatrixA[10] * MatrixB[10];

	TMatrix[11]=   MatrixA[ 8] * MatrixB[ 3]
		     + MatrixA[ 9] * MatrixB[ 7]
		     + MatrixA[10] * MatrixB[11]
		     + MatrixA[11];

	memcpy(MatrixC, TMatrix, 12*4);

}//MulMatrix




/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : RotatePoint
³Last update  : 29.01.1998
³Action       : Rotates a position
³
³Input variables: In-point, angles, out-point
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RotatePoint(float X0,
		 float Y0,
		 float Z0,
		 float XAng,
		 float YAng,
		 float ZAng,
		 float &X1,
		 float &Y1,
		 float &Z1)
{
	float XSin, XCos, YSin, YCos, ZSin, ZCos;
	XSin = sin(XAng);
	XCos = cos(XAng);
	YSin = sin(YAng);
	YCos = cos(YAng);
	ZSin = sin(ZAng);
	ZCos = cos(ZAng);

	float Matrix[9];

	Matrix[ 0] = XSin*ZSin*YSin + ZCos*YCos;
	Matrix[ 1] = ZCos*XSin*YSin - ZSin*YCos;
	Matrix[ 2] = XCos*YSin;
	Matrix[ 3] = ZSin*XCos;
	Matrix[ 4] = ZCos*XCos;
	Matrix[ 5] = -XSin;
	Matrix[ 6] = XSin*ZSin*YCos - ZCos*YSin;
	Matrix[ 7] = ZCos*XSin*YCos + ZSin*YSin;
	Matrix[ 8] = XCos*YCos;

	float XTemp = Matrix[0]*X0 + Matrix[1]*Y0 + Matrix[2]*Z0;
	float YTemp = Matrix[3]*X0 + Matrix[4]*Y0 + Matrix[5]*Z0;
	float ZTemp = Matrix[6]*X0 + Matrix[7]*Y0 + Matrix[8]*Z0;

	X1 = XTemp;
	Y1 = YTemp;
	Z1 = ZTemp;

}//RotatePoint


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ASSEMBLER ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : GetAngle
³Last update  : 29.01.1998
³Action       : Gets an angle out of two numbers (2d-coordinate)
³
³Input variables: A, B.
³
³Notes : Returns angle in radians.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float GetAngle(float A, float B)
{
	float Angle;
	if ( B != 0 )
		Angle = atan(fabs(A/B));
	else Angle = 0.5*Pi;

	if ( (A >= 0) && (B < 0) ) Angle = Pi - Angle;
	if ( (A < 0) && (B > 0) ) Angle = 2*Pi - Angle;
	if ( (A < 0) && (B <= 0) ) Angle += Pi;
	if ( (A == 0) && (B == 0) ) Angle = 0;

	return Angle;
}//GetAngle





