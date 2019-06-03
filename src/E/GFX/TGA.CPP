/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TGA.CPP
³Author       : Cyberfish/TLS
³Last update  : 05.01.1999
³Action       : Loads/decodes a TGA-picture (type 2/10) into memory...
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#include <stdio.h>
#include <mem.h>
#include "w:\general\var.h"
#include "w:\pak\pak.h"
#include "w:\gfx\tga.h"

int 	TGA_Width;
int 	TGA_Heigth;
int 	TGA_XPos;
int	TGA_YPos;
int 	TGA_Bits;


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : TGA_Load
³ID           : TGA
³Last update  : 05.01.1999
³Optimized    : -
³
³In:	Filename : File to load
³
³Out:  Returns offset to picture. 0 if error.
³
³Notes: Reads 15/16/24 bits pictures.
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void *TGA_Load(const char *FileName)
{
	TGA_Header *FHeader;

//---- Reading file...
	void *TempMem = PAK_ReadFile(FileName);
	if ( TempMem == NULL )
	{
		SYS_Error(1, "TGA_Load: Error reading .tga-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return NULL;
	}//if



//---- Analyzing header and allocating more memory
	FHeader = (TGA_Header*)TempMem;

	TGA_Width  = (FHeader->WidthHi << 8) + FHeader->WidthLo;
	TGA_Heigth = (FHeader->HeigthHi << 8) + FHeader->HeigthLo;
	TGA_Bits   = (FHeader->Bits);

	int BytesPrPixel = (TGA_Bits+1) >> 3;


	int MemReq = TGA_Width*TGA_Heigth*BytesPrPixel;
	if ( (TGA_Bits != 15) && (TGA_Bits != 16) && (TGA_Bits != 24) )
	{
		SYS_Error(1, "TGA_Load: Picture type not supported.\n");
		return NULL;
	}//if

	unsigned char *Picture = new unsigned char[MemReq];
	if (Picture == NULL)
	{
		SYS_Error(1, "TGA_Load: Not enough memory for picture.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		delete [] TempMem;
		return NULL;
	}//if

//---- Setting TGAData to picturedata...
	int CMSize = (FHeader->CMapLengthHi << 8) + (FHeader->CMapLengthLo);
	CMSize = CMSize * ((FHeader->CMapSize+1) >> 3) * FHeader->ColorMapType;

	unsigned char *TGAData = (char*)TempMem
				+sizeof(TGA_Header)
				+FHeader->IDFieldLength
				+CMSize;



	switch ( (int)FHeader->ImageType )
	{
		case 2: {	memcpy(Picture, TGAData, MemReq);
				break;
			}//case 2
		case 10:{
				__asm{
					pushad
					pushfd

					mov	esi, TGAData
					mov	edi, Picture

					mov	edx, TGA_Width
					imul	edx, TGA_Heigth
//					imul	edx, BytesPrPixel

					cmp	TGA_Bits, 24
					je      Decode24

					xor	ebx, ebx

				RLELoop16:
					cmp	ebx, edx
					jge	DoneRLE

					movzx   ecx, b[esi]
					and	ecx,  0x7F      	// =01111111b
					inc	ecx
					add	ebx, ecx		//increase pixelcounter

					test	BYTE PTR [esi], 0x80
					jz	RawPacket16

					mov     ax, [esi + 1]		//Load colorvalue
					rep	stosw			//Store pixels

					add	esi, 3			//Dump packet
					jmp	RLELoop16

				RawPacket16:
					inc	esi
					rep	movsw			//copy rawpacket
					jmp	RLELoop16


				Decode24:
					xor	ebx, ebx

				RLELoop24:
					cmp	ebx, edx
					jge	DoneRLE

					movzx   ecx, b[esi]
					and	ecx,  0x7F      	// =01111111b
					inc	ecx
					add	ebx, ecx		//increase pixelcounter

					test	BYTE PTR [esi], 0x80
					jz	RawPacket24

				Store24:
					mov	al, [esi + 3]
					mov	[edi + 2], al
					mov	al, [esi + 2]
					mov	[edi + 1], al
					mov	al, [esi + 1]
					mov	[edi + 0], al

					add	edi, 3
					dec	ecx
					jnz	Store24

					add	esi, 4			//Dump packet
					jmp	RLELoop24

				RawPacket24:
					inc	esi
					lea	ecx, [ecx + ecx*2]
					rep	movsb
					jmp	RLELoop24

				DoneRLE:

					popfd
					popad
				}//asm
				break;
			}//case 10


		default:{
				SYS_Error(1, "TGA_Load: TGA-type not supported.\n");
				SYS_Error(3, "  ( ", FileName, " )\n");
				delete [] TempMem;
				delete [] Picture;
			 	return NULL;
			}
	}//switch


//---- Evt. flipping picture upside down...
	if ( (FHeader->IDescriptor & 0x20) == 0 ) 	//mask out bit 5
	{
		unsigned char *TempLine = new unsigned char[TGA_Width*BytesPrPixel];
		if (TempLine == NULL)
		{
			SYS_Error(1, "TGA_Load: Not enough memory to flip picture.\n");
			SYS_Error(3, "  ( ", FileName, " )\n");
			delete [] TempMem;
			delete [] Picture;
			return NULL;
		}//if

		for ( int C = 0; C < (TGA_Heigth >> 1); C++ )
		{
			memcpy(	TempLine,			//copy upper line
				(void*)&Picture[C*TGA_Width*BytesPrPixel],
				TGA_Width*BytesPrPixel);

			memcpy( (void*)&Picture[C*TGA_Width*BytesPrPixel],
				(void*)&Picture[((TGA_Heigth-1)-C)*TGA_Width*BytesPrPixel],
				TGA_Width*BytesPrPixel);

			memcpy(	(void*)&Picture[((TGA_Heigth-1)-C)*TGA_Width*BytesPrPixel],
				TempLine,
				TGA_Width*BytesPrPixel);
		}

		delete [] TempLine;
	}

	delete [] TempMem;
	return Picture;
}//TGA_Load


