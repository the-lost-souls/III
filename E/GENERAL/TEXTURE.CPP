/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TEXTURE.CPP
³Author       : Cyberfish/TLS
³Last update  : 05.01.1999
³Action       : Routines for loading/unloading a texture.
³
³Notes : The textures are converted to 32 bits.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "w:\gfx\gfx.h"
#include "w:\general\var.h"
#include "w:\general\txt.h"

void *TXT_Scale(byte *Picture, float Scale, uint32 &Width, uint32 &Heigth);

TXT_Data *TXT = NULL;		//Start of the tree

float TXT_ScalingFactor = 1.0;
uint32 TXT_ScalingFlag = 0;

//uint32 MemUse = 0;


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TXT_Data::TXT_Data
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Constructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
TXT_Data::TXT_Data(void)
{
	Texture		= NULL;

	Width		= 0;
	Heigth		= 0;
	Bits		= 0;

	Users		= 1;
	UserID		= 0;		//0 = all

	memset(FileName, 0, SYS_FileNameLength);

	NextTXT = NULL;
}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TXT_Data::~TXT_Data
³ID           : VEC
³Last update  : 08.08.1998
³Action	      : Destructor
³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
TXT_Data::~TXT_Data(void)
{
	delete [] Texture;
	delete NextTXT;
}//if


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TXT_Load
³ID           : TXT
³Last update  : 05.01.1999
³Action       : Loads a picture, inserts it into the tree and returns pointer.
³
³In:	Filename
³
³Out:  Returns pointer picture, width and heigth in pixels
³
³Note: Converts all pictures to 32 bit.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
TXT_Data *TXT_Load(	const char *FileName,
			uint32 UserID)
{
	char UFileName[SYS_FileNameLength];
	for ( int C = 0; C < SYS_FileNameLength; C++ )
		UFileName[C] = toupper(FileName[C]);



//------------- REMOVE THIS:
	if ( TXT_ScalingFlag )
	{
		TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP01.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP02.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP03.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP04.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP05.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\TMPMAPS\\WMAP06.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\DEMO\\ROOM\\MAPS\\FLOOR.JPG") == 0 )
			TXT_ScalingFactor = 0.4;
		if ( strcmp(UFileName, "X:\\MAPS\\STEIN1.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\BLOBS\\SHADE.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\DEMO\\TEST\\MAPS\\PICTURE.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\MAPS\\SURHUND3.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\MAPS\\KROELL2.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\DEMO\\TEST\\MAPS\\GRID.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\DEMO\\TEST\\MAPS\\GRID2.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\DEMO\\WATRFALL\\MAPS\\CRUMPLE5.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\DEMO\\WATRFALL\\MAPS\\CONCR12.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\DEMO\\ROOM\\MAPS\\FSPEIL.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\FLARES\\FLARE20.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\FLARES\\FLARE19.JPG") == 0 )
			TXT_ScalingFactor = 0.5;
		if ( strcmp(UFileName, "X:\\FLARES\\FLARE22.JPG") == 0 )
			TXT_ScalingFactor = 0.5;

		if ( strcmp(UFileName, "X:\\MAPS\\G1.JPG") == 0 )
			TXT_ScalingFactor = 0.25;
		if ( strcmp(UFileName, "X:\\MAPS\\G2.JPG") == 0 )
			TXT_ScalingFactor = 0.25;
		if ( strcmp(UFileName, "X:\\MAPS\\G3.JPG") == 0 )
			TXT_ScalingFactor = 0.25;

	}//if

//------------- TO HERE!!!!!!!*/

	TXT_Data *TempTXT = TXT;
	TXT_Data *NewTXT = NULL;

	if ( TXT == NULL )
	{
		TXT = new TXT_Data;
		if ( TXT == NULL )
		{
			SYS_Error(1, "TXT_LoadTexture: Not enough memory for TXT_Data-structure.\n");
			return NULL;
		}//if
		NewTXT = TXT;
	}//if
	else
	{
	//---- Check if texture already is loaded...
		while ( TempTXT != NULL )
		{
			if ( (strcmp(TempTXT->FileName, UFileName) == 0) && (TempTXT->UserID == UserID) )
			{
				TempTXT->Users++;

				return TempTXT;
			}//if

			if ( TempTXT->NextTXT == NULL ) break;
			TempTXT = TempTXT->NextTXT;
		}//while

	//---- Allocating memory. TempTXT is now last texture in linked list.
		TempTXT->NextTXT = new TXT_Data;
		if ( TempTXT->NextTXT == NULL )
		{
			SYS_Error(1, "TXT_LoadTexture: Not enough memory for TXT_Data-structure.\n");
			return NULL;
		}//if
		NewTXT = TempTXT->NextTXT;
	}//else


//---- Loading texture...
	NewTXT->UserID = UserID;
	switch ( *(uint32*)&UFileName[strlen(UFileName) - 4] )
	{

	//---- Loading tga-picture...
		case 'AGT.':	NewTXT->Texture = TGA_Load(UFileName);
				NewTXT->Width 	= TGA_Width;
				NewTXT->Heigth  = TGA_Heigth;
				NewTXT->Bits	= TGA_Bits;
				break;

	//---- Loading tga-picture...
		case 'GPJ.':	NewTXT->Texture = JPG_Load(UFileName);
				NewTXT->Width 	= JPG_Width;
				NewTXT->Heigth  = JPG_Heigth;
				NewTXT->Bits	= JPG_Bits;
				break;


		default:	SYS_Error(1, "TXT_LoadTexture: Picture type not supported.\n");
				SYS_Error(3, "  ( ", UFileName, " )\n");
				return NULL;
	}//switch


	if ( NewTXT->Texture == NULL )
	{
		SYS_Error(1, "TXT_LoadTexture: Error loading picture.\n");
		SYS_Error(3, "  ( ", UFileName, " )\n");
		return NULL;
	}//if





//---- Converting from 15-bits to 32-bits texture...
	char *P = (char*)NewTXT->Texture;
	if ( (NewTXT->Bits == 15) || (NewTXT->Bits == 16) )
	{
		NewTXT->Texture = NULL;
		NewTXT->Texture = new byte[NewTXT->Width*NewTXT->Heigth*4];
		if ( NewTXT->Texture == NULL )
		{
			SYS_Error(1, "TXT_Load: Not enough memory to convert picture (15/16->32).\n");
			SYS_Error(3, "  ( ", UFileName, " )\n");
			return 0;
		}//if
		byte *P2 = (char*)NewTXT->Texture;
		int Width = NewTXT->Width;
		int Heigth = NewTXT->Heigth;
		__asm{
			pushad
			pushfd

			mov	esi, P
			mov	edi, P2

			mov	eax, Width
			mov	ebx, Heigth
			mov	ebp, eax
			imul	ebp, ebx

		Convert15_32:
			mov	ax, w[esi]
			mov	ebx, 0x7C00		//R
			mov	ecx, 0x3E0		//G
			mov	edx, 0x1F		//B
			and	ebx, eax
			and	ecx, eax
			and	edx, eax

			shr	ebx, 7
			shr	ecx, 2
			shl	edx, 3

			mov	[edi], dl		//B
			mov	[edi + 1], cl           //G
			mov	[edi + 2], bl           //R
			mov	b[edi + 3], 0           //A

			add	esi, 2
			add	edi, 4
			dec	ebp
			jnz	Convert15_32

			popfd
			popad
		}//asm
		delete [] P;

	}//if

//---- Converting from 24-bits to 32-bits texture...
	if ( (NewTXT->Bits == 24) )
	{
		NewTXT->Texture = NULL;
		NewTXT->Texture = new byte[NewTXT->Width*NewTXT->Heigth*4];
		if ( NewTXT->Texture == NULL )
		{
			SYS_Error(1, "TXT_Load: Not enough memory to convert picture (24->32).\n");
			SYS_Error(3, "  ( ", UFileName, " )\n");
			return 0;
		}//if
		byte *P2 = (char*)NewTXT->Texture;
		int Width = NewTXT->Width;
		int Heigth = NewTXT->Heigth;
		__asm{
			pushad
			pushfd

			mov	esi, P
			mov	edi, P2

			mov	eax, Width
			mov	ebx, Heigth
			mov	ebp, eax
			imul	ebp, ebx

		Convert24_32:
			xor	eax, eax
			mov	al, [esi + 2]
			shl	eax, 16
			mov	ah, [esi + 1]
			mov	al, [esi + 0]
			mov	[edi], eax

			add	esi, 3
			add	edi, 4
			dec	ebp
			jnz	Convert24_32

			popfd
			popad
		}//asm
		delete [] P;

	}//if


	strcpy(NewTXT->FileName, UFileName);



//---- Scaling texture...
	if ( TXT_ScalingFactor != 1.0 )
	{
		void *ScaledP = TXT_Scale((byte*)NewTXT->Texture, TXT_ScalingFactor, NewTXT->Width, NewTXT->Heigth);
		if ( !ScaledP )
		{
			SYS_Error(1, "TXT_Load: Error scaling picture.\n");
			SYS_Error(3, "  ( ", UFileName, " )\n");
			return 0;
		}
		delete NewTXT->Texture;
		NewTXT->Texture = ScaledP;
	}//if

//	printf("Picture: %s, %f MB Scaling = %f\n", FileName, (float)(NewTXT->Width*NewTXT->Heigth*4)/(1024*1024), TXT_ScalingFactor);

	return NewTXT;
}//TXT_Load


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TXT_ForceLoad
³ID           : TXT
³Last update  : 07.01.1999
³Action       : Loads a picture, inserts it into the tree and returns pointer
³
³In:	Filename
³
³Out:  Returns pointer picture, width and heigth in pixels
³
³Notes: These textures are set with users=-1, and may therefore not be reused.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void *TXT_ForceLoad(	const char *FileName,
			int &Width,
			int &Heigth)
{
	TXT_Data *NewTXT = TXT;
	TXT_Data *TmpTXT = NULL;

	if ( TXT == NULL )
	{
		TXT = new TXT_Data;
		if ( TXT == NULL )
		{
			SYS_Error(1, "TXT_ForceLoad: Not enough memory for new TXT_Data-structure.\n");
			return NULL;
		}
		NewTXT = TXT;
	}//if
	else
	{
		while ( NewTXT->NextTXT != NULL ) NewTXT = NewTXT->NextTXT;
		NewTXT->NextTXT = new TXT_Data;
		if ( NewTXT->NextTXT == NULL )
		{
			SYS_Error(1, "TXT_ForceLoad: Not enough memory for new TXT_Data-structure.\n");
			return NULL;
		}//if

		NewTXT = NewTXT->NextTXT;
	}//else

//---- Loading texture...
	switch ( *(uint32*)&FileName[strlen(FileName) - 4] )
	{

	//---- Loading tga-picture...
		case 'AGT.':	NewTXT->Texture = TGA_Load(FileName);
				NewTXT->Width 	= TGA_Width;
				NewTXT->Heigth  = TGA_Heigth;
				NewTXT->Bits	= TGA_Bits;
				break;

	//---- Loading tga-picture...
		case 'GPJ.':	NewTXT->Texture = JPG_Load(FileName);
				NewTXT->Width 	= JPG_Width;
				NewTXT->Heigth  = JPG_Heigth;
				NewTXT->Bits	= JPG_Bits;
				break;


		default:	SYS_Error(1, "TXT_ForceLoad: Picture type not supported.\n");
				SYS_Error(3, "  ( ", FileName, " )\n");
				return NULL;
	}//switch


	if ( NewTXT->Texture == NULL )
	{
		SYS_Error(1, "TXT_ForceLoad: Error loading picture.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if

	NewTXT->Users = -1;


//---- Converting from 15-bits to 32-bits texture...
	char *P = (char*)NewTXT->Texture;
	if ( (NewTXT->Bits == 15) || (NewTXT->Bits == 16) )
	{
		NewTXT->Texture = NULL;
		NewTXT->Texture = new byte[NewTXT->Width*NewTXT->Heigth*4];
		if ( NewTXT->Texture == NULL )
		{
			SYS_Error(1, "TXT_Load: Not enough memory to convert picture (15/16->32).\n");
			SYS_Error(3, "  ( ", FileName, " )\n");
			return 0;
		}//if
		byte *P2 = (char*)NewTXT->Texture;
		int Width = NewTXT->Width;
		int Heigth = NewTXT->Heigth;
		__asm{
			pushad
			pushfd

			mov	esi, P
			mov	edi, P2

			mov	eax, Width
			mov	ebx, Heigth
			mov	ebp, eax
			imul	ebp, ebx

		Convert15_32:
			mov	ax, w[esi]
			mov	ebx, 0x7C00		//R
			mov	ecx, 0x3E0		//G
			mov	edx, 0x1F		//B
			and	ebx, eax
			and	ecx, eax
			and	edx, eax

			shr	ebx, 7
			shr	ecx, 2
			shl	edx, 3

			mov	[edi], dl		//B
			mov	[edi + 1], cl           //G
			mov	[edi + 2], bl           //R
			mov	b[edi + 3], 0           //A

			add	esi, 2
			add	edi, 4
			dec	ebp
			jnz	Convert15_32

			popfd
			popad
		}//asm
		delete [] P;

	}//if

//---- Converting from 24-bits to 32-bits texture...
	if ( (NewTXT->Bits == 24) )
	{
		NewTXT->Texture = NULL;
		NewTXT->Texture = new byte[NewTXT->Width*NewTXT->Heigth*4];
		if ( NewTXT->Texture == NULL )
		{
			SYS_Error(1, "TXT_Load: Not enough memory to convert picture (24->32).\n");
			SYS_Error(3, "  ( ", FileName, " )\n");
			return 0;
		}//if
		byte *P2 = (char*)NewTXT->Texture;
		int Width = NewTXT->Width;
		int Heigth = NewTXT->Heigth;
		__asm{
			pushad
			pushfd

			mov	esi, P
			mov	edi, P2

			mov	eax, Width
			mov	ebx, Heigth
			mov	ebp, eax
			imul	ebp, ebx

		Convert24_32:
			xor	eax, eax
			mov	al, [esi + 2]
			shl	eax, 16
			mov	ah, [esi + 1]
			mov	al, [esi + 0]
			mov	[edi], eax

			add	esi, 3
			add	edi, 4
			dec	ebp
			jnz	Convert24_32

			popfd
			popad
		}//asm
		delete [] P;

	}//if


	strcpy(NewTXT->FileName, FileName);

//---- Scaling texture...
	if ( TXT_ScalingFactor != 1.0 )
	{
		void *ScaledP = TXT_Scale((byte*)NewTXT->Texture, TXT_ScalingFactor, NewTXT->Width, NewTXT->Heigth);
		if ( !ScaledP )
		{
			SYS_Error(1, "TXT_Load: Error scaling picture.\n");
			SYS_Error(3, "  ( ", FileName, " )\n");
			return 0;
		}
		delete NewTXT->Texture;
		NewTXT->Texture = ScaledP;
	}//if

	Width 	= NewTXT->Width;
	Heigth 	= NewTXT->Heigth;

	return NewTXT->Texture;
}//TXT_Load



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TXT_Unload
³ID           : TXT
³Last update  : 08.08.1998
³Action       : Unloads a picture, updates the tree
³
³In:	Offset of picture
³
³Out:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TXT_Unload(void *Texture)
{
	if ( TXT == NULL ) return;
	TXT_Data *DelTXT;


//---- Checking first texture...
	if ( TXT->Texture == Texture )
	{
		if ( TXT->Users > 1 )
		{
			TXT->Users--;
			return;
		}//if

		DelTXT 		= TXT;
		TXT 		= TXT->NextTXT;

		DelTXT->NextTXT = NULL;
		delete DelTXT;
		return;
	}//if


//---- Checking rest of the textures...
	TXT_Data *TempTXT = TXT;
	while ( TempTXT->NextTXT != NULL  )
	{
		if ( TempTXT->NextTXT->Texture == Texture )
		{
			if ( TempTXT->NextTXT->Users > 1 )
			{
				TempTXT->NextTXT->Users--;
				return;
			}//if

			DelTXT	= TempTXT->NextTXT;
			TempTXT->NextTXT = TempTXT->NextTXT->NextTXT;

			DelTXT->NextTXT = NULL;
			delete DelTXT;
			return;

		}//if
		TempTXT = TempTXT->NextTXT;
	}//while

	return;
}//TXT_Unload



void *TXT_Scale(byte *Picture, float Scale, uint32 &Width, uint32 &Heigth)
{
	uint32 NewWidth = (uint32)(Scale*Width + 0.5);
	uint32 NewHeigth = (uint32)(Scale*Heigth + 0.5);

	int *P = new int[NewWidth*NewHeigth];
	if ( !P )
	{
		SYS_Error(1, "TXT_Scale: Not enough memory to scale picture.\n");
		return NULL;
	}//if

	float USlope = ((float)Width/NewWidth);
	float VSlope = ((float)Heigth/NewHeigth);
	float U = 0.5;
	float V = 0.5;

	uint32 index = 0;

	for ( int VC = 0; VC < NewHeigth; VC++ )
	{
		uint32 YOffset = ((int)V)*Width;
		U = 0.5;
		for ( int UC = 0; UC < NewWidth; UC++ )
		{
			uint32 offset =  YOffset + (int)U;
			int c = *(int*)&Picture[offset*4];

			P[index] = c;

			index++;
			U+=USlope;
		}//for
		V+=VSlope;
	}//for

	Width = NewWidth;
	Heigth = NewHeigth;
	return P;
}//TXT_Scale
