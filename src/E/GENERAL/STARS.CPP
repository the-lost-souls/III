#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\general\stars.h"
#include <stdlib.h>
#include <mem.h>



STR_StarField::STR_StarField()
{
	STR_Type	= 28;
	memset(STR_Plugins, 0, 16*4);

	STR_NumStars 	= 0;
	STR_XMin	= -10;
	STR_YMin	= -10;
	STR_ZMin	= -10;
	STR_XMax	= 10;
	STR_YMax	= 10;
	STR_ZMax	= 10;

	STR_XSpeed 	= 0;
	STR_YSpeed	= 0;
	STR_ZSpeed	= 0;

	STR_SCN		= NULL;
	STR_StarTable	= NULL;

}//constructor



STR_StarField::~STR_StarField()
{
	delete [] STR_StarTable;
}//destructor



int STR_StarField::Init(int NumStars, SCN_Header *SCN)
{
//---- Setting up stars...
	STR_StarTable = new Star[NumStars];
	if ( !STR_StarTable )
	{
		SYS_Error(1, "STR_StarField::Init: Not enough memory for startable.\n");
		return 0;
	}//if

	STR_NumStars = NumStars;
	STR_SCN = SCN;

	srand(SYS_TimerTick);
	for ( int C = 0; C < NumStars; C++ )
	{
		STR_StarTable[C].X = ((float)rand()/RAND_MAX)*(STR_XMax-STR_XMin)-STR_XMax;
		STR_StarTable[C].Y = ((float)rand()/RAND_MAX)*(STR_YMax-STR_YMin)-STR_YMax;
		STR_StarTable[C].Z = ((float)rand()/RAND_MAX)*(STR_ZMax-STR_ZMin)-STR_ZMax;
	}//for

	return 1;
}//Init


void STR_StarField::SetRange(float XMin, float YMin, float ZMin,
			     float XMax, float YMax, float ZMax)
{
	STR_XMin = XMin;
	STR_YMin = YMin;
	STR_ZMin = ZMin;
	STR_XMax = XMax;
	STR_YMax = YMax;
	STR_ZMax = ZMax;
}//SetRange


void STR_StarField::SetSpeed(float XSpeed, float YSpeed, float ZSpeed)
{
	STR_XSpeed = XSpeed;
	STR_YSpeed = YSpeed;
	STR_ZSpeed = ZSpeed;
}//SetSpeed


static void STR_StarField::STR_Run(void)
{
	__asm int 3
	STR_StarField *STR = (STR_StarField*)SYS_OOffs;


//---- Getting camera matrix...
	float CameraMatrix[12];

	int CurrentTime = SYS_TimerTick;

	float Seconds = (float)(CurrentTime - STR->STR_SCN->StartTime)/(float)SYS_TicksPerSec;
	float InFrame = STR->STR_SCN->FirstFrame + Seconds*STR->STR_SCN->FramesPerSecond;

	float FinalMatrix[12];
	SCN_GetCameraMatrix(InFrame, STR->STR_SCN->Camera, FinalMatrix);


//---- Setting movement matrix...
/*	float SpeedMatrix[12];
	ZeroMatrix(SpeedMatrix);

/*	SpeedMatrix[ 3] = STR->STR_XSpeed*Seconds;
	SpeedMatrix[ 7] = STR->STR_YSpeed*Seconds;
	SpeedMatrix[11] = STR->STR_ZSpeed*Seconds;

	MulMatrix(SpeedMatrix, CameraMatrix, FinalMatrix);*/

//---- Calculating range...
	float TempMatrix[12];
	SCN_Object TempObj;
	TempObj.ParentObject 	= STR->STR_SCN->Camera->ParentObject;
	TempObj.TargetObject 	= STR->STR_SCN->Camera->TargetObject;
	TempObj.PathAlignment 	= STR->STR_SCN->Camera->PathAlignment;
	TempObj.NrKeyframes 	= STR->STR_SCN->Camera->NrKeyframes;
	TempObj.Keyframes 	= STR->STR_SCN->Camera->Keyframes;
	TempObj.EndBehavior 	= STR->STR_SCN->Camera->EndBehavior;

	SCN_GetObjectMatrix(InFrame, &TempObj, TempMatrix);

	TempObj.Keyframes = NULL;

	float XMin = STR->STR_XMin + TempMatrix[ 3];

	float YMin = STR->STR_YMin + TempMatrix[ 7];

	float ZMin = STR->STR_ZMin + TempMatrix[11];

	float XMax = STR->STR_XMax + TempMatrix[ 3];

	float YMax = STR->STR_YMax + TempMatrix[ 7];

	float ZMax = STR->STR_ZMax + TempMatrix[11];

	float XSpeed = STR->STR_XSpeed;
	float YSpeed = STR->STR_YSpeed;
	float ZSpeed = STR->STR_ZSpeed;


//---- Processing stars...
	int NumStars = STR->STR_NumStars;
	void *P = STR->STR_StarTable;
	float FPDump;
	__asm{
		pushad
		pushfd

		fldcw	FPU_Round

		mov	ecx, NumStars
		mov	esi, P

	StarLoop:



		fld	d[esi + 0]
		fadd	XSpeed
		fcom	XMax
		fstsw	ax
		sahf
		jb	NoXClip1
		fsub	XMax
		fadd	XMin
	NoXClip1:

		fcom	XMin
		fstsw	ax
		sahf
		jae     NoXClip2
		fsub	XMin
		fadd	XMax
	NoXClip2:
		fstp	d[esi + 0]

		fld	d[esi + 4]
		fadd	YSpeed
		fcom	YMax
		fstsw	ax
		sahf
		jb	NoYClip1
		fsub	YMax
		fadd	YMin
	NoYClip1:

		fcom	YMin
		fstsw	ax
		sahf
		jae     NoYClip2
		fsub	YMin
		fadd	YMax
	NoYClip2:
		fstp	d[esi + 4]

		fld	d[esi + 8]
		fadd	ZSpeed
		fcom	ZMax
		fstsw	ax
		sahf
		jb	NoZClip1
		fsub	ZMax
		fadd	ZMin
	NoZClip1:

		fcom	ZMin
		fstsw	ax
		sahf
		jae     NoZClip2
		fsub	ZMin
		fadd	ZMax
	NoZClip2:
		fstp	d[esi + 8]


						//Calculate X
		fld	d[esi + 0]
		fmul	FinalMatrix[0]
		fld	d[esi + 4]
		fmul	FinalMatrix[4]
		fld	d[esi + 8]
		fmul	FinalMatrix[8]

		faddp	st(1), st
		faddp	st(1), st
		fadd	FinalMatrix[12]
						//st(0) = X3D


						//Calculate Y
		fld	d[esi + 0]
		fmul	FinalMatrix[16]
		fld	d[esi + 4]
		fmul	FinalMatrix[20]
		fld	d[esi + 8]
		fmul	FinalMatrix[24]

		faddp	st(1), st
		faddp	st(1), st
		fadd	FinalMatrix[28]
						//st(0) = Y3D


						//Calculate Z
		fld	d[esi + 0]
		fmul	FinalMatrix[32]
		fld	d[esi + 4]
		fmul	FinalMatrix[36]
		fld	d[esi + 8]
		fmul	FinalMatrix[40]

		faddp	st(1), st
		faddp	st(1), st
		fadd	FinalMatrix[44]
						//st(0) = Z3D


		fcom	SYS_ZClip		//Behind camera?
		fstsw	ax
		sahf
		jb	FAR DumpStar
						//st(0) = Z3D

	//st0 = Z3D
	//st1 = Y3D
	//st2 = X3D

	//---- Project coordinates...
					//st0  st1  st2  st3  st4
					//Z3D  Y3D  X3D
		fld1
		fxch	st(1)
		fdivp   st(1), st	//1/Z  Y3D  X3D

		fmul	st(1), st(0)
		fmul	st(2), st(0)	//1/Z  Y/Z  X/Z

		fmul	SYS_ZBufFP

		fistp	FPDump		//Y/Z  X/Z
		mov	edx, FPDump	//edx = 1/Z fixedpoint
		add	edx, SYS_ZBufAdd

		fmul    SYS_YPerspective
		fadd	SYS_YCenter

		fcom	SYS_FPClipTop	//Testing upper side...
		fstsw	ax
		sahf
		jb	FAR DumpStar

		fcom	SYS_FPClipBottom//Testing bottom side...
		fstsw	ax
		sahf
		ja	FAR DumpStar

		fistp	FPDump
		mov	ebx, FPDump	//ebx = Y2D

					//X/Z
		fmul	SYS_XPerspective
		fadd	SYS_XCenter

		fcom	SYS_FPClipLeft	//Testing left side...
		fstsw	ax
		sahf
		jb	FAR DumpStar

		fcom	SYS_FPClipRight //Testing bottom side...
		fstsw	ax
		sahf
		ja	FAR DumpStar

		fistp	FPDump
		mov	eax, FPDump	//eax = X2D


	//---- Calculating offsets...
		imul	ebx, SYS_XRes
		add	ebx, eax
		mov	edi, SYS_ZBufOffs
		cmp	edx, d[edi + ebx*4]
		jb	DumpStar

		mov	edi, SYS_DBufOffs
		mov	d[edi + ebx*4], -1

	DumpStar:

		ffree	st(2)
		ffree	st(1)
		ffree	st(0)


		add	esi, 12
		dec	ecx
		jnz	StarLoop

		popfd
		popad

	}//asm


}//STR_StarField::Run
