#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\general\canvas.h"
#include "w:\general\filters.h"
#include <mem.h>
#include <stdlib.h>


MBL_Data::MBL_Data(void)
{
	MBL_Type = 26;
	memset(MBL_Plugins, -1, 16*4);
	DBuf = NULL;

}//constructor

MBL_Data::~MBL_Data(void)
{
	delete [] DBuf;
}//destructor

int MBL_Data::Init(void)
{
//---- Allocating memory...
	DBuf = new byte[SYS_XRes*SYS_YRes*4];
	if ( !DBuf )
	{
		SYS_Error(1, "MBL_Init: Not enough memory for MBL-buffer.\n");
		return NULL;
	}//if

	memset(DBuf, 0, SYS_XRes*SYS_YRes*4);

	return 1;
}//MBL_Init




void MBL_Process(void)
{
	MBL_Data *MBL = (MBL_Data*)SYS_OOffs;
	void *Tmp = MBL->DBuf;

	__asm{
		pushad
		pushfd

		mov	esi, Tmp
		mov	edi, SYS_DBufOffs

		mov	ecx, SYS_DBufSize
		shr	ecx, 2

	MBL_Loop:
		mov	eax, [esi + ecx*4]
		mov	ebx, [edi + ecx*4]
		and	eax, 0xFCFCFC
		and	ebx, 0xFCFCFC
		shr	eax, 2
		shr	ebx, 2
		lea	edx, [eax*2 + eax]
		add	edx, ebx
		mov	[esi + ecx*4], edx
		mov	[edi + ecx*4], edx


/*		mov     al, [esi + 2]		//R
		mov	bl, [edi + 2]		//R
		shr	al, 2
		shr	bl, 2
		lea	edx, [eax*2 + eax]
		add	edx, ebx
		mov	[edi + 2], dl
		mov	[esi + 2], dl

		mov     al, [esi + 1]		//G
		mov	bl, [edi + 1]		//G
		shr	al, 2
		shr	bl, 2
		lea	edx, [eax*2 + eax]
		add	edx, ebx
		mov	[edi + 1], dl
		mov	[esi + 1], dl

		mov     al, [esi + 0]		//B
		mov	bl, [edi + 0]		//B
		shr	al, 2
		shr	bl, 2
		lea	edx, [eax*2 + eax]
		add	edx, ebx
		mov	[edi + 0], dl
		mov	[esi + 0], dl*/

/*		mov     al, [esi + 2]		//R
		mov	bl, [edi + 2]		//R
		shr	al, 1
		shr	bl, 1
		add	bl, al
		mov	[edi + 2], bl
		mov	[esi + 2], bl

		mov     al, [esi + 1]		//G
		mov	bl, [edi + 1]		//G
		shr	al, 1
		shr	bl, 1
		add	bl, al
		mov	[edi + 1], bl
		mov	[esi + 1], bl

		mov     al, [esi + 0]		//B
		mov	bl, [edi + 0]		//B
		shr	al, 1
		shr	bl, 1
		add	bl, al
		mov	[edi + 0], bl
		mov	[esi + 0], bl*/

//		add	esi, 4
//		add	edi, 4
		dec	ecx
		jnz	MBL_Loop

		popfd
		popad

	}//asm


}//MBL_Process




BLN2_Data::BLN2_Data(void)
{
	BLN2_Type = 27;
	memset(BLN2_Plugins, -1, 16*4);
	Picture = NULL;

}//constructor

BLN2_Data::~BLN2_Data(void)
{
	delete [] Picture;
}//destructor

void BLN2_Process(void)
{
	BLN2_Data *Data = (BLN2_Data*)SYS_OOffs;
	void *P = Data->Picture;

	srand(SYS_TimerTick);

	int Offs = (((float)rand())/RAND_MAX)*SYS_XRes*SYS_YRes;

	__asm{
		mov	edi, Offs
		shl	edi, 2

		add	edi, P
		mov	d[edi], 0x00FFFFFF
	}//asm

	BLN2();

}//BLN2_Process

int BLN2_Data::Init(void)
{
//---- Allocating memory...
	Picture = new byte[SYS_XRes*SYS_YRes*4];
	if ( !Picture )
	{
		SYS_Error(1, "BLN2_Init: Not enough memory for picture.\n");
		return NULL;
	}//if

	memset(Picture, 0, SYS_XRes*SYS_YRes*4);

	return 1;
}//MBL_Init




BLN3_Data::BLN3_Data(void)
{
	BLN3_Type = 29;
	memset(BLN3_Plugins, -1, 16*4);
}//constructor


void BLN3_Process(void)
{
	BLN3_Data *P = (BLN3_Data*)SYS_OOffs;

	if ( SYS_TimerTick > P->BLN3_StartTime )
	{

		float dif = P->BLN3_Opacity1 - P->BLN3_Opacity0;
		float k = (float)(SYS_TimerTick - P->BLN3_StartTime)/P->BLN3_FadingTime;

		if ( SYS_TimerTick > (P->BLN3_StartTime + P->BLN3_FadingTime) ) k = 1.0;

		k = k*dif + P->BLN3_Opacity0;


		int R = P->BLN3_BlendR*k;
		int G = P->BLN3_BlendG*k;
		int B = P->BLN3_BlendB*k;

		__asm{
			pushad
			pushfd

			mov	eax, R		//al = R
			mov	ebx, G		//ah = G
			mov	ah, bl
			mov	ebx, B		//bl = B

			mov	esi, SYS_DBufOffs

			mov	ebp, SYS_XRes
			imul	ebp, SYS_YRes

		BLN3_Loop:

			mov	cl, [esi + 2]	//R
			mov	dl, [esi + 1]	//G

			add	cl, al
			sbb	bh, bh
			or	cl, bh
			mov	[esi + 2], cl

			add	dl, ah
			sbb	bh, bh
			or	dl, bh
			mov	[esi + 1], dl

			mov	dl, [esi]
			add	dl, bl
			sbb	cl, cl
			or	dl, cl
			mov	[esi], dl

			add	esi, 4
			dec	ebp
			jnz	BLN3_Loop

			popfd
			popad

		}//asm


	}//if


}


