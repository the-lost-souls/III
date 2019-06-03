/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : CANVAS.CPP
³Author       : Cyberfish/TLS
³Last update  : 05.01.1999
³Action       : Sets up the doublebuffer, zbuffer and ptc
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define Aquarium_ENGINE

#include "w:\divlibs\ptc072\source\ptc.h"
#include "w:\general\var.h"
#include "w:\render\render.h"
#include "w:\general\canvas.h"


PTC 	*ptc;
Surface DBuf;

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : CNV_Data::CNV_Data
³ID           : CNV
³Last update  : 12.12.1998
³Optimized    : -
³
³In:	Flags
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
CNV_Data::CNV_Data(void)
{
	Type = 4;

	for ( int I = 0; I < 16; I++ ) Plugins[I] = -1;

	Flags = CNV_FlagDefault;

}//CNV_Data constructor

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : CNV_Setup
³ID           : CNV
³Last update  : 05.01.1999
³Optimized    : -
³
³In:	XRes, YRes
³
³Notes: Returns 0 if error
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int CNV_Setup(void)
{
	int XRes = SYS_XRes;
	int YRes = SYS_YRes;

//---- Setting up ptc...
	ptc = new PTC(XRes, YRes, ARGB8888);
	if ( !ptc->ok() )
	{
		SYS_XRes = 320;
		SYS_YRes = 200;
		XRes = 320;
		YRes = 200;

		//ptc->Init(XRes, YRes, GREY8);
		if ( !ptc->Init(XRes, YRes, VIRTUAL16) )
		{
			SYS_Error(1, "CNV_Setup: Error initializing alternative videomode.\n");
			return 0;

		}
	}

//---- Setting up doublebuffer...
	SYS_DBufSize = XRes*YRes*4;
	SYS_DBufLine = XRes*4;

	DBuf.Init(*ptc, XRes, YRes, ARGB8888);

	if ( !DBuf.ok() )
	{
		SYS_Error(1, "CNV_Setup: Error setting up ptc-surface.\n");
		return 0;
	}//if

	SYS_DBufOffs = (unsigned char*)DBuf.Lock();
	if ( SYS_DBufOffs == NULL )
	{
		SYS_Error(1, "CNV_Setup: Error getting dbufaddr.\n");
		return 0;
	}


//---- Setting up z-buffer...
	SYS_ZBufSize = XRes*YRes*4;
	SYS_ZBufLine = XRes*4;

	SYS_ZBufOffs = new char[SYS_ZBufSize];
	if ( SYS_ZBufOffs == NULL )
	{
		SYS_Error(1, "CNV_Setup: Not enough memory for z-buffer.\n");
		delete SYS_DBufOffs;
		return 0;
	}//if


//---- Blanking dbuf/zbuf
	__asm{
		pushad
		pushfd

		mov	edi, SYS_DBufOffs
		mov	ecx, SYS_DBufSize
		shr	ecx, 2
		xor	eax, eax
		rep	stosd

		mov	edi, SYS_ZBufOffs
		mov	ecx, SYS_ZBufSize
		shr	ecx, 2
		xor	eax, eax
		rep	stosd

		popfd
		popad


	}//asm



//---- Setting div. variables...
	SYS_FPClipLeft 		= 0;
	SYS_FPClipRight 	= XRes - 1;
	SYS_FPClipBottom	= YRes - 1;
	SYS_FPClipTop		= 0;

	SYS_XPerspective	= ((float)XRes/320)*384;
	SYS_YPerspective	= ((float)YRes/240)*-384;

	SYS_XCenter 		= (float)XRes/2;
	SYS_YCenter		= (float)YRes/2;


//---- Setting up doublebuffer for T09...
	if ( T09_SetupFlag == TRUE )
	{
		T09_DBuf = (void*)new char[SYS_DBufSize];
		if ( T09_DBuf == NULL )
		{
			SYS_Error(1, "CNV_Setup: Not enough memory for T09's doublebuffer.\n");
			delete SYS_DBufOffs;
			delete SYS_ZBufOffs;
			return 0;
		}//if
	}//if

//---- Init rendering-routines...
	T02_Init();
	T03_Init();
	T04_Init();
	T05_Init();
	T06_Init();
	T07_Init();
	T08_Init();
	T09_Init();
	T10_Init();

	return 1;
}//CNV_Setup


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : CNV_Update
³ID           : CNV
³Last update  : 12.12.1998
³Action       : Updates the screen, using ptc (object 4)
³Optimized    : -
³
³In:	None
³
³Notes: Returns 0 if error
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CNV_Update(void)
{
	DBuf.Unlock();
	DBuf.Update();
	SYS_DBufOffs = (unsigned char*)DBuf.Lock();


	__asm{
		pushad
		pushfd

		mov	esi, SYS_DBufOffs

		mov	ecx, SYS_DBufSize
		shr	ecx, 6			//128 bytes each iteration

//---- Blanking canvas...
		mov	edx, 0x0000000
//                xor    edx, edx
	DBuf_Blank:

                mov    	[ esi +  0 ], edx
                mov    	[ esi + 32 ], edx

                mov    	[ esi +  4 ], edx
                mov    	[ esi + 36 ], edx

                mov    	[ esi +  8 ], edx
                mov    	[ esi + 40 ], edx

                mov    	[ esi + 12 ], edx
                mov    	[ esi + 44 ], edx

                mov    	[ esi + 16 ], edx
                mov    	[ esi + 48 ], edx

                mov    	[ esi + 20 ], edx
                mov    	[ esi + 52 ], edx

                mov    	[ esi + 24 ], edx
                mov    	[ esi + 56 ], edx

                mov    	[ esi + 28 ], edx
                mov    	[ esi + 60 ], edx

                add    	esi, 64
                dec    	ecx
                jnz    	DBuf_Blank

		popfd
		popad


	}//asm

	CNV_Data *CNV = (CNV_Data*)SYS_OOffs;
	if ( (CNV->Flags && CNV_FlagOptimizeZBuf) )
	{
		if ( SYS_ZBufAdd == 0xFF000000 )
		{
  			SYS_ZBufAdd = 0;

			__asm{
				pushad
				pushfd

				mov	esi, SYS_ZBufOffs

				mov	ecx, SYS_ZBufSize
				shr	ecx, 6			//64 bytes each iteration

		//---- Blanking zbuffer...
		                xor    edx, edx
		ZBuf_Blank1:

		                mov    	[ esi +  0 ], edx
		                mov    	[ esi + 32 ], edx

		                mov    	[ esi +  4 ], edx
		                mov    	[ esi + 36 ], edx

		                mov    	[ esi +  8 ], edx
		                mov    	[ esi + 40 ], edx

		                mov    	[ esi + 12 ], edx
		                mov    	[ esi + 44 ], edx

		                mov    	[ esi + 16 ], edx
		                mov    	[ esi + 48 ], edx

		                mov    	[ esi + 20 ], edx
		                mov    	[ esi + 52 ], edx

		                mov    	[ esi + 24 ], edx
		                mov    	[ esi + 56 ], edx

		                mov    	[ esi + 28 ], edx
		                mov    	[ esi + 60 ], edx

		                add    	esi, 64
		                dec    	ecx
		                jnz    	ZBuf_Blank1


				popfd
				popad
			}//asm
		}//if

		else SYS_ZBufAdd += 0x01000000;	//increase adder
	}//if
	else
	{
		__asm{
			pushad
			pushfd

			mov	esi, SYS_ZBufOffs

			mov	ecx, SYS_ZBufSize
			shr	ecx, 6			//64 bytes each iteration

	//---- Blanking zbuffer...
			xor    edx, edx
	ZBuf_Blank2:

			mov    	[ esi +  0 ], edx
			mov    	[ esi + 32 ], edx

			mov    	[ esi +  4 ], edx
			mov    	[ esi + 36 ], edx

			mov    	[ esi +  8 ], edx
			mov    	[ esi + 40 ], edx

			mov    	[ esi + 12 ], edx
			mov    	[ esi + 44 ], edx

			mov    	[ esi + 16 ], edx
			mov    	[ esi + 48 ], edx

			mov    	[ esi + 20 ], edx
			mov    	[ esi + 52 ], edx

			mov    	[ esi + 24 ], edx
			mov    	[ esi + 56 ], edx

			mov    	[ esi + 28 ], edx
			mov    	[ esi + 60 ], edx

			add    	esi, 64
			dec    	ecx
			jnz    	ZBuf_Blank2


			popfd
			popad
		}//asm

	}//else (no zbuf-optimization)

}










































