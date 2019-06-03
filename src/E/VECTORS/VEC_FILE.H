/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_FILE.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 16.01.1999
쿌ction       :
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef VEC_FILE_included
#define VEC_FILE_included

#include "w:\general\var.h"
#include "w:\vectors\vec_run.h"

const uint32 VEC_FILE_ID = 'FCEV';		//FVEC
const uint32 Vert_ID 	 = 'TREV';		//VERT
const uint32 Face_ID	 = 'SLOP';		//POLS
const uint32 Matr_ID	 = 'RTAM';		//MATR
const uint32 Mspc_ID 	 = 'CPSM';		//MSPC
const uint32 VEC_FILE_Vertice_ID  = Vert_ID;
const uint32 VEC_FILE_Face_ID     = Face_ID;
const uint32 VEC_FILE_Material_ID = Matr_ID;
const uint32 VEC_FILE_MatrSub_ID  = Mspc_ID;


enum TYPE { 	TYPE_Flat_Color,
		TYPE_Flat_ColorMap,
		TYPE_Flat_EnvMap,
		TYPE_Flat_Transparent,
		TYPE_Flat_Mixed,
		TYPE_Flat_Glass,
		TYPE_Flat_TransparentMap,
		TYPE_Flat_TransparentEnvMap,
		TYPE_Flat_TransparentMap_Additive,
		TYPE_Flat_TransparentEnvMap_Additive };


class VEC_FILE_Header 		//Fileheader for the VEC-format
{
public:
				//General file-info
	char    IDString[256];  //ID-string. Should be ended with EOF (1Ah)
	uint32 	ID;		//Chunk-ID
	uint32	Size;		//Chunk-size
	uint32	Version;	//Version of clwo that wrote the file
	uint32	FileSize;	//Size of the file
	uint32	Checksum;	//Checksum of the file

				//Info about the vectorobject
	uint32  Type;		//Object's type
	uint32	Vertices;	//Number of vertices
	uint32	Faces;		//Number of faces
	uint32	Materials;	//Number of materials

private:
};




class VEC_FILE_Vertice		//Structure for vertices (VEC-file)
{
public:
	float	X, Y, Z;	//The coordinate
private:

};




class VEC_FILE_VertChunk	//Structure for verticechunk (VEC-file)
{

public:
	uint32	ID;		//ChunkID
	uint32	Size;		//Chunksize
	VEC_FILE_Vertice Vertices[];

private:
};




class VEC_FILE_Face		//Structure for faces (VEC-file)
{
public:
	uint32	Vert0;		//indexes into verticetable
	uint32	Vert1;		//
	uint32	Vert2;		//

	float	U0, V0;		//Mapping coordinates
	float	U1, V1;		//
	float	U2, V2;		//

	uint32	Material;	//Index into material-list
private:
};




class VEC_FILE_FaceChunk 	//Structure for facechunk (VEC-file)
{
public:
	uint32	ID;		//Chunk-id
	uint32	Size;		//Chunksize
	VEC_FILE_Face Faces[];	//Faces
private:
};




class VEC_FILE_Material		//Structure for materialsubchunk (VEC-file)
{
public:
	uint32	ID;		//Chunk-id
	uint32	Size;		//Chunksize

	TYPE 	Type;		//The material's type

	float	R, G, B;	//The material's colour
	float	Luminosity;	//Dif. materialattributes
	float	Diffuse;	//
	float	Transparency;	//

	char	ColorMap[SYS_FileNameLength];//Filename of texture. 0-terminated.
	char	EnvMap[SYS_FileNameLength];
	char	BumpMap[SYS_FileNameLength];
private:
};


class VEC_FILE_MaterialChunk	//Structure for material chunk (VEC-file)
{
public:
	uint32	ID;		//Chunk-id
	uint32	Size;		//Chunksize

	VEC_FILE_Material Materials[];	//Materials
private:
};



VEC_Header *VEC_Load(const char *FileName);
VEC_Header *VEC_Load(VEC_FILE_Header *VECFile);


#endif				//ifdef VEC_included
