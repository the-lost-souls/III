/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VAR.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 12.12.1998
쿌ction       : Contains external definitions of variables + standard structs.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef VAR_included
#define VAR_included



#define ASM_CALL __cdecl		//asm calling conventions

#ifndef TRUE				//boolean expressions
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#define NULL 0
#endif

typedef signed   char   sint8;		//signed byte
typedef unsigned char   uint8;		//unsigned byte
typedef signed   short  sint16;		//signed word
typedef unsigned short  uint16;		//unsigned word
typedef signed   long   sint32;		//signed dword
typedef unsigned long   uint32;		//unsigned dword

typedef	unsigned char 	byte;		//byte
typedef	short    int    word;		//word
typedef		 int	dword;		//double word

#define b (BYTE PTR )			//for use in asm-blocks
#define w (WORD PTR )
#define d (DWORD PTR )
#define q (QWORD PTR )

const float Pi = 3.14159265359;


const int SYS_FileNameLength = 1024;


/*
컴컴컴컴컴컴� System declarations for general realtime engine 컴컴컴컴컴컴컴
*/

#ifdef Aquarium_ENGINE

extern "C"      int     SYS_ORoutines[];	//
extern "C"      int     SYS_PRoutines[];	//
extern "C"      int     SYS_LRoutines[];	//
extern "C"	float   SYS_Ambient;

extern "C"	int	SYS_QuitFlag;

extern "C"	void   *SYS_OListOffs;         	//ObjectListOffset
extern "C"      void   *SYS_LListOffs; 		//LightListOffset
extern "C"      void   *SYS_POffs;     		//Plugin data-offset
extern "C"	void   *SYS_PCaller;

extern "C"	void   *SYS_OOffs;
extern "C"	void   *SYS_LOffs;

extern "C"	void   *SYS_DBufOffs;  		//Offset doublebuffer
extern "C"	int	SYS_DBufSize;   	//Size doublebuffer
extern "C"	int	SYS_DBufLine;   	//Bytes per line, doublebuffer
extern "C"	void   *SYS_ZBufOffs;  		//Offset Z-buffer
extern "C"	int	SYS_ZBufSize;   	//Size Z-buffer
extern "C"	int	SYS_ZBufLine;   	//Bytes per line, Z-buffer
extern "C"	float	SYS_ZBufFP;   		//ZBuffer, fixed point factor
extern "C"	uint32	SYS_ZBufAdd;   		//ZBuffer, optimizing stuff :)
extern "C"	void   *SYS_VideoMem;
extern "C"	float   SYS_XCenter;
extern "C"	float   SYS_YCenter;
extern "C"	float	SYS_XPerspective;       //Used for 3D->2D projection
extern "C"	float	SYS_YPerspective;       //
extern "C"	float	SYS_FPClipRight;
extern "C"	float	SYS_FPClipLeft;
extern "C"	float	SYS_FPClipTop;
extern "C"	float	SYS_FPClipBottom;
extern "C"	float	SYS_ZClip;

extern "C"	int	SYS_XRes;
extern "C"	int 	SYS_YRes;

extern "C"	int	SYS_TimerTick;
extern "C"	int	SYS_NrFrames;
const 		int 	SYS_TicksPerSec = 65536;

#endif		//ifdef Aquarium_ENGINE
/*
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
*/





extern		int	Aquarium(int StartupMsg = TRUE);
extern		void	SYS_Error(int NrMsgs, const char *ErrorMsg, ... );

extern "C" 	char   *SYS_QuitMsg;
extern "C"	char   *SYS_QuitMsg_;

extern "C"	short int     	FPU_Original;		//The original controlword
extern "C"      short int	FPU_24;			//
extern "C"	short int	FPU_Round;		//Controlword for round
extern "C"	short int	FPU_Chop;		//Controlword for chop
extern "C"	short int	FPU_RoundUp;		//Controlword for round up
extern "C"	short int	FPU_RoundDown;		//Controlword for round down




/*
컴컴컴컴컴컴컴컴컴컴컴컴컴컴 General System structures 컴컴컴컴컴컴컴컴컴컴컴�
*/


const int M_Size = 4*9;
typedef struct{				//Datastructure for standard material
	int	M_Type;
	float	M_R;
	float	M_G;
	float	M_B;
	float	M_Luminosity;
	float	M_Diffuse;
	float 	M_Transparency;
	int	M_MapWidth;
	int	M_MapHeigth;
	void    *M_MapOffset;

} Material;



const int V_Size = 96;
typedef struct{				//Datastructure for standard vertex
	float	V_X;			//original coordinate
	float	V_Y;			//
	float	V_Z;			//
	float	V_InvZ;			//1/Z
	float	V_X2D;			//projected coordinate
	float	V_Y2D;			//
	float 	V_XRotated;		//rotated coordinate
	float	V_YRotated;
	float	V_ZRotated;
	float	V_U;			//mapping coordinate
	float	V_V;
	float	V_NVX;			//normalvector
	float	V_NVY;
	float	V_NVZ;
	float	V_IntensityR;		//calculated lightintensity
	float	V_IntensityG;
	float	V_IntensityB;
	int	V_ZFlag;		//Z-flag (in front of/behind camera)
	int	V_RFlag;		//Rotation flag
	int	V_PFlag;                //Position flag outside/inside screen
	int	V_Pad[4];

} Vertice;





const int F_Size = 96;
typedef struct{				//Datastructure for standard face
	int 	F_NextOffset;		//Offset next part
	int	F_SortValue;		//1/Z 0.32 fixed point
	int	F_Type;			//Part-type
	int	F_Priority;
	float   F_NVX;			//Normal vector
	float	F_NVY;			//
	float	F_NVZ;			//
	Vertice *F_V0Offs;		//Offset vertice0
	Vertice	*F_V1Offs;		//
	Vertice *F_V2Offs;		//
	Material*F_Material;             //
	float	F_IntensityR;		//Calculated light-intensity
	float	F_IntensityG;		//
	float	F_IntensityB;		//
	float	F_U0;			//Mapping coordinates
	float	F_V0;			//
	float	F_U1;			//
	float	F_V1;			//
	float	F_U2;			//
	float	F_V2;			//
	int	F_ZFlag;		//

	int	F_Pad[3];
} Face;






const int H_Size = 256;
typedef struct{				//Datastructure for standard vectorobject header
	int 	H_Type;			//Objecttype
	int 	H_Plugins[16];		//Plugins...
 	float 	H_Matrix[12];		//Transformation matrix
	int	H_Vertices;		//Number of vertices
	Vertice *H_VertOffs;		//Offset vertices
	int	H_Faces;		//Number of faces
	Face	*H_FaceOffs;		//Offset faces
	int	H_Materials;		//Number of materials
	Material *H_MatrOffs;		//Offset materials
	int	H_Memory;		//Number of bytes used by object

} Header;






typedef struct{				//Datastructure for standard light
	int	L_Type;			//Type of light (distant, point, spot)
	int	L_Plugins[16];		//plugins
	float   L_Matrix[12];		//Orientation matrix
	float	L_Pos[3];		//
	float	L_R;			//color
	float	L_G;			//
	float	L_B;                    //
	float	L_Intensity;		//current intensity
	float	L_MaxRange;		//maximum range of light
	float	L_Falloff;		//Falloff-value
	float	L_ConeAngle;		//
	float	L_EdgeAngle;
} Light;


typedef struct
{
	int StartFlag;
	int StartTime;
	int NrKeys;
	int CurrentKey;
	int TimingKeys[255];
}TIM_Data;




typedef struct
{
	int Type;
	int Plugins[16];
}NullObject;

/*
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
*/


#endif		//ifdef VAR_included
