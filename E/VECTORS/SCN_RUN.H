/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : SCN.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 03.12.1998
쿌ction       : SCN_FILE- and SCN-structures.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef SCN_RUN_included
#define SCN_RUN_included

#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"
#include "w:\vectors\lights.h"
#include "w:\vectors\vec_run.h"


class SCN_Keyframe6
{
public:
	int	Frame;
	float	Tension, Continuity, Bias;
	int	Linear;

	float	XPos, YPos, ZPos;
	float	XAng, YAng, ZAng;
private:

};//SCN_Keyframe6


class SCN_Object
{
public:
	SCN_Object(void);		//constructor
	~SCN_Object(void);		//destructor

	char	Name[SYS_FileNameLength];

	float	CacheTime1;		//
	float	CacheMatrix1[12];	//Matrix for object at t = CacheTime

	float	CacheTime2;		//
	float	CacheMatrix2[12];	//Matrix for object at t = CacheTime

	float	Matrix[12];

	float	XPivot, YPivot, ZPivot;

	SCN_Object *ParentObject;
	SCN_Object *TargetObject;

	int	EndBehavior;
	int	PathAlignment;

	int	NrKeyframes;
	SCN_Keyframe6 *Keyframes;

	VEC_Header *VEC;                //Offset of object's VEC-structure.
private:

};//SCN_Object

class SCN_Keyframe1
{
public:
	int	Frame;
	float	Tension, Continuity, Bias;
	int	Linear;

	float	Value;
private:

};//SCN_Keyframe1




class SCN_Light
{
public:
	SCN_Light(void);
	~SCN_Light(void);

	float Matrix[12];

	SCN_Object *ParentObject;
	SCN_Object *TargetObject;

	int	EndBehavior;
	int	PathAlignment;

	int	NrKeyframes;
	SCN_Keyframe6 *Keyframes;

	LGT_Header *LGT;		//Offset of light's LGT-structure

private:
};//SCN_Light


class SCN_Camera
{
public:
	SCN_Camera(void);
	~SCN_Camera(void);

	float	Matrix[12];

	SCN_Object *ParentObject;
	SCN_Object *TargetObject;

	int	EndBehavior;
	int	PathAlignment;

	float	ZoomFactor;

	int	NrKeyframes;
	SCN_Keyframe6 *Keyframes;

private:

};//SCN_Camera




class SCN_Envelope
{
public:
	SCN_Envelope(void);
	~SCN_Envelope(void);

	int	EndBehavior;

	int	NrKeyframes;
	SCN_Keyframe1 *Keyframes;

	float	*Value;

private:
};//SCN_Envelope


class SCN_Header
{
public:
	SCN_Header(void);
	~SCN_Header(void);

	int 	Type;			//Objecttype
	int 	Plugins[16];		//Plugins...

	int	FirstFrame;
	int	LastFrame;
	float	FramesPerSecond;

	int	StartTime;		//Start of the animation
	float	CurrentFrame;

	SCN_Header *CloneOrigin;

	int	NrObjects;
	SCN_Object *Objects;

	int	NrLights;
	SCN_Light *Lights;

	SCN_Camera *Camera;

	int	NrEnvelopes;
	SCN_Envelope *Envelopes;

private:

};//SCN_Header



void SCN_Motion6(float InFrame, int NrKeyframes, SCN_Keyframe6 *Keyframes, float *Vector);
void SCN_Motion1(float InFrame, int NrKeyframes, SCN_Keyframe1 *Keyframes, float *Value);
void SCN_GetObjectMatrix(float InFrame, SCN_Object *Object, float *Matrix);
void SCN_GetLightMatrix(float InFrame, SCN_Light *Light, float *Matrix);
void SCN_GetCameraMatrix(float InFrame, SCN_Camera *Camera, float *Matrix);
void SCN_RUN(void);
void SceneLightPlugin(void);
void ZeroMatrix(float *Matrix);
void MulMatrix(const float *MatrixA, const float *MatrixB, float *MatrixC);




#endif


