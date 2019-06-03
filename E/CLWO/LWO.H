/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : LWO.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 08.08.1998
쿌ction       :
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef LWO_included
#define LWO_included

#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"




enum LWO_Mapping {LWO_Flat, LWO_Sylindrical, LWO_Spherical};
const int LWO_XAxis = 1;
const int LWO_YAxis = 2;
const int LWO_ZAxis = 4;


class LWO_Point				//Structure for points (LWOFile)
{
public:
	float	X, Y, Z;
private:
};



class LWO_PointsChunk			//Structure for pointchunk (LWOFile)
{
public:
	uint32	ID;
	uint32	Size;
	float	Points[];
private:
};


class LWO_Polygon
{
public:
	uint16  NrVertices;
	uint16	Vertices[];		//Table with vertices + surfaceindex
private:
};



class LWO_PolygonChunk			//Structure for polygonchunk (LWOFile)
{
public:
	uint32	ID;			//ChunkID
	uint32	Size;			//ChunkSize

	uint16  Polygons[];		//Polygons
private:
};


class LWO_MaterialNameChunk		//Structure for materialnames (LWOFile)
{
public:
	uint32	ID;			//ChunkID
	uint32	Size;			//ChunkSize

	char	Names[];		//Materialnames
private:
};


class LWO_Texture
{
public:
	LWO_Texture(void);
	char	Filename[SYS_FileNameLength];

	float	XCenter, YCenter, ZCenter;
	float	XSize, YSize, ZSize;

	uint32	MappingType, MappingAxis;
private:
};




class LWO_Material			//Structure for LWO-materialdata
{
public:
	LWO_Material(void);

	float	R, G, B;
	float	Luminosity;
	float	Diffuse;
	float	Transparency;

	TYPE 	RenderType; 		//Type (flat, tmapped, envmapped etc...)
	uint32  Used;

	LWO_Texture ColorMap;
	LWO_Texture EnvMap;
	LWO_Texture BumpMap;

private:
};




class LWO_Conversion			//Data-structure for conversion
{
public:
	LWO_Conversion(void);
	~LWO_Conversion(void);

	void *LWOFile;

	LWO_PointsChunk 	*VerticesIn;
	LWO_PolygonChunk	*PolygonsIn;
	LWO_MaterialNameChunk   *MaterialNamesIn;

	LWO_Material 		*MaterialsIn;

	uint32	Vertices;		//Number of vertices
	uint32	Faces;			//Number of faces
	uint32	Materials;		//Number of materials

	VEC_FILE_Header		*HeaderOut;
	VEC_FILE_VertChunk	*VerticesOut;
	VEC_FILE_FaceChunk	*FacesOut;
	VEC_FILE_MaterialChunk	*MaterialsOut;

private:
};

VEC_FILE_Header *LWO_VEC(void *LWOFile, int Type = 0);





#endif 					//ifdef LWO_included
