#include "w:\general\var.h"

#ifndef _2D_included
#define _2D_included

extern "C"	void ASM_CALL	PIC(void);
extern "C"	void ASM_CALL	BLN(void);
extern "C"	void ASM_CALL	FOG(void);
extern "C"	void ASM_CALL	INV(void);

typedef struct
{
	int	PIC_Type;			//Objecttype
	int 	PIC_Plugins[16];		//Plugins...
	int	PIC_XPos;
	int	PIC_YPos;
	int	PIC_Width;
	int	PIC_Heigth;
	char   *PIC_Picture;
}PIC_Data;


typedef struct
{
	int	BLN_Type;			//Objecttype
	int 	BLN_Plugins[16];		//Plugins...

	float	BLN_R;
	float	BLN_G;
	float	BLN_B;

	float	BLN_Opacity0;
	float	BLN_Opacity1;

	int	BLN_StartFlag;
	int	BLN_StartTime;
	int	BLN_FadingTime;

}BLN_Data;


typedef struct
{
	int	FOG_Type;
	int	FOG_Plugins[16];

	float	FOG_R;
	float	FOG_G;
	float	FOG_B;

	int	FOG_Distance;

}FOG_Data;



typedef struct
{
	int	INV_Type;			//Objecttype
	int 	INV_Plugins[16];		//Plugins...

	int	INV_StartFlag;
	int	INV_StartTime;
	int	INV_FadingTime;

	float	INV_RMul;
	float	INV_GMul;
	float	INV_BMul;

	float	INV_Opacity0;
	float	INV_Opacity1;

}INV_Data;

#endif

