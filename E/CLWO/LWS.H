/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWS.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 25.01.1999
쿌ction       : Structures for lws-conversion.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef LWS_included
#define LWS_included

#include "w:\vectors\scn_file.h"
#include "w:\general\var.h"
#include <fstream.h>

const uint32 LWS_MaxObjects   = 250;
const uint32 LWS_MaxKeyframes = 200;
const uint32 LWS_MaxLights    =  50;

const uint32 LWS_ActiveObject =   0;
const uint32 LWS_ActiveLight  =   1;
const uint32 LWS_ActiveCamera =   2;


const uint32 LWS_FlagDefault  		= 0;
const uint32 LWS_FlagIgnoreObjects	= 0x0001;
		/*
		LWO-files are NOT converted. All objectpaths are now stored
		with .LWO extension.
		*/



class LWS_Object
{
private:

public:
	LWS_Object(void);		//constructor
	~LWS_Object(void);

	SCN_FILE_Object Object;
	SCN_FILE_Keyframe6 Keyframes[LWS_MaxKeyframes];

	float 	Metamorph, Dissolve;
	int	MorphTarget;

	SCN_FILE_LensFlare     *LensFlare;

};//LWS_Object


class LWS_Light
{
private:

public:
	LWS_Light(void);		//constructor
	~LWS_Light(void);		//destructor

	char 	Name[256];

	SCN_FILE_Light Light;
	SCN_FILE_Keyframe6 ObjectKeyframes[LWS_MaxKeyframes];

	SCN_FILE_Envelope 	IntensityEnv;
	SCN_FILE_Keyframe1 	IntensityKeyframes[LWS_MaxKeyframes];
	SCN_FILE_Envelope 	RangeEnv;
	SCN_FILE_Keyframe1 	RangeKeyframes[LWS_MaxKeyframes];
	SCN_FILE_Envelope 	ConeAngleEnv;
	SCN_FILE_Keyframe1 	ConeAngleKeyframes[LWS_MaxKeyframes];
	SCN_FILE_Envelope 	EdgeAngleEnv;
	SCN_FILE_Keyframe1 	EdgeAngleKeyframes[LWS_MaxKeyframes];

	SCN_FILE_LensFlare     *LensFlare;

};//LWS_Light



class LWS_Camera
{
private:
public:
	LWS_Camera(void);

	SCN_FILE_Camera 	Camera;
	SCN_FILE_Keyframe6 	CameraKeyframes[LWS_MaxKeyframes];

	SCN_FILE_Envelope	ZoomFactorEnv;
	SCN_FILE_Keyframe1	ZoomFactorKeyframes[LWS_MaxKeyframes];
};



class LWS_Conversion
{
private:

public:


	LWS_Conversion(void);		//Constructor
	~LWS_Conversion(void);		//Destructor

	int	FileSize;
	int	Flags;

	FILE	*TempFile;
	fstream	TextFile;

	SCN_FILE_Header	*Header;
	LWS_Object	*Objects;
	LWS_Light  	*Lights;
	LWS_Camera 	*Camera;

	char	Line[256];

	int	Valid;
	int	Active;

	int	CurObject;
	int	CurLight;

};//LWS_Conversion


SCN_FILE_Header *LWS_SCN(byte *LWSFile, uint32 FileSize, uint32 Flags = LWS_FlagDefault);
SCN_FILE_Header *LWS_SCN(const char *FileName, uint32 Flags = LWS_FlagDefault);

#endif
