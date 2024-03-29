/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : VEC_RUN.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 15.08.1998
쿌ction       : Constructors/destructors for VEC-structures.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include "w:\general\txt.h"
#include "w:\vectors\vec_run.h"
#include "w:\vectors\vec_file.h"
#include <mem.h>


/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Header::VEC_Header
쿔D           : VEC
쿗ast update  : 08.08.1998
쿌ction	      : Constructor
�
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
VEC_Header::VEC_Header(void)
{
	H_Type = 0;
	for ( int C = 0; C < 16; C++ ) H_Plugins[C] = 0;

	H_NrVertices 	= 0;
	H_Vertices 	= NULL;
	H_NrFaces 	= 0;
	H_Faces		= NULL;
	H_NrMaterials 	= 0;
	H_Materials	= NULL;

	H_Memory 	= 0;
}//constructor



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Header::~VEC_Header
쿔D           : VEC
쿗ast update  : 08.08.1998
쿌ction	      : Destructor
�
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
VEC_Header::~VEC_Header(void)
{
	delete [] H_Vertices;
	delete [] H_Faces;
	delete [] H_Materials;
}//destructor



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Material::VEC_Material
쿔D           : VEC
쿗ast update  : 08.08.1998
쿌ction	      : Constructor
�
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
VEC_Material::VEC_Material(void)
{
	M_Type = TYPE_Flat_Color;

	M_R = (float)200/256;
	M_G = (float)200/256;
	M_B = (float)200/256;

	M_Luminosity 	= 0.0;
	M_Diffuse	= 1.0;
	M_Transparency	= 0.0;

	M_MapWidth	= 0;
	M_MapHeigth	= 0;
	M_MapOffset	= NULL;
}//constructor



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Material::~VEC_Material
쿔D           : VEC
쿗ast update  : 08.08.1998
쿌ction	      : Destructor
�
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
VEC_Material::~VEC_Material(void)
{
	if ( M_MapOffset != NULL )
		TXT_Unload(M_MapOffset);

}//destructor


