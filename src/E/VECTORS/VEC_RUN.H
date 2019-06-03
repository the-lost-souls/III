/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_RUN.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 08.08.1998
쿌ction       :
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef VEC_RUN_included
#define VEC_RUN_included

#include "w:\general\var.h"

class VEC_Vertice
{
public:
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

private:
};


class VEC_Material
{
public:
	VEC_Material(void);		//constructor
	~VEC_Material(void);		//destructor

	int 	M_Type;
	float	M_R;
	float	M_G;
	float	M_B;
	float	M_Luminosity;
	float	M_Diffuse;
	float 	M_Transparency;
	int	M_MapWidth;
	int	M_MapHeigth;
	void    *M_MapOffset;
private:
};


class VEC_Face
{
public:
	int 	F_NextOffset;		//Offset next part
	int	F_SortValue;		//1/Z 0.32 fixed point
	int	F_Type;			//Part-type
	int	F_Priority;
	float   F_NVX;			//Normal vector
	float	F_NVY;			//
	float	F_NVZ;			//
	VEC_Vertice *F_V0Offs;		//Offset vertice0
	VEC_Vertice *F_V1Offs;		//
	VEC_Vertice *F_V2Offs;		//
	VEC_Material *F_Material;	//
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

private:
};



class VEC_Light
{
public:
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

private:
};



class VEC_Header
{
public:
	VEC_Header(void);
	~VEC_Header(void);

	int 	H_Type;			//Objecttype
	int 	H_Plugins[16];		//Plugins...
 	float 	H_Matrix[12];		//Transformation matrix

	int	H_NrVertices;		//Number of vertices
	VEC_Vertice *H_Vertices;  	//Offset vertices
	int	H_NrFaces;		//Number of faces
	VEC_Face *H_Faces;		//Offset faces
	int	H_NrMaterials;		//Number of materials
	VEC_Material *H_Materials;	//Offset materials
	int	H_Memory;	  	//Number of bytes used by object


private:
};


#endif				//ifdef VEC_included
