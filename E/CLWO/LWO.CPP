/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO.CPP
³Author       : Cyberfish/TLS & Toe/DU
³Last update  : 16.01.1999
³Action       : Converts a .lwo-file into vec-format.
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"
#include "w:\clwo\lwo.h"
#include "w:\clwo\div.h"
#include "w:\pak\pak.h"

#include <math.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>

int LWO_Analyze(LWO_Conversion *Data);
int LWO_ReadPNTS(LWO_Conversion *Data);
int LWO_ReadSURF(LWO_Conversion *Data);
int LWO_ReadPOLS(LWO_Conversion *Data);

void LWO_MapCoords( float X,
		float Y,
		float Z,
		float &U,
		float &V,
		LWO_Material *Material);


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_Conversion (constructor)
³ID           : LWO
³Last update  : 29.09.1998
³
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
LWO_Conversion::LWO_Conversion(void)
{
	VerticesIn 	= NULL;
	PolygonsIn      = NULL;
	MaterialNamesIn = NULL;

	MaterialsIn     = NULL;

	Vertices 	= 0;
	Faces	 	= 0;
	Materials 	= 1;			//Default material

	HeaderOut	= NULL;
	VerticesOut	= NULL;
	FacesOut	= NULL;
	MaterialsOut	= NULL;

	LWOFile 	= NULL;
}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_Conversion (destructor)
³ID           : LWO
³Last update  : 30.07.1998
³
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
LWO_Conversion::~LWO_Conversion(void)
{
	delete MaterialsIn;

	delete HeaderOut;
	delete VerticesOut;
	delete FacesOut;
	delete MaterialsOut;
}//destructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_Material (constructor)
³ID           : LWO
³Last update  : 04.08.1998
³
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
LWO_Material::LWO_Material(void)
{
	R = (float)200/256;
	G = (float)200/256;
	B = (float)200/256;

	Luminosity = 0;
	Diffuse    = 1.00;
//	Specular   = 0;
	Transparency = 0;

	RenderType = TYPE_Flat_Color;
	Used = 0;

}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_Texture (constructor)
³ID           : LWO
³Last update  : 26.12.1998
³
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
LWO_Texture::LWO_Texture(void)
{
	memset(Filename, 0, SYS_FileNameLength);

	MappingType = LWO_Flat;
	MappingAxis = LWO_XAxis;

	XSize = 1e10;
	YSize = 1e10;
	ZSize = 1e10;
	XCenter = 0;
	YCenter = 0;
	ZCenter = 0;

}//constructor



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_VEC
³ID	      : LWO
³Last update  : 16.01.99
³Action	      : Converts an .lwo-file into vec-format
³
³In:	pointer to loaded .lwo-file and type
³
³Out:  Returns offset to converted file (VEC_FILE_Header)
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
VEC_FILE_Header *LWO_VEC( void *LWOFile, int Type )
{

	LWO_Conversion *Data = new LWO_Conversion;
	if ( Data == NULL )
	{
		SYS_Error(1, "LWO_VEC: Not enough memory.\n");
		return NULL;
	}//if

	Data->LWOFile = LWOFile;


//---- Analyzing file
	if ( LWO_Analyze(Data) == 0 )
	{
		SYS_Error(1, "LWO_VEC: Error analyzing LWO-data\n");
		delete Data;
		return NULL;
	}//if


//---- Reading and converting vertices...
	if ( LWO_ReadPNTS(Data) == 0 )
	{
		SYS_Error(1, "LWO_VEC: Error reading/converting vertices.\n");
		delete Data;
		return NULL;
	}//if


//---- Reading and converting surfaces...
	if ( LWO_ReadSURF(Data) == 0 )
	{
		SYS_Error(1, "LWO_VEC: Error reading/converting surfaces.\n");
		delete Data;
		return NULL;
	}//if


//---- Reading and converting polygons...
	if ( LWO_ReadPOLS(Data) == 0 )
	{
		SYS_Error(1, "LWO_VEC: Error reading/converting polygons.\n");
		delete Data;
		return NULL;
	}//if


//---- Setting up fileheader...
	Data->HeaderOut = new VEC_FILE_Header[1];
	if ( Data->HeaderOut == NULL )
	{
		SYS_Error(1, "LWO_VEC: Not enough memory.\n");
		delete Data;
		return NULL;
	}//if

	Data->HeaderOut->ID 	  	= VEC_FILE_ID;
	Data->HeaderOut->Size 	  	= sizeof(VEC_FILE_Header) - 264;
	Data->HeaderOut->Version   	= 210;
	Data->HeaderOut->FileSize 	= sizeof(VEC_FILE_Header)
					+ Data->VerticesOut->Size + 8
					+ Data->FacesOut->Size + 8
					+ Data->MaterialsOut->Size + 8;
	Data->HeaderOut->Checksum	= 0;
	Data->HeaderOut->Type		= Type;
	Data->HeaderOut->Vertices	= Data->Vertices;
	Data->HeaderOut->Faces		= Data->Faces;
	Data->HeaderOut->Materials	= Data->Materials;


//---- Getting object's type...
	if ( Data->HeaderOut->Type == 0 )
	{
		int C = 0;
		while ( Data->MaterialsIn[C].Used == 0 ) C++;

		TYPE OType = Data->MaterialsIn[C].RenderType;
		for ( ; (unsigned)C < Data->HeaderOut->Materials; C++ )
			if ( (Data->MaterialsIn[C].Used != 0) && (Data->MaterialsIn[C].RenderType != OType) )
			{
				OType = TYPE_Flat_Mixed;
				break;
			}//if


		switch ( OType )
		{
			case TYPE_Flat_Color          	:
				Data->HeaderOut->Type = 5;
				break;

			case TYPE_Flat_ColorMap       	:
				Data->HeaderOut->Type = 10;
				break;

			case TYPE_Flat_EnvMap         	:
				Data->HeaderOut->Type = 14;
				break;


			case TYPE_Flat_Transparent    	:
				Data->HeaderOut->Type = 12;
				break;

			case TYPE_Flat_Mixed	      	:
				Data->HeaderOut->Type = 20;
				break;

			case TYPE_Flat_Glass          	:
				Data->HeaderOut->Type = 21;
				break;

			case TYPE_Flat_TransparentMap 	:
				Data->HeaderOut->Type = 22;
				break;

			case TYPE_Flat_TransparentMap_Additive:
				Data->HeaderOut->Type = 24;
				break;

			case TYPE_Flat_TransparentEnvMap:
				Data->HeaderOut->Type = 23;
				break;

			case TYPE_Flat_TransparentEnvMap_Additive:
				Data->HeaderOut->Type = 25;
				break;

			default			      	:
				break;
		}//switch

	}//if


//---- Setting up IDString...
	memset(Data->HeaderOut->IDString, 0, 256);
	int P = sprintf(Data->HeaderOut->IDString, "CLWO version 2.10\nCyberfish/TLS\n");
	P += sprintf(&Data->HeaderOut->IDString[P], "  Type         : %6d\n", Data->HeaderOut->Type);
	P += sprintf(&Data->HeaderOut->IDString[P], "  NrVertices   : %6d\n", Data->HeaderOut->Vertices);
	P += sprintf(&Data->HeaderOut->IDString[P], "  NrFaces      : %6d\n", Data->HeaderOut->Faces);
	P += sprintf(&Data->HeaderOut->IDString[P], "  NrMaterials  : %6d\n", Data->HeaderOut->Materials);


//---- Setting up final memblock...
	byte *Out = new byte[Data->HeaderOut->FileSize];
	if ( Out == NULL )
	{
		SYS_Error(1, "LWO_VEC: Not enough memory.\n");
		delete Data;
		return NULL;
	}//if

	P = 0;
	memcpy((char*)&Out[P], (char*)Data->HeaderOut, sizeof(VEC_FILE_Header));
	P += sizeof(VEC_FILE_Header);
	memcpy((char*)&Out[P], (char*)Data->VerticesOut, Data->VerticesOut->Size + 8);
	P += Data->VerticesOut->Size + 8;
	memcpy((char*)&Out[P], (char*)Data->FacesOut, Data->FacesOut->Size + 8);
	P += Data->FacesOut->Size + 8;
	memcpy((char*)&Out[P], (char*)Data->MaterialsOut, Data->MaterialsOut->Size + 8);

	for ( P = 0; P < Data->HeaderOut->FileSize; P++ )
		Data->HeaderOut->Checksum += (uint32)*(unsigned char*)&Out[P];
	((VEC_FILE_Header*)Out)->Checksum = Data->HeaderOut->Checksum;

	delete Data;

	return (VEC_FILE_Header*)Out;
}//LWO_VEC



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_Analyze
³Last update  : 30.07.1998
³Action	      : Gets different info from the .lwo (faces, vertices etc.)
³
³In:	pointer to loaded LWO_Conversion-structure
³
³Out:  Returns an evt. error (0 = error)
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int LWO_Analyze(LWO_Conversion *Data)
{
	int C = 0;
	byte *P = (byte*)Data->LWOFile;


//---- Check FORM -chunk
	if ( *(int*)P != 'MROF' )
	{
		SYS_Error(1, "LWO_Analyze: Couldn't find FORM chunk\n");
		return NULL;

	}//if

	int LWOSize = Flip_32(*(int*)(P + 4));


//---- Check LWOB -chunk
	if ( *(int*)(P + 8) != 'BOWL')
	{
		SYS_Error(1, "LWO_Analyze: Couldn't find LWOB chunk\n");
		return 0;
	}//if


//---- Searching through chunks...
	int BytesRead = 4;
	P += 12;

	do
	{
		Flip_32((P + 4));
		int ChunkSize = *(int*)(P + 4) + 8;

		switch ( *(int*)P )
		{

		//---- Found PNTS chunk...
			case 'STNP':	Data->VerticesIn = (LWO_PointsChunk*)P;
					break;

			case 'SLOP':	Data->PolygonsIn = (LWO_PolygonChunk*)P;
					break;

			case 'SFRS':	Data->MaterialNamesIn = (LWO_MaterialNameChunk*)P;
					break;

			case 'FRUS':	Data->Materials++;
					break;
		}//switch

		P += ChunkSize;
		BytesRead += ChunkSize;
	} while ( BytesRead < LWOSize);


//---- Reversing data in PNTS-chunk
	for ( C = 0; C < Data->VerticesIn->Size >> 2; C++ )
		Flip_32(&Data->VerticesIn->Points[C]);


//---- Reversing data in POLS-chunk...
	for ( C = 0; C < Data->PolygonsIn->Size >> 1; C++ )
		Flip_16(&Data->PolygonsIn->Polygons[C]);


//---- Removing pad-zeros in MaterialNames...
	void *TempP = (void*)&Data->MaterialNamesIn->Names;
	int TempInt = Data->MaterialNamesIn->Size;

	__asm{
		pushad
		pushfd

		mov	esi, TempP
		mov	edi, TempP

		mov	ecx, TempInt
		xor	edx, edx
		xor	eax, eax
	StringFix:

		mov	al, [esi]		//load character
		mov	[edi], al		//save character

		inc	edx
		cmp	eax, 0
		jnz	StringNotFinished

		test	edx, 1
		mov	edx, 0
		jz	StringNotFinished
		inc	esi
		dec	ecx

	StringNotFinished:

		inc	edi
		inc	esi

		dec	ecx
		jnz	StringFix

		popfd
		popad
	}//asm

	return 1;
}//AnalyzeLWO*/





/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_ReadPNTS
³Last update  : 09.06.1998
³Action	      : Converts the LWO-coordinates
³
³In:	pointer to LWO_Conversion-structure
³
³Out:  -
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int LWO_ReadPNTS(LWO_Conversion *Data)
{

//---- Calculating number of vertices, allocating memory...
	Data->Vertices  = Data->VerticesIn->Size /12;

	int ChunkSize   = sizeof(VEC_FILE_VertChunk)
			+ Data->Vertices*sizeof(VEC_FILE_Vertice);

	Data->VerticesOut = (VEC_FILE_VertChunk*)new byte[ChunkSize];
	if ( Data->VerticesOut == NULL )
	{
		SYS_Error(1, "LWO_ReadPNTS: Not enough memory to convert points\n");
		return NULL;
	}//if

	Data->VerticesOut->ID   = Vert_ID;
	Data->VerticesOut->Size = ChunkSize - 8;	//don't include header


//---- Copying vertices...
	memcpy(&Data->VerticesOut->Vertices[0], &Data->VerticesIn->Points[0], Data->Vertices*12);

/*	for ( int C = 0; C < Data->Vertices; C++ )
	{
		Data->VerticesOut->Vertices.X = Data->VerticesIn[C*3 + 0];
		Data->VerticesOut->Vertices.Y = Data->VerticesIn[C*3 + 1];
		Data->VerticesOut->Vertices.Z = Data->VerticesIn[C*3 + 2];
	}//for*/

	return 1;
}//SavePNTS


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_ReadSURF
³Last update  : 16.01.1999
³Action	      : Reads all necessary data from material-subchunks
³
³In:	pointer to LWO_Conversion-structure
³
³Out:  evt. 0 if error.
³
³Notes: The first material is set up as the default material. That's the
³reason for C1 and S starting at 1 instead of 0.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int LWO_ReadSURF(LWO_Conversion *Data)
{
	int C1 = 0;
	int C2 = 0;
	int C3 = 0;

	Data->MaterialsIn = new LWO_Material[Data->Materials];
	if ( Data->MaterialsIn == NULL)
	{
		SYS_Error(1, "LWO_ReadSurf: Not enough memory\n");
		return 0;
	}//if

	byte *P = ( (byte*)Data->LWOFile + 12);

	C1 = 1;
	do
	{
		if ( *(int*)P == 'FRUS' )
		{
			int ChunkSize = *(int*)&P[4];

		//---- Search for the surface...
			C2 = 0;

			int S = 1;		//S = Surface
		   	while ( strcmp((char*)(P + 8), &Data->MaterialNamesIn->Names[C2] ) != 0)
			{
				C2 += strlen(&Data->MaterialNamesIn->Names[C2]) + 1;
				S++;
			}//while

			uint32 BytesRead = strlen(&Data->MaterialNamesIn->Names[C2]) + 1;
			if ( (BytesRead & 1) == 1) BytesRead++;

			P += BytesRead + 8;


		//---- Div. stuff for determining material's type...
			uint32 Texture = 0;
			uint32 Additive = FALSE;

		//---- Reading chunks...
			do
			{
				switch ( *(int*)&P[0] )
				{
				//COLR: ground color of surface
					case 'RLOC':    Data->MaterialsIn[S].R = (float)P[6]/255;
							Data->MaterialsIn[S].G = (float)P[7]/255;
							Data->MaterialsIn[S].B = (float)P[8]/255;
							break;


				//VLUM: floating point luminosity
					case 'MULV':	C2 = Flip_32(*(int*)&P[6]);
							Data->MaterialsIn[S].Luminosity = *(float*)&C2;
							break;
				//LUMI: fixed point luminosity
					case 'IMUL':    C2 = Flip_16(*(short int*)&P[6]);
							Data->MaterialsIn[S].Luminosity = (float)C2/256;
							break;


				//VDIF: floating point diffuse
					case 'FIDV':	C2 = Flip_32(*(int*)&P[6]);
							Data->MaterialsIn[S].Diffuse    = *(float*)&C2;
							break;
				//DIFF: fixed point diffuse
					case 'FFID':    C2 = Flip_16(*(short int*)&P[6]);
							Data->MaterialsIn[S].Diffuse    = (float)C2/256;
							break;

				//VTRN: floating point transparency
					case 'NRTV':	C2 = Flip_32(*(int*)&P[6]);
							Data->MaterialsIn[S].Transparency = *(float*)&C2;
							break;
				//TRAN: fixed point transparency
					case 'NART':    C2 = Flip_16(*(short int*)&P[6]);
							Data->MaterialsIn[S].Transparency = (float)C2/256;
							break;

				//TIMG: textstring with filename texture
					case 'GMIT':    switch ( Texture )
							{
								//Colortexture's filename
								case 1: strcpy((char*)&Data->MaterialsIn[S].ColorMap.Filename, (char*)&P[6]);
									for ( C2 = 0; C2 < strlen(Data->MaterialsIn[S].ColorMap.Filename); C2++ )
										Data->MaterialsIn[S].ColorMap.Filename[C2] = toupper(Data->MaterialsIn[S].ColorMap.Filename[C2]);
									break;

								//Bumptexture's filename
								case 2: strcpy((char*)&Data->MaterialsIn[S].BumpMap.Filename, (char*)&P[6]);
									for ( C2 = 0; C2 < strlen(Data->MaterialsIn[S].BumpMap.Filename); C2++ )
										Data->MaterialsIn[S].BumpMap.Filename[C2] = toupper(Data->MaterialsIn[S].BumpMap.Filename[C2]);
									break;

								default:break;


							}//switch
							break;

				//TSIZ: texture size
					case 'ZIST':    switch (Texture)
							{

								case 1:	C2 = Flip_32(*(int*)&P[6]);
									Data->MaterialsIn[S].ColorMap.XSize = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[10]);
									Data->MaterialsIn[S].ColorMap.YSize = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[14]);
									Data->MaterialsIn[S].ColorMap.ZSize = *(float*)&C2;
									break;

								case 2:	C2 = Flip_32(*(int*)&P[6]);
									Data->MaterialsIn[S].BumpMap.XSize = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[10]);
									Data->MaterialsIn[S].BumpMap.YSize = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[14]);
									Data->MaterialsIn[S].BumpMap.ZSize = *(float*)&C2;
									break;

								default:break;
							}//switch
							break;

				//TCTR: texture center
					case 'RTCT':
							switch ( Texture )
							{
								case 1:	C2 = Flip_32(*(int*)&P[6]);
									Data->MaterialsIn[S].ColorMap.XCenter = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[10]);
									Data->MaterialsIn[S].ColorMap.YCenter = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[14]);
									Data->MaterialsIn[S].ColorMap.ZCenter = *(float*)&C2;
									break;

								case 2:	C2 = Flip_32(*(int*)&P[6]);
									Data->MaterialsIn[S].BumpMap.XCenter = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[10]);
									Data->MaterialsIn[S].BumpMap.YCenter = *(float*)&C2;
									C2 = Flip_32(*(int*)&P[14]);
									Data->MaterialsIn[S].BumpMap.ZCenter = *(float*)&C2;
									break;

							}//switch
							break;

				//TFLG: texture flags
					case 'GLFT':    switch ( Texture )
							{
								case 1:	C2 = Flip_16(*(short int*)&P[6]);
									Data->MaterialsIn[S].ColorMap.MappingAxis = C2 & 7;
									break;

								case 2:	C2 = Flip_16(*(short int*)&P[6]);
									Data->MaterialsIn[S].BumpMap.MappingAxis = C2 & 7;
									break;

								default:break;
							}//switch
							break;

				//FLAG: different surface flags...
					case 'GALF':	C2 = Flip_16(*(short int*)&P[6]);
							if (C2 & 0x200) Additive = TRUE;
							break;

				//CTEX: texture mapping type (colormap)
					case 'XETC':	Texture = 1;	//CTEX

							Data->MaterialsIn[S].ColorMap.MappingType = 0;
					       		if ( strcmp("Planar Image Map", (char*)&P[6]) == 0 )
					       		{
					       			Data->MaterialsIn[S].ColorMap.MappingType = LWO_Flat;
								break;
							}//if
							if ( strcmp("Sylindrical Image Map", (char*)&P[6]) == 0 )
							{
								Data->MaterialsIn[S].ColorMap.MappingType = LWO_Sylindrical;
								break;
							}//if

							if ( strcmp("Spherical Image Map", (char*)&P[6]) == 0 )
							{
								Data->MaterialsIn[S].ColorMap.MappingType = LWO_Spherical;
								break;
							}//if

							SYS_Error(1, "LWO_ReadSURF: Mappingtype not supported (CTEX).\n");
							SYS_Error(3, "  (\"", (char*)&P[6], "\")\n");
							return 0;
							break;

				//BTEX: texture mapping type (bump texture)
					case 'XETB':	Texture = 2; //BTEX

							Data->MaterialsIn[S].BumpMap.MappingType = 0;
					       		if ( strcmp("Planar Image Map", (char*)&P[6]) == 0 )
					       		{
					       			Data->MaterialsIn[S].BumpMap.MappingType = LWO_Flat;
								break;
							}//if
							if ( strcmp("Sylindrical Image Map", (char*)&P[6]) == 0 )
							{
								Data->MaterialsIn[S].BumpMap.MappingType = LWO_Sylindrical;
								break;
							}//if

							if ( strcmp("Spherical Image Map", (char*)&P[6]) == 0 )
							{
								Data->MaterialsIn[S].BumpMap.MappingType = LWO_Spherical;
								break;
							}//if

							SYS_Error(1, "LWO_ReadSURF: Mappingtype not supported (BTEX).\n");
							SYS_Error(3, "  (\"", (char*)&P[6], "\")\n");
							return 0;
							break;

				//RIMG: textstring with environmentmap
					case 'GMIR':	strcpy((char*)&Data->MaterialsIn[S].EnvMap.Filename, (char*)&P[6]);
							for ( C2 = 0; C2 < strlen(Data->MaterialsIn[S].EnvMap.Filename); C2++ )
								Data->MaterialsIn[S].EnvMap.Filename[C2] = toupper(Data->MaterialsIn[S].EnvMap.Filename[C2]);

							break;


				//DTEX: Diffuse map
					case 'XETD':	Texture = 0;
							break;
				//STEX: Specular map
					case 'XETS':	Texture = 0;
							break;
				//RTEX: Reflection map
					case 'XETR':	Texture = 0;
							break;
				//TTEX: transparency map
					case 'XETT':	Texture = 0;
							break;
				//LTEX: Luminosity map
					case 'XETL':	Texture = 0;
							break;


					default:	break;
				} //switch

				BytesRead += Flip_16(*(short int*)&P[4]) + 6;
				P += Flip_16(*(short int*)&P[4]) + 6;
			} while ( BytesRead < ChunkSize );


		//---- Setting material's type...
			if ( Data->MaterialsIn[S].Transparency != 0.0 )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_Transparent;

			if ( Data->MaterialsIn[S].ColorMap.Filename[0] != '\0' )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_ColorMap;

			if ( Data->MaterialsIn[S].ColorMap.Filename[0] != '\0' &&
			     Data->MaterialsIn[S].Transparency != 0.0 )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_TransparentMap;

			if ( Data->MaterialsIn[S].ColorMap.Filename[0] != '\0' &&
			     Data->MaterialsIn[S].Transparency != 0.0 &&
			     Additive )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_TransparentMap_Additive;

			if ( Data->MaterialsIn[S].EnvMap.Filename[0] != '\0' )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_EnvMap;

			if ( Data->MaterialsIn[S].EnvMap.Filename[0] != '\0' &&
			     Data->MaterialsIn[S].Transparency != 0.0 )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_TransparentEnvMap;

			if ( Data->MaterialsIn[S].EnvMap.Filename[0] != '\0' &&
			     Data->MaterialsIn[S].Transparency != 0.0 &&
			     Additive )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_TransparentEnvMap_Additive;

			if ( Data->MaterialsIn[S].BumpMap.Filename[0] != '\0' &&
			     Data->MaterialsIn[S].Transparency != 0.0 )
				Data->MaterialsIn[S].RenderType = TYPE_Flat_Glass;


			C1++;
		}//if
		else P += *(int*)&P[ 4 ] + 8;	//Dump chunk
	} while ( C1 < Data->Materials );


//---- Allocating memory for VEC_FILE_Material-chunk...
	int ChunkSize = sizeof(VEC_FILE_MaterialChunk) + Data->Materials*sizeof(VEC_FILE_Material);

	Data->MaterialsOut = (VEC_FILE_MaterialChunk*)new byte[ChunkSize];
	if ( Data->MaterialsOut == NULL )
	{
		SYS_Error(1, "LWO_ReadSURF: Not enough memory.\n");
		return 0;
	}//if


//---- Setting up materialchunk...
	Data->MaterialsOut->ID 	 = Matr_ID;
	Data->MaterialsOut->Size = ChunkSize - 8;

	VEC_FILE_Material *M = &Data->MaterialsOut->Materials[0];

//---- Pointer to first out-material
	for ( C1 = 0; C1 < Data->Materials; C1++ )
	{
	//---- Setting header (ID + chunksize)
		M->ID   = Mspc_ID;
		M->Size = sizeof(VEC_FILE_Material) - 8;
		M->Type = Data->MaterialsIn[C1].RenderType;

	//---- Setting data...
		M->R = Data->MaterialsIn[C1].R;
		M->G = Data->MaterialsIn[C1].G;
		M->B = Data->MaterialsIn[C1].B;

		M->Luminosity 	= Data->MaterialsIn[C1].Luminosity;
		M->Diffuse 	= Data->MaterialsIn[C1].Diffuse;
		M->Transparency = Data->MaterialsIn[C1].Transparency;

		memset(M->ColorMap, 0, SYS_FileNameLength);
		memset(M->BumpMap, 0, SYS_FileNameLength);
		memset(M->EnvMap, 0, SYS_FileNameLength);

		strcpy(M->ColorMap, Data->MaterialsIn[C1].ColorMap.Filename);
		strcpy(M->BumpMap, Data->MaterialsIn[C1].BumpMap.Filename);
		strcpy(M->EnvMap, Data->MaterialsIn[C1].EnvMap.Filename);

		M++;
	}//for
	return 1;
}//ReadMaterials


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : SavePOLS
³Last update  : 30.07.1998
³Action	      : Converts faces from an .lwo-file
³
³In:	pointer to LWO_Conversion-structure
³
³Out:  evt. 0 if error
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int LWO_ReadPOLS(LWO_Conversion *Data)
{
	int Vert0, Vert1, Vert2;
	float X0, Y0, Z0, X1, Y1, Z1, X2, Y2, Z2;
	float U0, V0, U1, V1, U2, V2;

	LWO_Polygon *P = (LWO_Polygon*)&Data->PolygonsIn->Polygons;	//Pointer

//---- Counting faces...
	int BytesRead = 0;
	do
	{
		if ( P->NrVertices > 2 ) Data->Faces += P->NrVertices - 2;
		BytesRead += P->NrVertices*2 + 4;
		P = (LWO_Polygon*)( (byte*)P + P->NrVertices*2 + 4);
	} while ( BytesRead < Data->PolygonsIn->Size);


//---- Allocating memory for faces...
	Data->FacesOut = (VEC_FILE_FaceChunk*)new byte[sizeof(VEC_FILE_FaceChunk) + Data->Faces*sizeof(VEC_FILE_Face)];
	if ( Data->FacesOut == NULL )
	{
		SYS_Error(1, "LWO_ReadPOLS: Not enough memory.\n");
		return 0;
	}//if

//---- Converting and saving polygons as faces...
	int F = 0;					//Current face
	P = (LWO_Polygon*)&Data->PolygonsIn->Polygons;	//Pointer

	do
	{
		int NrVerts 	= (int)P->NrVertices;
		int Material 	= (int)P->Vertices[NrVerts];
			//Material is saved right after vertices

		Vert0 = P->Vertices[0];

		LWO_MapCoords(Data->VerticesOut->Vertices[Vert0].X,
			      Data->VerticesOut->Vertices[Vert0].Y,
			      Data->VerticesOut->Vertices[Vert0].Z,
			      U0, V0, &Data->MaterialsIn[Material]);

		while ( NrVerts >= 3 )
		{
			Vert1 = (int)P->Vertices[NrVerts - 2];
			Vert2 = (int)P->Vertices[NrVerts - 1];

			Data->FacesOut->Faces[F].Vert0 = Vert0;
			Data->FacesOut->Faces[F].Vert1 = Vert1;
			Data->FacesOut->Faces[F].Vert2 = Vert2;
			Data->FacesOut->Faces[F].Material = Material;
			Data->MaterialsIn[Material].Used++;


		//---- Calculating mappingcoords...
			LWO_MapCoords(	Data->VerticesOut->Vertices[Vert1].X,
			     		Data->VerticesOut->Vertices[Vert1].Y,
			     		Data->VerticesOut->Vertices[Vert1].Z,
			      		U1, V1, &Data->MaterialsIn[Material]);


			//LWO_MapCoords(X1, Y1, Z1, U1, V1, &Data->MaterialsIn[Material]);

			LWO_MapCoords(	Data->VerticesOut->Vertices[Vert2].X,
			      		Data->VerticesOut->Vertices[Vert2].Y,
			      		Data->VerticesOut->Vertices[Vert2].Z,
			      		U2, V2, &Data->MaterialsIn[Material]);



		//---- Moving mapcoords within 0->1
			int SmallestU = (int)U0;
			if ( (int)U1 < SmallestU ) SmallestU = (int)U1;
			if ( (int)U2 < SmallestU ) SmallestU = (int)U2;

			int SmallestV = (int)V0;
			if ( (int)V1 < SmallestV ) SmallestV = (int)V1;
			if ( (int)V2 < SmallestV ) SmallestV = (int)V2;

			float U0_ = U0 - SmallestU;
			float U1_ = U1 - SmallestU;
			float U2_ = U2 - SmallestU;
			float V0_ = V0 - SmallestV;
			float V1_ = V1 - SmallestV;
			float V2_ = V2 - SmallestV;


		//---- Saving final mapcoords...
			Data->FacesOut->Faces[F].U0 = U0_;
			Data->FacesOut->Faces[F].V0 = V0_;
			Data->FacesOut->Faces[F].U1 = U1_;
			Data->FacesOut->Faces[F].V1 = V1_;
			Data->FacesOut->Faces[F].U2 = U2_;
			Data->FacesOut->Faces[F].V2 = V2_;

			NrVerts--;
			F++;
		}//while

		P = (LWO_Polygon*)( (byte*)P + P->NrVertices*2 + 4);
	} while ( F < Data->Faces );


//---- Setting chunk-ID and size...
	Data->FacesOut->ID   = Face_ID;
	Data->FacesOut->Size = sizeof(VEC_FILE_FaceChunk) + Data->Faces*sizeof(VEC_FILE_Face);

	return 1;

}//LWO_ReadPOLS*/


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : LWO_MapCoords
³Last update  : 16.01.1999
³Action	      : Calculates mapping coordinates
³
³In:	The point's X, Y, Z and pointer to a LWO_Material-structure
³
³Out:   The calculated U and V
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void LWO_MapCoords( float X,
		float Y,
		float Z,
		float &U,
		float &V,
		LWO_Material *Material)
{
	LWO_Texture *Map = NULL;
	if ( Material->RenderType == TYPE_Flat_ColorMap ) Map = &Material->ColorMap;
	if ( Material->RenderType == TYPE_Flat_TransparentMap ) Map = &Material->ColorMap;
	if ( Material->RenderType == TYPE_Flat_Glass ) Map = &Material->BumpMap;
	if ( Material->RenderType == TYPE_Flat_TransparentMap_Additive ) Map = &Material->ColorMap;


	if ( Map->MappingType == LWO_Flat )
	{
		float XC = X - Map->XCenter;
		float YC = Y - Map->YCenter;
		float ZC = Z - Map->ZCenter;

		switch ( Map->MappingAxis )
		{
			case LWO_XAxis:	U = (ZC/Map->ZSize) + 0.5;
					V = (-YC/Map->YSize) + 0.5;
					break;
			case LWO_YAxis:	U = (XC/Map->XSize) + 0.5;
					V = (-ZC/Map->ZSize) + 0.5;
					break;
			case LWO_ZAxis:	U = (XC/Map->XSize) + 0.5;
					V = (-YC/Map->YSize) + 0.5;
					break;
		}
		U = fabs(U);
		V = fabs(V);
		U = (int)(U*100 + 0.5);	//Rounding to two decimals
		V = (int)(V*100 + 0.5); //Rounding to two decimals
		U = U/100;
		V = V/100;

	}//if "Flat"*/
}//LWO_MapCoords


