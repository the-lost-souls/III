/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : SCN.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 25.01.99
쿌ction       : SCN_FILE structuredefinitions
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef SCN_FILE_included
#define SCN_FILE_included

#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"
#include "w:\vectors\lights.h"
#include "w:\vectors\scn_run.h"

const uint32 SCN_FILE_ID	= 'NECS';
const uint32 SCN_FILE_Object_ID = 'JBOV';
const uint32 SCN_FILE_Light_ID  = 'THGL';
const uint32 SCN_FILE_Camera_ID = 'RMAC';

const uint32 SCN_FILE_LightIntensity_ID = 'TNIL';
const uint32 SCN_FILE_LightRange_ID     = 'GNRL';
const uint32 SCN_FILE_LightEdgeAngle_ID = 'GDEL';
const uint32 SCN_FILE_LightConeAngle_ID = 'NOCL';
const uint32 SCN_FILE_ZoomFactor_ID	= 'MOOZ';

const uint32 SCN_FILE_LLensFlare_ID	= 'RLFL';
const uint32 SCN_FILE_OLensFlare_ID	= 'RLFO';

const uint32 SCN_FILE_FlagDefault		= 0;
const uint32 SCN_FILE_FlagIgnoreObjects		= 0x1;
	/*
	   All parent/target-objects is returned as INDEXES, not offsets. Also,
	   all VEC_Header-pointers inside SCN_Object are NULL.
	*/

class SCN_FILE_Keyframe1	//Keyframe with 1 channel
{
public:
	uint32 	Frame;

	float 	Tension;
	float	Continuity;
	float	Bias;

	uint32	Linear;

	float 	Value;

private:

};//SCN_FILE_Keyframe1


class SCN_FILE_Keyframe6	//Keyframe with 6 channels
{
private:

public:
	uint32	Frame;

	float	Tension;
	float	Continuity;
	float	Bias;

	uint32	Linear;

	float	XPos, YPos, ZPos;
	float	XAng, YAng, ZAng;
};//SCN_FILE_Keyframe6




class SCN_FILE_Header		//Fileheader for SCN-files
{
public:
				//General file-info
	char    IDString[256];  //ID-string. Should be ended with EOF (1Ah)
	uint32 	ID;		//Chunk-ID
	uint32	Size;		//Chunk-size
	uint32	Version;	//Version of clws that wrote the file
	uint32	FileSize;	//Size of the file
	uint32	Checksum;	//Checksum of the file

	uint32	NrObjects;
	uint32	NrLights;
	uint32	NrEnvelopes;
	uint32	FirstFrame;
	uint32	LastFrame;
	uint32	FramesPerSecond;
private:

};//SCN_FILE_Header


class SCN_FILE_Object		//Standard object-chunk for scn-files
{
public:
	uint32	ID;
	uint32	Size;

	char	Path[SYS_FileNameLength];

	float	XPivot, YPivot, ZPivot;

	int	EndBehavior;
	int	PathAlignment;

	int	ParentObject;
	int	TargetObject;

	int	NrKeyframes;
	SCN_FILE_Keyframe6 Keyframes[];

private:
};//SCN_FILE_Object


class SCN_FILE_Light		//Standard light-chunk for scn-files
{
public:
	uint32	ID;
	uint32	Size;

	uint32	Type;

	float	R, G, B;
	float	Range, Intensity;
	float	ConeAngle, EdgeAngle;

	int	EndBehavior;
	int	PathAlignment;

	int	ParentObject;
	int	TargetObject;

	int	NrKeyframes;
	SCN_FILE_Keyframe6 Keyframes[];

private:
};//SCN_FILE_Light


class SCN_FILE_Camera		//Standard camera-chunk for scn-files
{
public:
	uint32	ID;
	uint32	Size;

	int	EndBehavior;
	int	PathAlignment;

	int	ParentObject;
	int	TargetObject;

	float	ZoomFactor;

	int	NrKeyframes;
	SCN_FILE_Keyframe6 Keyframes[];
private:

};//SCN_FILE_Camera;



class SCN_FILE_Envelope		//Envelope for lights with 1-channel keyframes
{
public:
	uint32	ID;
	uint32	Size;

	uint32	EndBehavior;

	uint32	NrKeyframes;
	SCN_FILE_Keyframe1 Keyframes[];
private:
};//SCN_FILE_Envelope


class SCN_FILE_LensFlare
{
public:
	uint32	ID;
	uint32	Size;

	uint32	Type;

	uint32	Flags;

	float	FlareSize;
	char	MapFile[SYS_FileNameLength];
private:
};


SCN_Header *SCN_Load(SCN_FILE_Header *SCNFile, uint32 Flags, SCN_Header *CloneOrigin);
SCN_Header *SCN_Load(const char *FileName, uint32 Flags, SCN_Header *CloneOrigin);
SCN_Header *SCN_Load(const char *FileName, uint32 Flags = SCN_FILE_FlagDefault);

#endif


