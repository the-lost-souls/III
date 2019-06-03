/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : VEC_RUN.CPP
³Author       : Cyberfish/TLS
³Last update  : 15.08.1998
³Action       : Constructors/destructors for VEC-structures.
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#include "w:\general\txt.h"
#include "w:\vectors\vec_run.h"
#include "w:\vectors\vec_file.h"
#include <mem.h>


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : VEC_Header::VEC_Header
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : VEC_Header::~VEC_Header
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
VEC_Header::~VEC_Header(void)
{
	delete [] H_Vertices;
	delete [] H_Faces;
	delete [] H_Materials;
}//destructor



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : VEC_Material::VEC_Material
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : VEC_Material::~VEC_Material
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
VEC_Material::~VEC_Material(void)
{
	if ( M_MapOffset != NULL )
		TXT_Unload(M_MapOffset);

}//destructor


