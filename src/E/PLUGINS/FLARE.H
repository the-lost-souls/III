/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : FLARE.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 25.01.99
쿌ction       : Flaredefinitions.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef FLR_included
#define FLR_included


const uint32 FLR_FlagDefault 		= 0x0;
const uint32 FLR_FlagFadeBehindObject	= 0x1;
const uint32 FLR_FlagFadeOffScreen      = 0x2;
const uint32 FLR_FlagFadeWithLight	= 0x4;

class FLR_Data
{
public:
	FLR_Data(void);
	~FLR_Data(void);

	void   *NextOffset;		//Offset next part
	uint32	SortValue;		//1/Z 0.32 fixed point
	uint32	Type;			//Part-type
	uint32	Priority;

	uint32	Flags;

	float 	Scale;

	float	R, G, B;
	float	Intensity;
	float	RealIntensity;

	int	X, Y, Z;
	float	XMin, YMin, XMax, YMax;	//Upper left, lower right

	float	MapWidth, MapHeigth;
	void   *Map;

private:
};


		void 		FLR_ObjectPlugin(void);
		void  		FLR_LightPlugin(void);
		int 		FLR_Setup(FLR_Data *FLR, const char *TGAFile);
extern "C" 	void 		ASM_CALL FLR_Draw(void);
extern 		uint32		FLR_Priority;


#endif //FLR_included

