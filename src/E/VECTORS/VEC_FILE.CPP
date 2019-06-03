/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : VEC_FILE.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 16.01.99
쿌ction       : Loads a vectorobject.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\general\txt.h"
#include "w:\vectors\vec_file.h"
#include "w:\vectors\vec_run.h"
#include "w:\pak\pak.h"
#include "w:\render\render.h"
#include <stdio.h>
#include <mem.h>
#include <math.h>



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Load
쿔D           : VEC
쿗ast update  : 08.08.1998
쿌ction       : Loads a file (.vec-format) and returns pointer to VEC_Header
�
쿔n:	Filename
�
쿚ut:  Returns pointer to header
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/
VEC_Header *VEC_Load(const char *FileName)
{
//---- Reading .vec-file from disk
	VEC_FILE_Header *VECFile = (VEC_FILE_Header*)PAK_ReadFile(FileName);
	if ( VECFile == NULL )
	{
		SYS_Error(1, "VEC_Load: Error reading .vec-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if

//---- Loading vector...
	VEC_Header *VEC = VEC_Load(VECFile);
	if ( VEC == NULL )
	{
		SYS_Error(1, "VEC_Load: Error running VEC_Load.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		delete VECFile;
		return NULL;
	}//if

	delete VECFile;
	return VEC;
}//VEC_Load



/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ ROUTINE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : VEC_Load
쿔D           : VEC
쿗ast update  : 16.01.99
쿌ction       : Loads a file (.vec-format) and returns pointer to VEC_Header
�
쿔n:	Offset of loaded .vec-file
�
쿚ut:  Returns pointer to header
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

VEC_Header *VEC_Load(VEC_FILE_Header *VECFile)
{
//---- Testing file's header...
	if ( VECFile->ID != VEC_FILE_ID )
	{
		SYS_Error(1, "VEC_Load: Corrupt VEC_FILE_ID.\n");
		return NULL;
	}//if

	int C1 = 0;
	int C2 = 0;
	int C3 = 0;


//---- Testing checksum...
	C2 = VECFile->Checksum;
	VECFile->Checksum = 0;

	byte *P = (byte*)VECFile;
	C3 = 0;

	for ( C1 = 0; C1 < VECFile->FileSize; C1++ )
		C3 += (uint32)*(uint8*)&P[C1];

	if ( C3 != C2 )
	{
		SYS_Error(1, "VEC_Load: Checksum failure.\n");
		return NULL;
	}//if



//---- Allocating memory...
	VEC_Header *VEC = new VEC_Header;
	if ( VEC == NULL )
	{
		SYS_Error(1, "VEC_Load: Not enough memory for objectheader.\n");
		return NULL;
	}//if

	VEC->H_Vertices = new VEC_Vertice[VECFile->Vertices];
	if ( VEC->H_Vertices == NULL )
	{
		SYS_Error(1, "VEC_Load: Not enough memory for vertices.\n");
		delete VEC;
		return NULL;
	}//if

	VEC->H_Faces = new VEC_Face[VECFile->Faces];
	if ( VEC->H_Faces == NULL )
	{
		SYS_Error(1, "VEC_Load: Not enough memory for faces.\n");
		delete VEC;
		return NULL;
	}//if

	VEC->H_Materials = new VEC_Material[VECFile->Materials];
	if ( VEC->H_Materials == NULL )
	{
		SYS_Error(1, "VEC_Load: Not enough memory for materials.\n");
		delete VEC;
		return NULL;
	}//if




//---- Searching for wanted chunks...
	VEC_FILE_VertChunk *V_Chunk = NULL;
	VEC_FILE_FaceChunk *F_Chunk = NULL;
	VEC_FILE_MaterialChunk *M_Chunk = NULL;


	P = (byte*)VECFile;
	C1 = 256 + 8 + VECFile->Size;
	while ( C1 < VECFile->FileSize )
	{
		switch ( *(uint32*)&P[C1] )
		{
			case VEC_FILE_Vertice_ID : V_Chunk = (VEC_FILE_VertChunk*)&P[C1];
				     		   break;

			case VEC_FILE_Face_ID    : F_Chunk = (VEC_FILE_FaceChunk*)&P[C1];
				     		   break;

			case VEC_FILE_Material_ID: M_Chunk = (VEC_FILE_MaterialChunk*)&P[C1];
				     		   break;

			default    		 : break;
		}//switch

		C1 += *(uint32*)&P[C1 + 4] + 8;	//Next chunk
	}//while


//---- Setting up header...
	VEC->H_Type		= VECFile->Type;
	VEC->H_NrVertices 	= VECFile->Vertices;
	VEC->H_NrFaces		= VECFile->Faces;
	VEC->H_NrMaterials	= VECFile->Materials;


//---- Reading vertices...
	VEC_FILE_Vertice *Vertice = &V_Chunk->Vertices[0];
	for ( C1 = 0; C1 < VEC->H_NrVertices; C1++ )
	{
		VEC->H_Vertices[C1].V_X = Vertice->X;
		VEC->H_Vertices[C1].V_Y = Vertice->Y;
		VEC->H_Vertices[C1].V_Z = Vertice->Z;

		VEC->H_Vertices[C1].V_NVX = 0;
		VEC->H_Vertices[C1].V_NVY = 0;
		VEC->H_Vertices[C1].V_NVZ = 0;

		Vertice++;
	}//for


//---- Reading materials...
	VEC_FILE_Material *Material = &M_Chunk->Materials[0];
	for ( C1 = 0; C1 < VEC->H_NrMaterials; C1++ )
	{
		VEC->H_Materials[C1].M_Type = Material->Type;

		VEC->H_Materials[C1].M_R = Material->R;
		VEC->H_Materials[C1].M_G = Material->G;
		VEC->H_Materials[C1].M_B = Material->B;

		VEC->H_Materials[C1].M_Luminosity 	= Material->Luminosity;
		VEC->H_Materials[C1].M_Diffuse  	= Material->Diffuse;
		VEC->H_Materials[C1].M_Transparency	= Material->Transparency;

	//---- Loading texture...
		switch ( Material->Type )
		{

			case TYPE_Flat_ColorMap:{
				TXT_Data * Tmp = TXT_Load(Material->ColorMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->ColorMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;

				break;
			}//case

			case TYPE_Flat_EnvMap:{
				TXT_Data * Tmp = TXT_Load(Material->EnvMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->EnvMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;

				break;
			}//case

			case TYPE_Flat_TransparentMap:{
				TXT_Data * Tmp = TXT_Load(Material->ColorMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->ColorMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;
				break;

			}//case

			case TYPE_Flat_TransparentEnvMap:{
				TXT_Data * Tmp = TXT_Load(Material->EnvMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->EnvMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;

				break;
			}//case

			case TYPE_Flat_TransparentMap_Additive:{
				TXT_Data * Tmp = TXT_Load(Material->ColorMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->ColorMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;

				break;
			}//case

			case TYPE_Flat_TransparentEnvMap_Additive:{
				TXT_Data * Tmp = TXT_Load(Material->EnvMap, 0);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->EnvMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;
				break;
			}//case


			case TYPE_Flat_Glass:{
				T09_SetupFlag = TRUE;

				TXT_Data * Tmp = TXT_Load(Material->BumpMap, 1);
				if ( !Tmp )
				{
					SYS_Error(1, "VEC_Load: Error loading texture.\n");
					SYS_Error(3, "  ( ", Material->BumpMap, " )\n");
					return NULL;
				}//if
				VEC->H_Materials[C1].M_MapOffset = Tmp->Texture;
				VEC->H_Materials[C1].M_MapWidth = Tmp->Width;
				VEC->H_Materials[C1].M_MapHeigth = Tmp->Heigth;

			//---- Preparing texture...
				if ( Tmp->Users == 1 ) 		//This is the first user!
				{
					int XSize = VEC->H_Materials[C1].M_MapWidth;
					int YSize = VEC->H_Materials[C1].M_MapHeigth;

					void *P	  = VEC->H_Materials[C1].M_MapOffset;
					void *P2  = (void*)new char[XSize*YSize*2];
					if ( P2 == NULL )
					{
						SYS_Error(1, "VEC_Load: Not enough memory to set up glass' bumptexture.\n");
						SYS_Error(3, "  ( ", Material->BumpMap, " )\n");
						return NULL;
					}//if

					int OKFlag = TRUE;


					__asm{
						pushad
						pushfd

					//---- Converting picture to greyscale...
						mov	esi, P
						mov	edi, P
						mov	ecx, XSize
						imul	ecx, YSize

					GreyLoop:
						xor	eax, eax
						xor	ebx, ebx
						xor	edx, edx
						mov	 al, [esi + 2]	//R
						mov	 bl, [esi + 1]	//G
						mov	 dl, [esi + 0]	//B

						shr	eax, 3		//0 <= R <=31
						shr	ebx, 3
						shr	edx, 3

						add	eax, ebx
						add	eax, edx

						xor	edx, edx
						mov	ebx, 3
						idiv	ebx

						mov	w[edi], ax

						add	esi, 4
						add	edi, 2
						dec	ecx
						jnz	GreyLoop


					//---- Converting greyscale to displacement-indexes...
						mov	esi, P
						mov	edi, P2
						xor	edx, edx
					YLoop:
						xor	ecx, ecx
					XLoop:
						push	ecx
						push	edx

						//using ebx as pointer to upper value
						mov	ebx, XSize
						shl	ebx, 1
						neg	ebx
						add	ebx, esi

						cmp	edx, 1
						jae	UpperOK
						add	ebx, XSize
						add	ebx, XSize

					UpperOK:
						xor	eax, eax
						mov	ax, w[ebx]	//ax = upper

						//using ebx as pointer to lower value
						mov	ebx, esi
						add	ebx, XSize
						add	ebx, XSize

						inc	edx
						cmp	edx, YSize
						jne	LowerOK
						sub	ebx, XSize
						sub	ebx, XSize
					LowerOK:
						xor	edx, edx
						mov	dx, w[ebx]
						sub	eax, edx //eax = vertical diff.

						//using ebx as pointer to left value
						lea	ebx, [esi - 2]
						cmp	ecx, 1
						jae	LeftOK
						add	ebx, 2
					LeftOK:
						xor	edx, edx
						mov	dx, w[ebx]

						//using ebx as pointer to right value
						lea	ebx, [esi + 2]
						inc	ecx
						cmp	ecx, XSize
						jne	RightOK
						sub	ebx, 2
					RightOK:
						xor	ecx, ecx
						mov	cx, w[ebx]
						sub	edx, ecx   //edx = horisontal diff.

						//Calculating displacement offset
						add	eax, 32		//1 <= x <= 63
						add	edx, 32

						mov	ah, dl		//ah = hor.
									//al = vert.

						mov	w[edi], ax

						add	edi, 2
						add	esi, 2

						pop	edx
						pop	ecx
						inc	ecx
						cmp	ecx, XSize
						jb	XLoop

						inc	edx
						cmp	edx, YSize
						jb	YLoop

						mov	esi, P2
						mov	edi, P
						mov	ecx, XSize
						imul	ecx, YSize
						rep	movsw

						popfd
						popad
					}//asm

					delete [] P2;

					if ( OKFlag == FALSE )
					{
						SYS_Error(1, "VEC_Load: Error setting up glass' bumpmap. Reduce resolution.\n");
						SYS_Error(3, "  ( ", Material->ColorMap, " )\n");
						return NULL;
					}//if
				}
				break;
			}//case

		}//switch

		Material++;
	}//for


//---- Reading faces, and calculating normalvectors...
	VEC_FILE_Face *Face = &F_Chunk->Faces[0];
	for ( C1 = 0; C1 < VEC->H_NrFaces; C1++ )
	{
		VEC->H_Faces[C1].F_V0Offs = &VEC->H_Vertices[Face->Vert0];
		VEC->H_Faces[C1].F_V1Offs = &VEC->H_Vertices[Face->Vert1];
		VEC->H_Faces[C1].F_V2Offs = &VEC->H_Vertices[Face->Vert2];

		VEC->H_Faces[C1].F_Material = &VEC->H_Materials[Face->Material];

		VEC->H_Faces[C1].F_U0 = Face->U0 * (VEC->H_Faces[C1].F_Material->M_MapWidth - 1);
		VEC->H_Faces[C1].F_V0 = Face->V0 * (VEC->H_Faces[C1].F_Material->M_MapHeigth - 1);
		VEC->H_Faces[C1].F_U1 = Face->U1 * (VEC->H_Faces[C1].F_Material->M_MapWidth - 1);
		VEC->H_Faces[C1].F_V1 = Face->V1 * (VEC->H_Faces[C1].F_Material->M_MapHeigth - 1);
		VEC->H_Faces[C1].F_U2 = Face->U2 * (VEC->H_Faces[C1].F_Material->M_MapWidth - 1);
		VEC->H_Faces[C1].F_V2 = Face->V2 * (VEC->H_Faces[C1].F_Material->M_MapHeigth - 1);


	//---- Calculating face-vectors (A = Vert1 - Vert0, B = Vert2 - Vert0)
		float A0 = VEC->H_Vertices[Face->Vert1].V_X - VEC->H_Vertices[Face->Vert0].V_X;
		float A1 = VEC->H_Vertices[Face->Vert1].V_Y - VEC->H_Vertices[Face->Vert0].V_Y;
		float A2 = VEC->H_Vertices[Face->Vert1].V_Z - VEC->H_Vertices[Face->Vert0].V_Z;

		float B0 = VEC->H_Vertices[Face->Vert2].V_X - VEC->H_Vertices[Face->Vert0].V_X;
		float B1 = VEC->H_Vertices[Face->Vert2].V_Y - VEC->H_Vertices[Face->Vert0].V_Y;
		float B2 = VEC->H_Vertices[Face->Vert2].V_Z - VEC->H_Vertices[Face->Vert0].V_Z;


	//---- Calculating face's normalvector
		float NVX = A1*B2-A2*B1;
		float NVY = A2*B0-A0*B2;
		float NVZ = A0*B1-A1*B0;
		float DotProduct = NVX*NVX + NVY*NVY + NVZ*NVZ;
		float Length = 1;

		if ( DotProduct != 0 ) Length = sqrt(DotProduct);

		VEC->H_Faces[C1].F_NVX = NVX / Length;
		VEC->H_Faces[C1].F_NVY = NVY / Length;
		VEC->H_Faces[C1].F_NVZ = NVZ / Length;

		VEC->H_Faces[C1].F_Priority = 0;


	//---- Adding vectors to vertices' "normalvectors"...
		VEC->H_Faces[C1].F_V0Offs->V_NVX += NVX;
		VEC->H_Faces[C1].F_V0Offs->V_NVY += NVY;
		VEC->H_Faces[C1].F_V0Offs->V_NVZ += NVZ;

		VEC->H_Faces[C1].F_V1Offs->V_NVX += NVX;
		VEC->H_Faces[C1].F_V1Offs->V_NVY += NVY;
		VEC->H_Faces[C1].F_V1Offs->V_NVZ += NVZ;

		VEC->H_Faces[C1].F_V2Offs->V_NVX += NVX;
		VEC->H_Faces[C1].F_V2Offs->V_NVY += NVY;
		VEC->H_Faces[C1].F_V2Offs->V_NVZ += NVZ;

		Face++;

	}//for


//---- Setting part-types and priority for mixed objecttype...
	if ( VEC->H_Type == 20 )
	{
		for ( C1 = 0; C1 < VEC->H_NrFaces; C1++ )
		{
			VEC_Material *M = VEC->H_Faces[C1].F_Material;
			switch ( M->M_Type )
			{
				case TYPE_Flat_Color:
					VEC->H_Faces[C1].F_Type = 2;
					VEC->H_Faces[C1].F_Priority = 250;
					break;

				case TYPE_Flat_ColorMap:
					VEC->H_Faces[C1].F_Type = 3;
					VEC->H_Faces[C1].F_Priority = 250;
					break;

				case TYPE_Flat_Transparent:
					VEC->H_Faces[C1].F_Type = 4;
					VEC->H_Faces[C1].F_Priority = 240;
					break;

				case TYPE_Flat_EnvMap:
					VEC->H_Faces[C1].F_Priority = 250;
					VEC->H_Faces[C1].F_Type = 6;
					break;

				/*case TYPE_Glass:
					VEC->H_Faces[C1].F_Priority = 240;
					VEC->H_Faces[C1].F_Type = 7;*/

			}//switch
		}
	}//if


//---- Calculating "smoothed" normalvectors for vertices...
	for ( C1 = 0; C1 < VEC->H_NrVertices; C1++)
	{
		float NVX = VEC->H_Vertices[C1].V_NVX;
		float NVY = VEC->H_Vertices[C1].V_NVY;
		float NVZ = VEC->H_Vertices[C1].V_NVZ;

		float Length = sqrt(NVX*NVX + NVY*NVY + NVZ*NVZ);

		VEC->H_Vertices[C1].V_NVX = NVX / Length;
		VEC->H_Vertices[C1].V_NVY = NVY / Length;
		VEC->H_Vertices[C1].V_NVZ = NVZ / Length;

		VEC->H_Vertices[C1].V_RFlag = 0;
	}//for

	return VEC;
}//Vxx_Load

