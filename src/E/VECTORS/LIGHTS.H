/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LIGHTS.H
쿌uthor       : Cyberfish/TLS & Toe/DU
쿗ast update  : 08.02.1998
쿌ction       : Contains external definitions of variables/routines
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef LGT_included
#define LGT_included


class LGT_Header
{
public:
	int	L_Type;			//Type of light (distant, point, spot)
	void   *L_Plugins[16];		//plugins

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


private:
};

extern "C"	int		LIT_Init;
extern "C"	void ASM_CALL 	LIT_InitP(void);

extern "C"	void ASM_CALL 	L01(void);
extern "C"	void ASM_CALL 	L02(void);
extern "C"	void ASM_CALL 	L03(void);

#endif	//LGT_Included


