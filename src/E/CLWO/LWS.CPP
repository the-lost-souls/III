/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : LWS.CPP
쿌uthor       : Cyberfish/TLS & Toe/DU
쿗ast update  : 19.01.99
쿌ction       : Converts a .lws-file into scn-format.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include <stdio.h>
#include <string.h>
#include <fstream.h>

#include "w:\general\var.h"
#include "w:\vectors\scn_file.h"
#include "w:\clwo\lws.h"
#include "w:\vectors\vec_file.h"
#include "w:\clwo\lwo.h"
#include "w:\clwo\div.h"
#include "w:\pak\pak.h"
#include "w:\plugins\flare.h"


//---- Supported functions in .lws-file
void 	LWS_LWSC(LWS_Conversion *Data);
void 	LWS_FirstFrame(LWS_Conversion *Data);
void 	LWS_LastFrame(LWS_Conversion *Data);
void 	LWS_FramesPerSecond(LWS_Conversion *Data);
int  	LWS_LoadObject(LWS_Conversion *Data);
int  	LWS_ObjectMotion(LWS_Conversion *Data);
void 	LWS_EndBehavior(LWS_Conversion *Data);
void 	LWS_ParentObject(LWS_Conversion *Data);
int 	LWS_AddLight(LWS_Conversion *Data);
void 	LWS_PivotPoint(LWS_Conversion *Data);
int  	LWS_LightMotion(LWS_Conversion *Data);
void 	LWS_LightColor(LWS_Conversion *Data);
int  	LWS_LgtIntensity(LWS_Conversion *Data);
void 	LWS_LightType(LWS_Conversion *Data);
int  	LWS_LightRange(LWS_Conversion *Data);
int     LWS_ConeAngle(LWS_Conversion *Data);
int 	LWS_EdgeAngle(LWS_Conversion *Data);
void 	LWS_ShowCamera(LWS_Conversion *Data);
void	LWS_TargetObject(LWS_Conversion *Data);
int  	LWS_CameraMotion(LWS_Conversion *Data);
void 	LWS_LookAheadFrames(LWS_Conversion *Data);
int 	LWS_AddNullObject(LWS_Conversion *Data);
void	LWS_MorphTarget(LWS_Conversion *Data);
void	LWS_Metamorph(LWS_Conversion *Data);
void	LWS_ObjDissolve(LWS_Conversion *Data);
void	LWS_LightName(LWS_Conversion *Data);
int	LWS_LensFlare(LWS_Conversion *Data);
void	LWS_LensFlareFade(LWS_Conversion *Data);
int	LWS_ZoomFactor(LWS_Conversion *Data);
void 	LWS_LensFlareOptions(LWS_Conversion *Data);


int LWS_Read1Keys(LWS_Conversion *Data, SCN_FILE_Envelope *Env);



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Conversion
쿗ast update  : 02.08.1998
쿌ction	      : Constructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Conversion::LWS_Conversion(void)
{
	FileSize 	= 0;
	TempFile	= NULL;

	Header		= NULL;
	Objects 	= NULL;
	Lights  	= NULL;
	Camera		= NULL;

	CurObject	= -1;
	CurLight	= -1;
}//constructor


/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : ~LWS_Conversion
쿗ast update  : 02.08.1998
쿌ction	      : Destructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Conversion::~LWS_Conversion(void)
{
	delete Header;
	delete [] Objects;
	delete [] Lights;
	delete Camera;

}//destructor


/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Object
쿗ast update  : 02.08.1998
쿌ction	      : Constructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Object::LWS_Object(void)
{
	Object.ID 		= SCN_FILE_Object_ID;
	Object.ParentObject 	= -1;
	Object.TargetObject 	= -1;
	Object.PathAlignment 	= 0;
	Object.XPivot 		= 0;
	Object.YPivot 		= 0;
	Object.ZPivot 		= 0;

	memset(Object.Path, 0, SYS_FileNameLength);

	MorphTarget 	= -1;
	Metamorph	= 0.0;
	Dissolve 	= 0.0;

	LensFlare	= NULL;
}//constructor

LWS_Object::~LWS_Object(void)
{
	delete LensFlare;
}//destructor



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Light
쿗ast update  : 18.01.1999
쿌ction	      : Constructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Light::LWS_Light(void)
{
	memset(Name, 0, 256);
	LensFlare 		= NULL;

	Light.ID 		= SCN_FILE_Light_ID;
	Light.ParentObject 	= -1;
	Light.TargetObject 	= -1;
	Light.PathAlignment 	= 0;
	Light.Range 		= 1e20;
	Light.ConeAngle 	= 0;
	Light.EdgeAngle 	= 0;
	Light.Intensity 	= 0;

	IntensityEnv.NrKeyframes 	= 0;
	RangeEnv.NrKeyframes 		= 0;
	ConeAngleEnv.NrKeyframes 	= 0;
	EdgeAngleEnv.NrKeyframes 	= 0;

	LensFlare = NULL;

}//constructor

/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Light
쿗ast update  : 18.01.1999
쿌ction	      : Destructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Light::~LWS_Light(void)
{
	delete LensFlare;
}//destructor


/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Camera
쿗ast update  : 02.08.1998
쿌ction	      : Constructor
�
쿙otes:
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
LWS_Camera::LWS_Camera(void)
{
	Camera.ID 		= SCN_FILE_Camera_ID;
	Camera.PathAlignment    = 0;
	Camera.ParentObject	= -1;
	Camera.TargetObject	= -1;
	Camera.ZoomFactor	= 3.2;

	ZoomFactorEnv.NrKeyframes = 0;
}//constructor



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_SCN
쿗ast update  : 14.08.1998
쿌ction       : Loads a scene-file (.SCN)
�
쿔nput variables : Filename of .scn-file
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
SCN_FILE_Header *LWS_SCN(const char *FileName, uint32 Flags)
{
//---- Reading .scn-file from disk
	byte *LWSFile = (byte*)PAK_ReadFile(FileName);
	if ( LWSFile == NULL )
	{
		SYS_Error(1, "LWS_SCN: Error reading .lws-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if

//---- Loading vector...
	SCN_FILE_Header *SCN = LWS_SCN(LWSFile, PAK_FileSize(FileName), Flags);
	if ( SCN == NULL )
	{
		SYS_Error(1, "LWS_SCN: Error running LWS_SCN.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		delete LWSFile;
		return NULL;
	}//if

	delete LWSFile;
	return SCN;
}//SCN_Load



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_SCN
쿗ast update  : 18.01.99
쿌ction	      : Converts an .lws-file into scn-format
�
쿔n:	pointer to loaded .lws-file
�
쿚ut:  Returns offset to converted file (SCN_FILE_Header)
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
SCN_FILE_Header *LWS_SCN(byte *LWSFile, uint32 FileSize, uint32 Flags)
{
	int C1, C2;
	LWS_Conversion *Data = new LWS_Conversion;
	if ( Data == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory.\n");
		return NULL;
	}//if

	Data->Flags = Flags;


//---- Checking filesize and replacing "," with "." ...
	C1 = 0;
	for (C1 = 0; C1 < FileSize; C1++)
	    if ( LWSFile[C1] == ',' ) LWSFile[C1] = '.';
	Data->FileSize = FileSize;


//---- Saving tempfile...
	Data->TempFile = fopen("temp.lws", "wb");
	if (Data->TempFile == NULL)
	{
		SYS_Error(1, "LWS_SCN: Error opening file (temp.lws)\n");
		delete Data;
		return NULL;
	}//if

	if (fwrite(LWSFile, Data->FileSize, 1, Data->TempFile) < 1)
	{
		SYS_Error(1, "LWS_SCN: Error writing file (temp.lws)\n");
		fclose(Data->TempFile);
		delete Data;
		return NULL;
	}//if

	fclose(Data->TempFile);


//---- Allocating memory for header, objects, lights and camera...
	Data->Header = new SCN_FILE_Header;
	if ( Data->Header == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory for fileheader.\n");
		delete Data;
		return NULL;
	}//if
	memset(Data->Header->IDString, 0, 256);


	Data->Objects = new LWS_Object[LWS_MaxObjects];
	if ( Data->Objects == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory to convert objects.\n");
		delete Data;
		return NULL;
	}//if

	Data->Lights = new LWS_Light[LWS_MaxLights];
	if ( Data->Lights == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory to convert lights.\n");
		delete Data;
		return NULL;
	}//if

	Data->Camera  = new LWS_Camera;
	if ( Data->Camera == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory to convert camera.\n");
		delete Data;
		return NULL;
	}//if


//---- Opening tempfile...
	Data->TextFile.open("temp.lws", ios::nocreate | ios::in);
	if ( Data->TextFile.fail() )
	{
		SYS_Error(1, "LWS_SCN: Couldn't open tempfile (temp.tmp).\n");
		delete Data;
		return NULL;
	}//if


//---- Reading tempfile...
	while ( !Data->TextFile.eof() )
	{
		Data->TextFile >> Data->Line;
		if ( strcmp(Data->Line, "LWSC") 		== 0 ) LWS_LWSC(Data);
		if ( strcmp(Data->Line, "FirstFrame") 		== 0 ) LWS_FirstFrame(Data);
		if ( strcmp(Data->Line, "LastFrame") 		== 0 ) LWS_LastFrame(Data);
		if ( strcmp(Data->Line, "FramesPerSecond") 	== 0 ) LWS_FramesPerSecond(Data);
		if ( strcmp(Data->Line, "LoadObject" ) 		== 0 ) if (LWS_LoadObject(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"LoadObject\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "ObjectMotion")		== 0 ) if (LWS_ObjectMotion(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"ObjectMotion\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "EndBehavior") 		== 0 ) LWS_EndBehavior(Data);
		if ( strcmp(Data->Line, "ParentObject") 	== 0 ) LWS_ParentObject(Data);
		if ( strcmp(Data->Line, "AddLight") 		== 0 ) if (LWS_AddLight(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"AddLight\".\n");
			delete Data;
			return NULL;
		}//if

		if ( strcmp(Data->Line, "PivotPoint") 		== 0 ) LWS_PivotPoint(Data);
		if ( strcmp(Data->Line, "LightMotion") 		== 0 ) if (LWS_LightMotion(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"LightMotion\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "LightColor")		== 0 ) LWS_LightColor(Data);
		if ( strcmp(Data->Line, "LgtIntensity") 	== 0 ) if (LWS_LgtIntensity(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"LgtIntensity\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "LightRange" ) 		== 0 ) if (LWS_LightRange(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"LightRange\".\n");
			delete Data;
			return NULL;
		}//if

		if ( strcmp(Data->Line, "LightType") 		== 0 ) LWS_LightType(Data);
		if ( strcmp(Data->Line, "ConeAngle" ) 		== 0 ) if (LWS_ConeAngle(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"ConeAngle\".\n");
			delete Data;
			return NULL;
		}//if

		if ( strcmp(Data->Line, "EdgeAngle" ) 		== 0 ) if (LWS_EdgeAngle(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"EdgeAngle\".\n");
			delete Data;
			return NULL;
		}//if

		if ( strcmp(Data->Line, "ShowCamera") 		== 0 ) LWS_ShowCamera(Data);
		if ( strcmp(Data->Line, "TargetObject" ) 	== 0 ) LWS_TargetObject(Data);
		if ( strcmp(Data->Line, "CameraMotion") 	== 0 ) if (LWS_CameraMotion(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"CameraMotion\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "LookAheadFrames")  	== 0 ) LWS_LookAheadFrames(Data);
		if ( strcmp(Data->Line, "AddNullObject" ) 	== 0 ) if ( LWS_AddNullObject(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"AddNullObject\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "MorphTarget") 		== 0 ) LWS_MorphTarget(Data);
		if ( strcmp(Data->Line, "Metamorph") 		== 0 ) LWS_Metamorph(Data);
		if ( strcmp(Data->Line, "ObjDissolve" )		== 0 ) LWS_ObjDissolve(Data);
		if ( strcmp(Data->Line, "LightName" )		== 0 ) LWS_LightName(Data);
		if ( strcmp(Data->Line, "LensFlare" ) 		== 0 ) if ( LWS_LensFlare(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"LensFlare\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "LensFlareFade" ) 	== 0 ) LWS_LensFlareFade(Data);
		if ( strcmp(Data->Line, "ZoomFactor" ) 		== 0 ) if (LWS_ZoomFactor(Data) == 0)
		{
			SYS_Error(1, "LWS_SCN: Error reading command \"ZoomFactor\".\n");
			delete Data;
			return NULL;
		}//if
		if ( strcmp(Data->Line, "LensFlareOptions" )	== 0 ) LWS_LensFlareOptions(Data);
	}//while

	Data->TextFile.close();

	if ( Data->Valid == FALSE )
	{
		SYS_Error(1, "LWS_SCN: This is not a valid .lws-file.");
		delete Data;
		return NULL;
	}//if


//---- Morphing metamorph-objects...
	VEC_FILE_Header *Original = NULL;
	VEC_FILE_Header *Target = NULL;
	for ( C1 = 0; C1 <= Data->CurObject; C1++ )
	{
		if ( Data->Objects[C1].MorphTarget != -1 )
		{

		//---- Loading original object and morphtarget object...

			Original = (VEC_FILE_Header*)PAK_ReadFile(Data->Objects[C1].Object.Path);
			if ( Original == NULL )
			{
				SYS_Error(1, "LWS_SCN: Error reading morphobject.\n");
				SYS_Error(3, "  ( ", Data->Objects[C1].Object.Path, " )\n");
				delete Data;

				return NULL;
			}//if

			Target = (VEC_FILE_Header*)PAK_ReadFile(Data->Objects[Data->Objects[C1].MorphTarget].Object.Path);
			if ( Target == NULL )
			{
				SYS_Error(1, "LWS_SCN: Error reading morphtarget.\n");
				SYS_Error(3, "  ( ", Data->Objects[Data->Objects[C1].MorphTarget].Object.Path, " )\n");

				delete Original;
				delete Data;

				return NULL;
			}//if

			if ( Original->Vertices != Target->Vertices )
			{
				SYS_Error(1, "LWS_SCN: Object and morphtarget have different number of vertices.\n");
				SYS_Error(5, "  ( ", Data->Objects[C1].Object.Path, " -> ", Data->Objects[Data->Objects[C1].MorphTarget].Object.Path, " )\n");

				delete Original;
				delete Target;
				delete Data;

				return NULL;
			}//if


		//---- Searching for verticechunks...
			VEC_FILE_VertChunk *OriginalVerts;
			VEC_FILE_VertChunk *TargetVerts;

			OriginalVerts = (VEC_FILE_VertChunk*)( (byte*)Original + sizeof(VEC_FILE_Header));
			TargetVerts = (VEC_FILE_VertChunk*)( (byte*)Target + sizeof(VEC_FILE_Header));

			for ( C2 = 0; C2 < Original->Vertices; C2++ )
			{
				OriginalVerts->Vertices[C2].X += (TargetVerts->Vertices[C2].X - OriginalVerts->Vertices[C2].X) *Data->Objects[C1].Metamorph;
				OriginalVerts->Vertices[C2].Y += (TargetVerts->Vertices[C2].Y - OriginalVerts->Vertices[C2].Y) *Data->Objects[C1].Metamorph;
				OriginalVerts->Vertices[C2].Z += (TargetVerts->Vertices[C2].Z - OriginalVerts->Vertices[C2].Z) *Data->Objects[C1].Metamorph;
			}//if

			delete Target;


		//---- Creating new checksum...
			Original->Checksum = 0;
			byte *B = (byte*)Original;

			uint32 Checksum = 0;
			for ( C2 = 0; C2 < Original->FileSize; C2++ )
				Checksum += (uint32)*(uint8*)&B[C2];
			Original->Checksum = Checksum;


		//---- Saving morphed file...
			FILE *MorphFile;
			MorphFile = fopen(Data->Objects[C1].Object.Path, "wb");
			if (MorphFile == NULL)
			{
				SYS_Error(1, "LWS_SCN: Error opening morphed file.\n");
				SYS_Error(3, "  ( ", Data->Objects[C1].Object.Path, " )\n");

				delete Original;
				delete Data;

				return NULL;
			}//if

			if (fwrite((char*)Original, Original->FileSize, 1, MorphFile) < 1)
			{
				SYS_Error(1, "LWS_SCN: Error writing morphed file.\n");
				SYS_Error(3, "  ( ", Data->Objects[C1].Object.Path, " )\n");

				delete Original;
				delete Data;
				fclose(MorphFile);

				return NULL;
			}//if

			delete Original;
			fclose(MorphFile);

		}//if
	}//for

	for ( C1 = 0; C1 <= Data->CurObject; C1++ )
       		if ( Data->Objects[C1].Dissolve == 1.0 )
		{
			memset(Data->Objects[C1].Object.Path, 0, SYS_FileNameLength);

       			strcpy(Data->Objects[C1].Object.Path, "NullObject");
		}//if


//---- Setting chunksizes and filesize...
	Data->Header->FileSize = sizeof(SCN_FILE_Header);

					//Adding objects...
	for ( C1 = 0; C1 <= Data->CurObject; C1++ )
	{
		Data->Objects[C1].Object.Size  = sizeof(SCN_FILE_Object) - 8
					       + Data->Objects[C1].Object.NrKeyframes * sizeof(SCN_FILE_Keyframe6);

		Data->Header->FileSize += Data->Objects[C1].Object.Size + 8;

		if ( Data->Objects[C1].LensFlare )
			Data->Header->FileSize += sizeof(SCN_FILE_LensFlare);
	}//for


					//Adding lights...
	Data->Header->NrEnvelopes = 0;

	for ( C1 = 0; C1 <= Data->CurLight; C1++ )
	{
		Data->Lights[C1].Light.Size = sizeof(SCN_FILE_Light) - 8
					    + Data->Lights[C1].Light.NrKeyframes * sizeof(SCN_FILE_Keyframe6);
		Data->Header->FileSize += Data->Lights[C1].Light.Size + 8;

	//---- Adding lights' plugins...
		if ( Data->Lights[C1].IntensityEnv.NrKeyframes != 0 )
		{

			Data->Header->FileSize  += sizeof(SCN_FILE_Envelope)
					       	+  Data->Lights[C1].IntensityEnv.NrKeyframes*sizeof(SCN_FILE_Keyframe1);
			Data->Header->NrEnvelopes++;
		}//if

		if ( Data->Lights[C1].RangeEnv.NrKeyframes != 0 )
		{
			Data->Header->FileSize  += sizeof(SCN_FILE_Envelope)
					       	+  Data->Lights[C1].RangeEnv.NrKeyframes*sizeof(SCN_FILE_Keyframe1);
			Data->Header->NrEnvelopes++;
		}//if

		if ( Data->Lights[C1].ConeAngleEnv.NrKeyframes != 0 )
		{
			Data->Header->FileSize  += sizeof(SCN_FILE_Envelope)
					       	+  Data->Lights[C1].ConeAngleEnv.NrKeyframes*sizeof(SCN_FILE_Keyframe1);
			Data->Header->NrEnvelopes++;
		}//if

		if ( Data->Lights[C1].EdgeAngleEnv.NrKeyframes != 0 )
		{
			Data->Header->FileSize  += sizeof(SCN_FILE_Envelope)
					       	+  Data->Lights[C1].EdgeAngleEnv.NrKeyframes*sizeof(SCN_FILE_Keyframe1);
			Data->Header->NrEnvelopes++;
		}//if


	//---- Adding lensflare...
		if ( Data->Lights[C1].LensFlare )
			Data->Header->FileSize  += sizeof(SCN_FILE_LensFlare);

	}//for

	Data->Camera->Camera.Size = sizeof(SCN_FILE_Camera) - 8
				  + Data->Camera->Camera.NrKeyframes*sizeof(SCN_FILE_Keyframe6);
	Data->Header->FileSize += Data->Camera->Camera.Size + 8;

	if ( Data->Camera->ZoomFactorEnv.NrKeyframes != 0 )
	{
		Data->Header->FileSize  += sizeof(SCN_FILE_Envelope)
				       	+  Data->Camera->ZoomFactorEnv.NrKeyframes*sizeof(SCN_FILE_Keyframe1);
		Data->Header->NrEnvelopes++;
	}//if


//---- Setting up header...
	Data->Header->ID 	= SCN_FILE_ID;
	Data->Header->Size 	= sizeof(SCN_FILE_Header) - 264;
	Data->Header->Version	= 110;
	Data->Header->Checksum  = 0;
	Data->Header->NrObjects = Data->CurObject + 1;
	Data->Header->NrLights	= Data->CurLight + 1;

	int P = sprintf(Data->Header->IDString, "CLWS version 1.10\n");
	P += sprintf(&Data->Header->IDString[P], "  FirstFrame   : %6d\n", Data->Header->FirstFrame);
	P += sprintf(&Data->Header->IDString[P], "  LastFrame    : %6d\n", Data->Header->LastFrame);
	P += sprintf(&Data->Header->IDString[P], "  FramesPerSec : %6d\n", Data->Header->FramesPerSecond);
	P += sprintf(&Data->Header->IDString[P], "  Objects      : %6d\n", Data->Header->NrObjects);
	P += sprintf(&Data->Header->IDString[P], "  Lights       : %6d\n", Data->Header->NrLights);


//---- Allocating final memoryblock...
	byte *Out = new byte[Data->Header->FileSize];
	if ( Out == NULL )
	{
		SYS_Error(1, "LWS_SCN: Not enough memory for final data.\n");
		delete Data;
		return NULL;
	}//if
	memset(Out, 0, Data->Header->FileSize);

//---- Setting up final scene-data...
	P = 0;

	//Copying header...
	memcpy((char*)&Out[P], (char*)Data->Header, sizeof(SCN_FILE_Header));
	P += sizeof(SCN_FILE_Header);



	//Copying objects...
	for ( C1 = 0; C1 < Data->Header->NrObjects; C1++ )
	{
		memcpy((char*)&Out[P], (char*)&Data->Objects[C1].Object, Data->Objects[C1].Object.Size + 8);
		P += Data->Objects[C1].Object.Size + 8;

	//---- Copying lensflare...
		if ( Data->Objects[C1].LensFlare )
		{
			memcpy((char*)&Out[P], (char*)Data->Objects[C1].LensFlare, sizeof(SCN_FILE_LensFlare));
			P += sizeof(SCN_FILE_LensFlare);
		}//if
	}//for




	//Copying lights with evt. plugins...
	for ( C1 = 0; C1 < Data->Header->NrLights; C1++ )
	{
		memcpy((char*)&Out[P], (char*)&Data->Lights[C1].Light, Data->Lights[C1].Light.Size + 8);
		P += Data->Lights[C1].Light.Size + 8;

		//Evt. copying IntensityEnvelope...
		if ( Data->Lights[C1].IntensityEnv.NrKeyframes != 0 )
		{
			memcpy((char*)&Out[P], (char*)&Data->Lights[C1].IntensityEnv, Data->Lights[C1].IntensityEnv.Size + 8);
			P += Data->Lights[C1].IntensityEnv.Size + 8;
		}//if

		//Evt. copying RangeEnvelope...
		if ( Data->Lights[C1].RangeEnv.NrKeyframes != 0 )
		{
			memcpy((char*)&Out[P], (char*)&Data->Lights[C1].RangeEnv, Data->Lights[C1].RangeEnv.Size + 8);
			P += Data->Lights[C1].RangeEnv.Size + 8;
		}//if

		//Evt. copying ConeAngleEnvelope...
		if ( Data->Lights[C1].ConeAngleEnv.NrKeyframes != 0 )
		{
			memcpy((char*)&Out[P], (char*)&Data->Lights[C1].ConeAngleEnv, Data->Lights[C1].ConeAngleEnv.Size + 8);
			P += Data->Lights[C1].ConeAngleEnv.Size + 8;
		}//if

		//Evt. copying EdgeAngleEnvelope...
		if ( Data->Lights[C1].EdgeAngleEnv.NrKeyframes != 0 )
		{
			memcpy((char*)&Out[P], (char*)&Data->Lights[C1].EdgeAngleEnv, Data->Lights[C1].EdgeAngleEnv.Size + 8);
			P += Data->Lights[C1].EdgeAngleEnv.Size + 8;
		}//if

		//Evt. copying lensflaredata...
		if ( Data->Lights[C1].LensFlare )
		{
			memcpy((char*)&Out[P], (char*)Data->Lights[C1].LensFlare, Data->Lights[C1].LensFlare->Size + 8);
			P += Data->Lights[C1].LensFlare->Size + 8;
		}//if
	}//for

	//Copying camera...
	memcpy((char*)&Out[P], (char*)&Data->Camera->Camera, Data->Camera->Camera.Size + 8);
	P += Data->Camera->Camera.Size + 8;

	//Evt. copying camera's zoomfactorenvelope...
	if ( Data->Camera->ZoomFactorEnv.NrKeyframes != 0 )
	{
		memcpy((char*)&Out[P], (char*)&Data->Camera->ZoomFactorEnv, Data->Camera->ZoomFactorEnv.Size + 8);
		P += Data->Camera->ZoomFactorEnv.Size + 8;
	}//if


//---- Calculating checksum...
	for ( P = 0; P < Data->Header->FileSize; P++ )
		Data->Header->Checksum += (uint32)*(unsigned char*)&Out[P];

	((SCN_FILE_Header*)Out)->Checksum = Data->Header->Checksum;

	delete Data;
	return (SCN_FILE_Header*)Out;
}//LWS_SCN




//----------------- Function "LWSC"

void LWS_LWSC(LWS_Conversion *Data)
{
	Data->Valid = TRUE;
}//F_LWSC



//----------------- Function "FirstFrame"
void LWS_FirstFrame(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Header->FirstFrame;
}//F_FirstFrame;



//----------------- Function "LastFrame"
void LWS_LastFrame(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Header->LastFrame;
}//F_LawsFrame;



//----------------- Function "FramesPerSecond"
void LWS_FramesPerSecond(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Header->FramesPerSecond;
}//F_FrameStep;



//----------------- Function "LoadObject"
int LWS_LoadObject(LWS_Conversion *Data)
{
	char ObjInFile[256] = "\0";
	char ObjOutFile[256] = "\0";

	Data->CurObject++;
	if ( Data->CurObject == LWS_MaxObjects )
	{
		SYS_Error(1, "LWS_LoadObject: Too many objects in .lws-file.\n");
		return 0;
	}//if

	Data->Active = LWS_ActiveObject;
	Data->TextFile >> ObjInFile;

//---- Converting object...
	if ( SearchStr(ObjInFile, '.') == 0) *(int*)&ObjInFile[strlen(ObjInFile)] = 'OWL.';
	strcpy(ObjOutFile, ObjInFile);
	if ( !(Data->Flags & LWS_FlagIgnoreObjects) )
		*(int*)&ObjOutFile[strlen(ObjInFile) - 4] = 'CEV.';

	strcpy(Data->Objects[Data->CurObject].Object.Path, ObjOutFile);

/*						//Getting objecttype...
	int Pos = 0;
	for ( int C1 = 0; C1 < strlen(ObjInFile); C1++ )
		if ( ObjInFile[C1] == '_' ) Pos = C1;
	if ( Pos == 0 )
	{
		printf("ERR: Filename doesn't define objecttype\n");
		return 0;
	}//if

	int Type;
	if ( Str2Int((char*)&ObjInFile[Pos + 1], Type, '.') != 0 )
	{
		printf("ERR: Unable converting number for objecttype\n");
		return 0;
	}//if*/


	if ( !(Data->Flags & LWS_FlagIgnoreObjects) )
	{

		byte *LWO;
		LWO = (byte*)PAK_ReadFile(ObjInFile);
		if ( LWO == NULL )
		{
			SYS_Error(1, "LWS_LoadObject: Error reading .lwo-file.\n");
			SYS_Error(3, "  ( ", ObjInFile, " )\n");
			return 0;
		}//if

		VEC_FILE_Header *VEC = LWO_VEC(LWO);

		delete [] LWO;

		if ( VEC == NULL )
		{
			SYS_Error(1, "LWS_LoadObject: Error converting .lwo-file.\n");
			SYS_Error(3, "  ( ", ObjInFile, " )\n");
			return 0;
		}//if

	//---- Saving file...
		FILE *VECFile;
		VECFile = fopen(ObjOutFile, "wb");
		if (VECFile == NULL)
		{
			SYS_Error(1, "LWS_LoadObject: Error opening file.\n");
			SYS_Error(3, "  ( ", ObjOutFile, " )\n");
			delete VEC;
			return 0;
		}//if

		if (fwrite(VEC, VEC->FileSize, 1, VECFile) < 1)
		{
			SYS_Error(1, "LWS_LoadObject: Error writing file.\n");
			SYS_Error(3, "  ( ", ObjOutFile, " )\n");
			delete VEC;
			fclose(VECFile);
			return 0;
		}//if

		delete VEC;
		fclose(VECFile);
	}//if ignoreobjects


	return 1;
}//F_LoadObject




//----------------- Function "ObjectMotion"
int LWS_ObjectMotion(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Line; 		//Dump (identifier)
	Data->TextFile >> Data->Line; 		//Dump number of id-channels

						//Reading NrKeyframes
	int NrKeyframes;
	Data->TextFile >> NrKeyframes;
	if (NrKeyframes > LWS_MaxKeyframes)
	{
		SYS_Error(1, "LWS_ObjectMotion: Too many keyframes.\n");
		SYS_Error(3, "  ( ", Data->Objects[Data->CurObject].Object.Path, " )\n");
		return 0;
	}//if
	Data->Objects[Data->CurObject].Object.NrKeyframes = NrKeyframes;

//---- Reading keyframedata...
	SCN_FILE_Keyframe6 *Keyframe = &Data->Objects[Data->CurObject].Object.Keyframes[0];
	for ( int C = 0; C < NrKeyframes; C++ )
	{
		Data->TextFile >> Keyframe->XPos;
		Data->TextFile >> Keyframe->YPos;
		Data->TextFile >> Keyframe->ZPos;

		Data->TextFile >> Keyframe->YAng;
		Data->TextFile >> Keyframe->XAng;
		Data->TextFile >> Keyframe->ZAng;

		Keyframe->XAng = (Keyframe->XAng/360)*2*Pi;
		Keyframe->YAng = (Keyframe->YAng/360)*2*Pi;
		Keyframe->ZAng = (Keyframe->ZAng/360)*2*Pi;

		Data->TextFile >> Data->Line;        //Dump scaling-values
		Data->TextFile >> Data->Line;
		Data->TextFile >> Data->Line;

		Data->TextFile >> Keyframe->Frame;
		Data->TextFile >> Keyframe->Linear;

		Data->TextFile >> Keyframe->Tension;
		Data->TextFile >> Keyframe->Continuity;
		Data->TextFile >> Keyframe->Bias;

		Keyframe++;
	}//for

	return 1;
}//F_ObjectMotion




//----------------- Function "EndBehavior"
void LWS_EndBehavior(LWS_Conversion *Data)
{
	int EndBehavior;
	Data->TextFile >> EndBehavior;
	switch ( Data->Active )
	{
		case LWS_ActiveObject : Data->Objects[Data->CurObject].Object.EndBehavior = EndBehavior;
				  	break;
		case LWS_ActiveLight  : Data->Lights[Data->CurLight].Light.EndBehavior = EndBehavior;
				  	break;
		case LWS_ActiveCamera : Data->Camera->Camera.EndBehavior = EndBehavior;
				  	break;

	}//switch

}//F_EndBavior



//----------------- Function "ParentObject"
void LWS_ParentObject(LWS_Conversion *Data)
{
	int ParentObject;
	Data->TextFile >> ParentObject;

	switch ( Data->Active )
	{
		case LWS_ActiveObject : Data->Objects[Data->CurObject].Object.ParentObject = ParentObject - 1;
				  	break;
		case LWS_ActiveLight  : Data->Lights[Data->CurLight].Light.ParentObject = ParentObject - 1;
				  	break;
		case LWS_ActiveCamera : Data->Camera->Camera.ParentObject = ParentObject - 1;
				  	break;


	}//switch
}//F_ParentObject




//----------------- Function "MorphTarget"
void LWS_MorphTarget(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Objects[Data->CurObject].MorphTarget;
	Data->Objects[Data->CurObject].MorphTarget--;
}//F_MorphTarget



//----------------- Function "Metamorph"
void LWS_Metamorph(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Objects[Data->CurObject].Metamorph;
}//F_MetaMorph



//------------------ Function "ObjDissolve"
void LWS_ObjDissolve(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Objects[Data->CurObject].Dissolve;
}//F_ObjDissolve



//------------------ Function "AddLight"
int LWS_AddLight(LWS_Conversion *Data)
{
	Data->Active = 1;
	Data->CurLight++;
	if ( Data->CurLight == LWS_MaxLights )
	{
		SYS_Error(1, "LWS_AddLight: Too many lights in .lws-file.\n");
		return 0;
	}//if
	return 1;
}//F_AddLight



//------------------ Function "PivotPoint"
void LWS_PivotPoint(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Objects[Data->CurObject].Object.XPivot;
	Data->TextFile >> Data->Objects[Data->CurObject].Object.YPivot;
	Data->TextFile >> Data->Objects[Data->CurObject].Object.ZPivot;
}//F_PivotPoint



//------------------ Function "LightMotion"
int LWS_LightMotion(LWS_Conversion *Data)
{
	Data->TextFile >> Data->Line;		//Dump <identifer>
	Data->TextFile >> Data->Line;		//Dump "number of id-channels"

	int NrKeyframes;
	Data->TextFile >> NrKeyframes;
	if ( NrKeyframes > LWS_MaxKeyframes )
	{
		SYS_Error(1, "LWS_LightMotion: Too many keyframes.\n");
		return 0;
	}//if

	Data->Lights[Data->CurLight].Light.NrKeyframes = NrKeyframes;

//---- Reading keyframedata...
	SCN_FILE_Keyframe6 *Keyframe = &Data->Lights[Data->CurLight].Light.Keyframes[0];
	for ( int C = 0; C < NrKeyframes; C++ )
	{
		Data->TextFile >> Keyframe->XPos;
		Data->TextFile >> Keyframe->YPos;
		Data->TextFile >> Keyframe->ZPos;

		Data->TextFile >> Keyframe->YAng;
		Data->TextFile >> Keyframe->XAng;
		Data->TextFile >> Keyframe->ZAng;

		Keyframe->XAng = (Keyframe->XAng/360)*2*3.1459;
		Keyframe->YAng = (Keyframe->YAng/360)*2*3.1459;
		Keyframe->ZAng = (Keyframe->ZAng/360)*2*3.1459;

		//Dump scaling-values
		Data->TextFile >> Data->Line >> Data->Line >> Data->Line;

		Data->TextFile >> Keyframe->Frame;
		Data->TextFile >> Keyframe->Linear;

		Data->TextFile >> Keyframe->Tension;
		Data->TextFile >> Keyframe->Continuity;
		Data->TextFile >> Keyframe->Bias;

		Keyframe++;
	}//for

	return 1;
}//F_LightMotion



//------------------ Function "LightColor"
void LWS_LightColor(LWS_Conversion *Data)
{
	int R, G, B;
	Data->TextFile >> R >> G >> B;
	Data->Lights[Data->CurLight].Light.R = (float)R/255;
	Data->Lights[Data->CurLight].Light.G = (float)G/255;
	Data->Lights[Data->CurLight].Light.B = (float)B/255;
}//F_LightColor



//------------------ Function "LgtIntensity"
int LWS_LgtIntensity(LWS_Conversion *Data)
{
	char Temp[20];
	Data->TextFile >> Temp;
	if ( strcmp(Temp, "(envelope)") == 0 )
	{

		if ( LWS_Read1Keys(Data, &Data->Lights[Data->CurLight].IntensityEnv) == 0 )
		{
			SYS_Error(1, "LWS_LgtIntensity: Error reading envelope.\n");
			return 0;
		}//if

		Data->Lights[Data->CurLight].IntensityEnv.ID   = SCN_FILE_LightIntensity_ID;
		Data->Lights[Data->CurLight].IntensityEnv.Size = sizeof(SCN_FILE_Envelope) - 8 + sizeof(SCN_FILE_Keyframe1)*Data->Lights[Data->CurLight].IntensityEnv.NrKeyframes;
	}//if
	else Str2Float(Temp, Data->Lights[Data->CurLight].Light.Intensity, '\0');
	return 1;
}//F_LgtIntensity



//------------------ Function "LightRange"
int LWS_LightRange(LWS_Conversion *Data)
{
	char Temp[20];
	Data->TextFile >> Temp;
	if ( strcmp(Temp, "(envelope)") == 0 )
	{
		if ( LWS_Read1Keys(Data, &Data->Lights[Data->CurLight].RangeEnv) == 0 )
		{
			SYS_Error(1, "LWS_LightRange: Error reading envelope.\n");
			return 0;
		}//if
		Data->Lights[Data->CurLight].RangeEnv.ID   = SCN_FILE_LightRange_ID;
		Data->Lights[Data->CurLight].RangeEnv.Size = sizeof(SCN_FILE_Envelope) - 8 + sizeof(SCN_FILE_Keyframe1)*Data->Lights[Data->CurLight].RangeEnv.NrKeyframes;
	}//if
	else Str2Float(Temp, Data->Lights[Data->CurLight].Light.Range, '\0');
	return 1;
}//F_LightRange



//------------------ Function "LightType"
void LWS_LightType(LWS_Conversion *Data)
{
	int LightType;
	Data->TextFile >> LightType;
	Data->Lights[Data->CurLight].Light.Type = LightType + 1;
}//F_LightType



//------------------ Function "ConeAngle"
int LWS_ConeAngle(LWS_Conversion *Data)
{
	char Temp[20];
	Data->TextFile >> Temp;
	if ( strcmp(Temp, "(envelope)") == 0 )
	{
		if ( LWS_Read1Keys(Data, &Data->Lights[Data->CurLight].ConeAngleEnv) == 0 )
		{
			SYS_Error(1, "LWS_ConeAngle: Error reading envelope.\n");
			return 0;
		}//if

		Data->Lights[Data->CurLight].ConeAngleEnv.ID   = SCN_FILE_LightConeAngle_ID;
		Data->Lights[Data->CurLight].ConeAngleEnv.Size = sizeof(SCN_FILE_Envelope) - 8 + sizeof(SCN_FILE_Keyframe1)*Data->Lights[Data->CurLight].ConeAngleEnv.NrKeyframes;
	}//if
	else Str2Float(Temp, Data->Lights[Data->CurLight].Light.ConeAngle, '\0');


//---- Converting angles to radians...
	Data->Lights[Data->CurLight].Light.ConeAngle = (Data->Lights[Data->CurLight].Light.ConeAngle/360)*2*Pi;
	for ( int C = 0; C < Data->Lights[Data->CurLight].ConeAngleEnv.NrKeyframes; C++ )
		Data->Lights[Data->CurLight].ConeAngleEnv.Keyframes[C].Value =
			(Data->Lights[Data->CurLight].ConeAngleEnv.Keyframes[C].Value/360)*2*Pi;

	return 1;
}//F_LightType



//------------------ Function "EdgeAngle"
int LWS_EdgeAngle(LWS_Conversion *Data)
{
	char Temp[20];
	Data->TextFile >> Temp;
	if ( strcmp(Temp, "(envelope)") == 0 )
	{
		if ( LWS_Read1Keys(Data, &Data->Lights[Data->CurLight].EdgeAngleEnv) == 0 )
		{
			SYS_Error(1, "LWS_EdgeAngle: Error reading envelope.\n");
			return 0;
		}//if

		Data->Lights[Data->CurLight].EdgeAngleEnv.ID   = SCN_FILE_LightEdgeAngle_ID;
		Data->Lights[Data->CurLight].EdgeAngleEnv.Size = sizeof(SCN_FILE_Envelope) - 8 + sizeof(SCN_FILE_Keyframe1)*Data->Lights[Data->CurLight].EdgeAngleEnv.NrKeyframes;
	}//if
	else Str2Float(Temp, Data->Lights[Data->CurLight].Light.EdgeAngle, '\0');


//---- Converting angles to radians...
	Data->Lights[Data->CurLight].Light.EdgeAngle = (Data->Lights[Data->CurLight].Light.EdgeAngle/360)*2*Pi;
	for ( int C = 0; C < Data->Lights[Data->CurLight].EdgeAngleEnv.NrKeyframes; C++ )
		Data->Lights[Data->CurLight].EdgeAngleEnv.Keyframes[C].Value =
			(Data->Lights[Data->CurLight].EdgeAngleEnv.Keyframes[C].Value/360)*2*Pi;

	return 1;
}//F_EdgeAngle



//------------------ Function "ShowCamera"
void LWS_ShowCamera(LWS_Conversion *Data)
{
	Data->Active = LWS_ActiveCamera;
}//F_ShowCamera



void LWS_TargetObject(LWS_Conversion *Data)
{
	int TargetObject;
	Data->TextFile >> TargetObject;

	switch ( Data->Active )
	{
		case LWS_ActiveObject : Data->Objects[Data->CurObject].Object.TargetObject = TargetObject - 1;
				  	break;
		case LWS_ActiveLight  : Data->Lights[Data->CurLight].Light.TargetObject = TargetObject - 1;
				  	break;
		case LWS_ActiveCamera : Data->Camera->Camera.TargetObject = TargetObject - 1;
				  	break;


	}//switch
}//F_TargetObject



//------------------ Function "CameraMotion"
int LWS_CameraMotion(LWS_Conversion *Data)
{

	Data->TextFile >> Data->Line;		//Dump <identifer>
	Data->TextFile >> Data->Line;		//Dump "number of id-channels"

	int NrKeyframes;
	Data->TextFile >> NrKeyframes;
	if ( NrKeyframes > LWS_MaxKeyframes )
	{
		SYS_Error(1, "LWS_CameraMotion: Too many keyframes.\n");
		return 0;
	}//if

	Data->Camera->Camera.NrKeyframes = NrKeyframes;


//---- Reading keyframedata...
	SCN_FILE_Keyframe6 *Keyframe = &Data->Camera->Camera.Keyframes[0];
	for ( int C = 0; C < NrKeyframes; C++ )
	{
		Data->TextFile >> Keyframe->XPos;
		Data->TextFile >> Keyframe->YPos;
		Data->TextFile >> Keyframe->ZPos;

		Data->TextFile >> Keyframe->YAng;
		Data->TextFile >> Keyframe->XAng;
		Data->TextFile >> Keyframe->ZAng;

		Keyframe->XAng = (Keyframe->XAng/360)*2*3.1459;
		Keyframe->YAng = (Keyframe->YAng/360)*2*3.1459;
		Keyframe->ZAng = (Keyframe->ZAng/360)*2*3.1459;

		//Dump scaling-values
		Data->TextFile >> Data->Line >> Data->Line >> Data->Line;

		Data->TextFile >> Keyframe->Frame;
		Data->TextFile >> Keyframe->Linear;

		Data->TextFile >> Keyframe->Tension;
		Data->TextFile >> Keyframe->Continuity;
		Data->TextFile >> Keyframe->Bias;

		Keyframe++;

	}//for

	return 1;
}//F_CameraMotion



//------------------ Function "LookAheadFrames"
void LWS_LookAheadFrames(LWS_Conversion *Data)
{
	int PathAlignment;
	Data->TextFile >> PathAlignment;
	switch ( Data->Active )
	{
		case LWS_ActiveObject : Data->Objects[Data->CurObject].Object.PathAlignment = PathAlignment;
				  	break;
		case LWS_ActiveLight  : Data->Lights[Data->CurLight].Light.PathAlignment = PathAlignment;
				  	break;
		case LWS_ActiveCamera : Data->Camera->Camera.PathAlignment = PathAlignment;
				  	break;


	}//switch

}//F_ShowCamera



//------------------ Function "AddNullObject"
int LWS_AddNullObject(LWS_Conversion *Data)
{
	Data->Active = 0;
	Data->CurObject++;

//---- Lensflare?
	char Line[256];
	Data->TextFile.getline(Line, 256, '\n');
	int LensFlare = 0;

	switch ( *(int*)&Line[1] )
	{
		case '1RLF'   : LensFlare = 1;
				break;
		case '2RLF'   : LensFlare = 2;
				break;
		case '3RLF'   : LensFlare = 3;
				break;

		default	      : break;
	}//switch

	if ( LensFlare )
	{
		SCN_FILE_LensFlare *L = new SCN_FILE_LensFlare;
		if ( L == NULL )
		{
			SYS_Error(1, "LWS_AddNullObject: Not enough memory for lensflaredata.\n");
			SYS_Error(3, "  ( ", Line, " )\n");
			return 0;
		}//if
		Data->Objects[Data->CurObject].LensFlare = L;


	//---- Setting up lensflare...
		memset(L->MapFile, 0, SYS_FileNameLength);

		L->ID 	= 'RLFO';
		L->Size = sizeof(SCN_FILE_LensFlare) - 8;
		L->Flags= FLR_FlagDefault;

		//Fixing filename
		strcpy(L->MapFile, (char*)&Line[23]);
		for ( int C = 0; C < strlen(L->MapFile); C++ )
		{
			if ( L->MapFile[C] == ';' ) L->MapFile[C] = ':';
			if ( L->MapFile[C] == '|' ) L->MapFile[C] = '\\';
		}//for

		//Getting size...
		if ( !Str2Float((char*)&Line[6], L->FlareSize, ' ') )
		{
			SYS_Error(1, "LWS_AddNullObject: Error extracting flaresize from object's name.\n");
			return 0;
		}

		//Getting flags...
		for ( C = 7; C >= 0; C-- )
		{
			if ( Line[14 + C] == '1' ) L->Flags |= (1 << (7-C));
		}//for

		L = NULL;
	}//if



//---- Setting up nullobject...
	if ( Data->CurObject == LWS_MaxObjects )
	{
		SYS_Error(1, "LWS_AddNullObject: Too many objects in .lws-file.\n");
		return 0;
	}//if
	strcpy(Data->Objects[Data->CurObject].Object.Path, "NullObject");
	return 1;
}//F_AddNullObject



//------------------ Function "LightName"
void LWS_LightName(LWS_Conversion *Data)
{
	Data->TextFile.getline(Data->Lights[Data->CurLight].Name, 256);
}//LWS_Lightname



//------------------ Function "LensFlare"
int LWS_LensFlare(LWS_Conversion *Data)
{
	SCN_FILE_LensFlare *L = new SCN_FILE_LensFlare;
	if ( L == NULL )
	{
		SYS_Error(1, "LWS_LensFlare: Not enough memory for lensflaredata.\n");
		SYS_Error(3, "  ( ", Data->Lights[Data->CurLight].Name, " )\n");
		return 0;
	}//if
	Data->Lights[Data->CurLight].LensFlare = L;

//---- Setting up lensflare...
	memset(L->MapFile, 0, SYS_FileNameLength);

	L->Flags= FLR_FlagDefault;
	L->ID 	= 'RLFL';
	L->Size = sizeof(SCN_FILE_LensFlare) - 8;

	switch ( *(int*)&Data->Lights[Data->CurLight].Name[1] )
	{
		case '1RLF'   : L->Type = 1;
				break;
		case '2RLF'   : L->Type = 2;
				break;
		case '3RLF'   : L->Type = 3;
				break;

		default	      : SYS_Error(1, "LWS_LensFlare: No flaretype defined.\n");
				return 0;
	}//switch


	//Fixing filename
	strcpy(L->MapFile, (char*)&Data->Lights[Data->CurLight].Name[14]);
	for ( int C = 0; C < strlen(L->MapFile); C++ )
	{
		if ( L->MapFile[C] == ';' ) L->MapFile[C] = ':';
		if ( L->MapFile[C] == '|' ) L->MapFile[C] = '\\';
	}//for

	if ( !Str2Float((char*)&Data->Lights[Data->CurLight].Name[6], L->FlareSize, ' ') )
	{
		SYS_Error(1, "LWS_LensFlare: Error extracting flaresize from light's name.\n");
		return 0;
	}

	L = NULL;
	return 1;
}//LWS_Lightname



//------------------ Function "LensFlare"
void LWS_LensFlareFade(LWS_Conversion *Data)
{
	int FadeFlags = 0;

	Data->TextFile >> FadeFlags;
	if ( FadeFlags & 0x8 )
		Data->Lights[Data->CurLight].LensFlare->Flags |= FLR_FlagFadeBehindObject;
	if ( FadeFlags & 0x4 )
		Data->Lights[Data->CurLight].LensFlare->Flags |= FLR_FlagFadeOffScreen;
}//LWS_LensFlareFade



//------------------ Function "LensFlareOptions"
void LWS_LensFlareOptions(LWS_Conversion *Data)
{
	int Flags = 0;

	Data->TextFile >> Flags;
	if ( Flags & 0x40 )
		Data->Lights[Data->CurLight].LensFlare->Flags |= FLR_FlagFadeWithLight;
}//LWS_LensFlareOptions






//------------------ Function "ZoomFactor"
int LWS_ZoomFactor(LWS_Conversion *Data)
{
	char Temp[20];
	Data->TextFile >> Temp;
	if ( strcmp(Temp, "(envelope)") == 0 )
	{
		if ( LWS_Read1Keys(Data, &Data->Camera->ZoomFactorEnv) == 0 )
		{
			SYS_Error(1, "LWS_ZoomFactor: Error reading envelope.\n");
			return 0;
		}//if

		Data->Camera->ZoomFactorEnv.ID   = SCN_FILE_ZoomFactor_ID;
		Data->Camera->ZoomFactorEnv.Size = sizeof(SCN_FILE_Envelope) - 8 + sizeof(SCN_FILE_Keyframe1)*Data->Camera->ZoomFactorEnv.NrKeyframes;
	}//if
	else Str2Float(Temp, Data->Camera->Camera.ZoomFactor, '\0');

	return 1;
}//F_ZoomFactor





/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS_Read1Keys
쿗ast update  : 02.08.1998
쿌ction	      : Constructor
�
쿔n: Pointers to a LWS_Conversion-structor and a SCN_FILE_Envelope.
�
쿚ut: 0 if error.
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
int LWS_Read1Keys(LWS_Conversion *Data, SCN_FILE_Envelope *Env)
{
	Data->TextFile >> Data->Line;		//Dump "number of info channels"
	Data->TextFile >> Env->NrKeyframes;	//Read number of keyframes
	if ( Env->NrKeyframes > LWS_MaxKeyframes )
	{
		SYS_Error(1, "LWS_Read1Keys: Too many keyframes.\n");
		return 0;
	}//if

	for ( int C = 0; C < Env->NrKeyframes; C++)//Read keyframedata...
	{
		Data->TextFile >> Env->Keyframes[C].Value;

		Data->TextFile >> Env->Keyframes[C].Frame;
		Data->TextFile >> Env->Keyframes[C].Linear;

		Data->TextFile >> Env->Keyframes[C].Tension;
		Data->TextFile >> Env->Keyframes[C].Continuity;
		Data->TextFile >> Env->Keyframes[C].Bias;
	}//for

	Data->TextFile >> Data->Line; 		//Dump "EndBehavior"
	Data->TextFile >> Env->EndBehavior;	//Read value
	return 1;
}//LWS_Read1Keys


/*void main(void)
{
	if ( Aquarium() == 0 ) return;

	void *LWSFile = PAK_ReadFile("test.lws");
	if ( LWSFile == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	int FileSize = PAK_FileSize("test.lws");

	SCN_FILE_Header *SCNFile = LWS_SCN((unsigned char*)LWSFile, FileSize);
	if ( SCNFile == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

}//main*/

