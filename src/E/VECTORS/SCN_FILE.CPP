/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_FILE.CPP
³Author       : Cyberfish/TLS
³Last update  : 25.01.1999
³Action       : Loads a vectorscene (.SCN)
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"
#include "w:\vectors\vec_run.h"
#include "w:\vectors\scn_file.h"
#include "w:\pak\pak.h"
#include "w:\plugins\flare.h"
#include "w:\general\txt.h"
#include <stdio.h>
#include <mem.h>
#include <math.h>

int SCN_SetupObject(SCN_FILE_Object *In, SCN_Object *Out, uint32 Flags);
int SCN_SetupLight(SCN_FILE_Light *In, SCN_Light *Out);
int SCN_SetupCamera(SCN_FILE_Camera *In, SCN_Camera *Out);
int SCN_SetupEnvelope(SCN_FILE_Envelope *In, SCN_Envelope *Out);


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Header::SCN_Header
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Header::SCN_Header(void)
{
	Type		= 19;
	for ( int C1 = 0; C1 < 16; C1++ ) Plugins[C1] = -1;

	FirstFrame 	= 0;
	LastFrame 	= 0;
	FramesPerSecond = 0;

	CloneOrigin     = NULL;		//NULL = This is NOT a clone.

	NrObjects	= 0;
	Objects		= NULL;

	NrLights	= 0;
	Lights		= NULL;

	Camera		= NULL;

	NrEnvelopes	= 0;
	Envelopes	= NULL;
}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Header::~SCN_Header
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Header::~SCN_Header(void)
{
	if ( CloneOrigin != NULL )
	{
		for ( int C = 0; C < NrObjects; C++ )
			Objects[C].VEC = NULL;
	}
	delete [] Objects;
	delete [] Lights;
	delete Camera;
	delete [] Envelopes;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Object::SCN_Object
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Object::SCN_Object(void)
{
	CacheTime1	= -1.0;
	memset(CacheMatrix1, 0, sizeof(CacheMatrix1));
	CacheTime2	= -2.0;
	memset(CacheMatrix2, 0, sizeof(CacheMatrix2));

	memset(Matrix, 0, sizeof(Matrix));

	XPivot 		= 0;
	YPivot 		= 0;
	ZPivot 		= 0;

	ParentObject 	= NULL;
	TargetObject 	= NULL;

	EndBehavior	= 0;
	PathAlignment	= 0;

	NrKeyframes	= 0;
	Keyframes	= NULL;

	VEC		= NULL;

}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Object::~SCN_Object
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Object::~SCN_Object(void)
{
	delete [] Keyframes;
	delete VEC;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Light::SCN_Light
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Light::SCN_Light(void)
{
	memset(Matrix, 0, sizeof(Matrix));

	ParentObject 	= NULL;
	TargetObject 	= NULL;

	EndBehavior   	= 0;
	PathAlignment 	= 0;

	NrKeyframes 	= 0;
	Keyframes 	= NULL;

	LGT		= NULL;

}//constructor

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Light::~SCN_Light
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Light::~SCN_Light(void)
{
	delete [] Keyframes;
	delete LGT;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Camera::SCN_Camera
³ID           : SCN
³Last update  : 25.01.1999
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Camera::SCN_Camera(void)
{
	memset(Matrix, 0, sizeof(Matrix));

	ParentObject 	= NULL;
	TargetObject 	= NULL;

	EndBehavior   	= 0;
	PathAlignment 	= 0;

	ZoomFactor	= 3.2;

	NrKeyframes 	= 0;
	Keyframes 	= NULL;
}//constructor

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Camera::~SCN_Camera
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Camera::~SCN_Camera(void)
{
	delete [] Keyframes;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Envelope::SCN_Envelope
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Envelope::SCN_Envelope(void)
{
	EndBehavior	= 0;

	NrKeyframes	= 0;
	Keyframes	= NULL;

	Value		= NULL;
}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Envelope::~SCN_Envelope
³ID           : SCN
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
SCN_Envelope::~SCN_Envelope(void)
{
	delete [] Keyframes;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Load
³Last update  : 14.08.1998
³Action       : Loads a scene-file (.SCN)
³
³Input variables : Filename of .scn-file
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

SCN_Header *SCN_Load(const char *FileName, uint32 Flags, SCN_Header *CloneOrigin)
{

//---- Reading .scn-file from disk
	SCN_FILE_Header *SCNFile = (SCN_FILE_Header*)PAK_ReadFile(FileName);
	if ( SCNFile == NULL )
	{
		SYS_Error(1, "SCN_Load: Error reading .scn-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if

//---- Loading vector...
	SCN_Header *SCN = SCN_Load(SCNFile, Flags, CloneOrigin);
	if ( SCN == NULL )
	{
		SYS_Error(1, "SCN_Load: Error running SCN_Load.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		delete SCNFile;
		return NULL;
	}//if

	delete SCNFile;
	return SCN;
}//SCN_Load



SCN_Header *SCN_Load(const char *FileName, uint32 Flags)
{

//---- Reading .scn-file from disk
	SCN_FILE_Header *SCNFile = (SCN_FILE_Header*)PAK_ReadFile(FileName);
	if ( SCNFile == NULL )
	{
		SYS_Error(1, "SCN_Load: Error reading .scn-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if

//---- Loading vector...
	SCN_Header *SCN = SCN_Load(SCNFile, SCN_FILE_FlagDefault, NULL);
	if ( SCN == NULL )
	{
		SYS_Error(1, "SCN_Load: Error running SCN_Load.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		delete SCNFile;
		return NULL;
	}//if

	delete SCNFile;
	return SCN;
}//SCN_Load

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_Load
³Last update  : 25.01.1999
³Action       : Loads a scene-file (.SCN)
³
³Input variables : Pointer to SCN_FILE_Header
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

SCN_Header *SCN_Load(SCN_FILE_Header *SCNFile, uint32 Flags, SCN_Header *CloneOrigin)
{
//---- Testing file's header...
	if ( SCNFile->ID != SCN_FILE_ID )
	{
		SYS_Error(1, "SCN_Load: Corrupt SCN_FILE_ID.\n");
		return NULL;
	}//if

	int C1 = 0;
	int C2 = 0;
	int C3 = 0;


//---- Testing checksum...
	C2 = SCNFile->Checksum;
	SCNFile->Checksum = 0;

	byte *P = (byte*)SCNFile;
	C3 = 0;

	for ( C1 = 0; C1 < SCNFile->FileSize; C1++ )
		C3 += (uint32)*(uint8*)&P[C1];

	if ( C3 != C2 )
	{
		SYS_Error(1, "SCN_Load: Checksum failure.n\n");
		return NULL;
	}//if


//---- Allocate memory...
	SCN_Header *SCN = new SCN_Header;
	if ( SCN == NULL )
	{
		SYS_Error(1, "SCN_Load: Not enough memory for SCN-header.\n");
		return NULL;
	}//if

	SCN->Objects = new SCN_Object[SCNFile->NrObjects];
	if ( SCN->Objects == NULL )
	{
		SYS_Error(1, "SCN_Load: Not enough memory for SCN-objects.\n");
		return NULL;
	}//if

	SCN->Lights = new SCN_Light[SCNFile->NrLights];
	if ( SCN->Lights == NULL )
	{
		SYS_Error(1, "SCN_Load: Not enough memory for SCN-lights.\n");
		return NULL;
	}//if

	SCN->Envelopes = new SCN_Envelope[SCNFile->NrEnvelopes];
	if ( SCN->Envelopes == NULL )
	{
		SYS_Error(1, "SCN_Load: Not enough memory for SCN-envelopes.\n");
		return NULL;
	}//if

	SCN->Camera = new SCN_Camera;
	if ( SCN->Camera == NULL )
	{
		SYS_Error(1, "SCN_Load: Not enough memory for SCN-camera.\n");
		return NULL;
	}//if


//---- Setting up SCN-header...
	SCN->FirstFrame	= SCNFile->FirstFrame;
	SCN->LastFrame	= SCNFile->LastFrame;
	SCN->FramesPerSecond = SCNFile->FramesPerSecond;

	SCN->NrObjects 	= SCNFile->NrObjects;
	SCN->NrLights	= SCNFile->NrLights;
	SCN->NrEnvelopes= SCNFile->NrEnvelopes;

	int CurObject = 0;
	int CurLight = 0;
	int CurEnvelope = 0;

//---- Testing cloneorigin...
	if ( CloneOrigin != NULL )
	{
		SCN->CloneOrigin = CloneOrigin;
		if ( CloneOrigin->NrObjects != SCN->NrObjects )
		{
			SYS_Error(1, "SCN_Load: Error cloning scene. NrObjects not equal.\n");
			return NULL;
		}//if
	}//if


//---- Reading file...
	P = (byte*)SCNFile;
	C1 = sizeof(SCN_FILE_Header);

	int BytesRead = sizeof(SCN_FILE_Header);
	do
	{
		switch ( *(uint32*)&P[C1] )
		{
			case SCN_FILE_Object_ID:	if ( SCN->CloneOrigin != NULL)
							{
								if ( SCN_SetupObject((SCN_FILE_Object*)&P[C1], &SCN->Objects[CurObject], Flags | SCN_FILE_FlagIgnoreObjects) == 0 )
								{
									SYS_Error(1, "SCN_Load: Error setting up object.\n");
									return NULL;
								}//if
								SCN->Objects[CurObject].VEC = CloneOrigin->Objects[CurObject].VEC;;

							}//if
							else
							if ( SCN_SetupObject((SCN_FILE_Object*)&P[C1], &SCN->Objects[CurObject], Flags) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up object.\n");
								return NULL;
							}//if

							if ( !(Flags & SCN_FILE_FlagIgnoreObjects) )
							{
								//Converting parentobject from index to offset
								if ( SCN->Objects[CurObject].ParentObject == (SCN_Object*)-1 )
									SCN->Objects[CurObject].ParentObject = NULL;
								else SCN->Objects[CurObject].ParentObject = &SCN->Objects[(int)SCN->Objects[CurObject].ParentObject];

								//Converting targetobject from index to offset
								if ( SCN->Objects[CurObject].TargetObject == (SCN_Object*)-1 )
									SCN->Objects[CurObject].TargetObject = NULL;
								else SCN->Objects[CurObject].TargetObject = &SCN->Objects[(int)SCN->Objects[CurObject].TargetObject];
							}//if ignoreobjects

							CurObject++;
						 	break;

			case SCN_FILE_Light_ID:		if ( SCN_SetupLight((SCN_FILE_Light*)&P[C1], &SCN->Lights[CurLight]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up light.\n");
								return NULL;
							}//if

							//Converting parentobject from index to offset
							if ( SCN->Lights[CurLight].ParentObject == (SCN_Object*)-1 )
								SCN->Lights[CurLight].ParentObject = NULL;
							else SCN->Lights[CurLight].ParentObject = &SCN->Objects[(int)SCN->Lights[CurLight].ParentObject];

							//Converting targetobject from index to offset
							if ( SCN->Lights[CurLight].TargetObject == (SCN_Object*)-1 )
								SCN->Lights[CurLight].TargetObject = NULL;
							else SCN->Lights[CurLight].TargetObject = &SCN->Objects[(int)SCN->Lights[CurLight].TargetObject];

							CurLight++;
							break;

			case SCN_FILE_Camera_ID:	if ( SCN_SetupCamera((SCN_FILE_Camera*)&P[C1], SCN->Camera) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up camera.\n");
								return NULL;
							}//if

							//Converting parentobject from index to offset
							if ( SCN->Camera->ParentObject == (SCN_Object*)-1 )
								SCN->Camera->ParentObject = NULL;
							else SCN->Camera->ParentObject = &SCN->Objects[(int)SCN->Camera->ParentObject];

							//Converting targetobject from index to offset
							if ( SCN->Camera->TargetObject == (SCN_Object*)-1 )
								SCN->Camera->TargetObject = NULL;
							else SCN->Camera->TargetObject = &SCN->Objects[(int)SCN->Camera->TargetObject];

							break;

			case SCN_FILE_LightIntensity_ID:if ( SCN_SetupEnvelope((SCN_FILE_Envelope*)&P[C1], &SCN->Envelopes[CurEnvelope]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up envelope.\n");
								return NULL;
							}//if
							SCN->Envelopes[CurEnvelope].Value = &SCN->Lights[CurLight - 1].LGT->L_Intensity;
							CurEnvelope++;
							break;

			case SCN_FILE_LightRange_ID:	if ( SCN_SetupEnvelope((SCN_FILE_Envelope*)&P[C1], &SCN->Envelopes[CurEnvelope]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up envelope.\n");
								return NULL;
							}//if
							SCN->Envelopes[CurEnvelope].Value = &SCN->Lights[CurLight - 1].LGT->L_MaxRange;
							CurEnvelope++;
							break;

			case SCN_FILE_LightEdgeAngle_ID:if ( SCN_SetupEnvelope((SCN_FILE_Envelope*)&P[C1], &SCN->Envelopes[CurEnvelope]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up envelope.\n");
								return NULL;
							}//if
							SCN->Envelopes[CurEnvelope].Value = &SCN->Lights[CurLight - 1].LGT->L_EdgeAngle;
							CurEnvelope++;
							break;

			case SCN_FILE_LightConeAngle_ID:if ( SCN_SetupEnvelope((SCN_FILE_Envelope*)&P[C1], &SCN->Envelopes[CurEnvelope]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up envelope.\n");
								return NULL;
							}//if
							SCN->Envelopes[CurEnvelope].Value = &SCN->Lights[CurLight - 1].LGT->L_ConeAngle;
							CurEnvelope++;
							break;

			case SCN_FILE_LLensFlare_ID:	{
								SCN_FILE_LensFlare *F = (SCN_FILE_LensFlare*)&P[C1];


								FLR_Data *FTmp = new FLR_Data;
								if ( !FTmp )
								{
									SYS_Error(1, "SCN_Load: Error allocating memory for lensflare.\n");
									return NULL;
								}//if

								if ( !FLR_Setup(FTmp, F->MapFile))
								{
									SYS_Error(1, "SCN_Load: Error setting up lensflare.\n");
									delete FTmp;
									return NULL;
								}//if

								FTmp->Scale = F->FlareSize/TXT_ScalingFactor;
								FTmp->R = SCN->Lights[CurLight - 1].LGT->L_R;
								FTmp->G = SCN->Lights[CurLight - 1].LGT->L_G;
								FTmp->B = SCN->Lights[CurLight - 1].LGT->L_B;
								FTmp->Flags = F->Flags;
								FTmp->RealIntensity = 1.0;

								SCN->Lights[CurLight - 1].LGT->L_Plugins[2] = &FLR_ObjectPlugin;
								SCN->Lights[CurLight - 1].LGT->L_Plugins[3] = FTmp;
								SCN->Lights[CurLight - 1].LGT->L_Plugins[4] = (void*)-1;

							}
							break;

			case SCN_FILE_OLensFlare_ID:	{
								SCN_FILE_LensFlare *F = (SCN_FILE_LensFlare*)&P[C1];

								FLR_Data *FTmp = new FLR_Data;
								if ( !FTmp )
								{
									SYS_Error(1, "SCN_Load: Error allocating memory for lensflare.\n");
									return NULL;
								}//if

								if ( !FLR_Setup(FTmp, F->MapFile))
								{
									SYS_Error(1, "SCN_Load: Error setting up lensflare.\n");
									delete FTmp;
									return NULL;
								}//if

								FTmp->Scale = F->FlareSize/TXT_ScalingFactor;
								FTmp->Flags = F->Flags;
								FTmp->RealIntensity = 1.0;

								SCN->Objects[CurObject - 1].VEC->H_Plugins[0] = (int)&FLR_ObjectPlugin;
								SCN->Objects[CurObject - 1].VEC->H_Plugins[1] = (int)FTmp;
								SCN->Objects[CurObject - 1].VEC->H_Plugins[2] = -1;

							}
							break;

			case SCN_FILE_ZoomFactor_ID:    if ( SCN_SetupEnvelope((SCN_FILE_Envelope*)&P[C1], &SCN->Envelopes[CurEnvelope]) == 0 )
							{
								SYS_Error(1, "SCN_Load: Error setting up envelope.\n");
								return NULL;
							}//if
							SCN->Envelopes[CurEnvelope].Value = &SCN->Camera->ZoomFactor;
							CurEnvelope++;
							break;

			default:			break;

		}//switch

		C1 += *(uint32*)&P[C1 + 4] + 8;

	} while ( C1 < SCNFile->FileSize );

	return SCN;
}//Vxx_Load



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SCN_SetupObject
³Last update  : 28.11.1998
³Action       :
³
³Input variables :
³
³Notes : ParentObject and TargetObject are INDEXES, not pointers!
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

int SCN_SetupObject(SCN_FILE_Object *In, SCN_Object *Out, uint32 Flags)
{

//---- Copying dif. values...
	Out->XPivot = In->XPivot;
	Out->YPivot = In->YPivot;
	Out->ZPivot = In->ZPivot;

	Out->EndBehavior	= In->EndBehavior;
	Out->PathAlignment	= In->PathAlignment;

	Out->ParentObject 	= (SCN_Object*)In->ParentObject;
	Out->TargetObject	= (SCN_Object*)In->TargetObject;

	Out->NrKeyframes 	= In->NrKeyframes;

	strcpy(Out->Name, In->Path);

//---- Allocating memory for keyframes...
	Out->Keyframes = new SCN_Keyframe6[Out->NrKeyframes];
	if ( Out->Keyframes == NULL )
	{
		SYS_Error(1, "SCN_SetupObject: Not enough memory for keyframes.\n");
		return 0;
	}//if


//---- Copying keyframes...
	for ( int C1 = 0; C1 < Out->NrKeyframes; C1++ )
		Out->Keyframes[C1] = *(SCN_Keyframe6*)&In->Keyframes[C1].Frame;



//---- Evt. loading object from external file...
	if ( !(Flags & SCN_FILE_FlagIgnoreObjects) )

	{
		if ( strcmp(In->Path, "NullObject") != 0 )
		{
			Out->VEC = VEC_Load( In->Path);
			if ( Out->VEC == NULL )
			{
				SYS_Error(1, "SCN_SetupObject: Error loading object.\n");
				SYS_Error(3, "  ( ", In->Path, " )\n");
				return 0;
			}//if
		}//if
		else
		{		//Setting up nullobject...
			Out->VEC = new VEC_Header;
			if ( Out->VEC == NULL )
			{
				SYS_Error(1, "SCN_SetupObject: Not enough memory for nullobject.\n");
				return 0;
			}//if

			Out->VEC->H_Type = 1;
		}//else

	}//if IgnoreObjects

	return 1;
}//SetupObject





/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SetupLight
³Last update  : 14.08.1998
³Action       :
³
³Input variables :
³
³
³Notes : ParentObject and TargetObject are INDEXES, not pointers!
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

int SCN_SetupLight(SCN_FILE_Light *In, SCN_Light *Out)
{
	int C1;

	Out->LGT = new LGT_Header;
	if ( Out->LGT == NULL )
	{
		SYS_Error(1, "SCN_SetupLight: Not enough memory for light.\n");
		return 0;
	}//if

	for ( C1 = 0; C1 < 16; C1++ ) Out->LGT->L_Plugins[C1] = (void*)-1;

//---- Setting up LGT-structure...
	Out->LGT->L_Type    	= In->Type;
	Out->LGT->L_Plugins[0]	= (void*)&SceneLightPlugin;
	Out->LGT->L_R       	= In->R;
	Out->LGT->L_G       	= In->G;
	Out->LGT->L_B       	= In->B;
	Out->LGT->L_MaxRange 	= In->Range;
	Out->LGT->L_Intensity	= In->Intensity;
	Out->LGT->L_ConeAngle	= In->ConeAngle;
	Out->LGT->L_EdgeAngle	= In->EdgeAngle;

	Out->LGT->L_EdgeAngle	= cos(Out->LGT->L_ConeAngle - Out->LGT->L_EdgeAngle);
	Out->LGT->L_ConeAngle	= cos(Out->LGT->L_ConeAngle);


//---- Setting up SCN_Light-structure
	Out->EndBehavior   = In->EndBehavior;
	Out->PathAlignment = In->PathAlignment;

	Out->ParentObject = (SCN_Object*)In->ParentObject;
	Out->TargetObject = (SCN_Object*)In->TargetObject;

	Out->NrKeyframes = In->NrKeyframes;

//---- Allocating memory for keyframes...
	Out->Keyframes = new SCN_Keyframe6[Out->NrKeyframes];
	if ( Out->Keyframes == NULL )
	{
		SYS_Error(1, "SCN_SetupLight: Not enough memory for keyframes.\n");
		return 0;
	}//if


//---- Copying keyframes...
	for (  C1 = 0; C1 < Out->NrKeyframes; C1++ )
		Out->Keyframes[C1] = *(SCN_Keyframe6*)&In->Keyframes[C1].Frame;

	return 1;
}//SetupLight




/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SetupCamera
³Last update  : 14.08.1998
³Action       :
³
³Input variables :
³
³
³Notes : ParentObject and TargetObject are INDEXES, not pointers!
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int SCN_SetupCamera(SCN_FILE_Camera *In, SCN_Camera *Out)
{
//---- Setting up SCN_Camera-structure...
	Out->EndBehavior 	= In->EndBehavior;
	Out->PathAlignment	= In->PathAlignment;

	Out->ParentObject	= (SCN_Object*)In->ParentObject;
	Out->TargetObject	= (SCN_Object*)In->TargetObject;

	Out->NrKeyframes	= In->NrKeyframes;

	Out->ZoomFactor		= In->ZoomFactor;


//---- Allocating memory for keyframes...
	Out->Keyframes = new SCN_Keyframe6[Out->NrKeyframes];
	if ( Out->Keyframes == NULL )
	{
		SYS_Error(1, "SCN_SetupCamera: Not enough memory for keyframes.\n");
		return 0;
	}//if


//---- Copying keyframes...
	for (  int C1 = 0; C1 < Out->NrKeyframes; C1++ )
		Out->Keyframes[C1] = *(SCN_Keyframe6*)&In->Keyframes[C1].Frame;

	return 1;
}//SetupCamera


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SetupIntensityEnv
³Last update  : 14.08.1998
³Action       : Sets up an envelope 1-keyframe motions
³
³Input variables :
³
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int SCN_SetupEnvelope(SCN_FILE_Envelope *In, SCN_Envelope *Out)
{
	Out->EndBehavior	= In->EndBehavior;

	Out->NrKeyframes	= In->NrKeyframes;

//---- Allocating memory for keyframes...
	Out->Keyframes = new SCN_Keyframe1[Out->NrKeyframes];
	if ( Out->Keyframes == NULL )
	{
		SYS_Error(1, "SCN_SetupEnvelope: Not enough memory for keyframes.\n");
		return 0;
	}//if


//---- Copying keyframes...
	for (  int C1 = 0; C1 < Out->NrKeyframes; C1++ )
		Out->Keyframes[C1] = *(SCN_Keyframe1*)&In->Keyframes[C1].Frame;

	return 1;
}//void
