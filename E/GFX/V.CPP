//Testing TGA-load routine


#include <stdio.h>
#include <mem.h>
#include <iostream.h>
#include "w:\gfx\tga.h"
#include "w:\general\var.h"

void main(void)
{
	unsigned char *Picture;
	Picture = TGA_Load("test.tga", 0, 0);
	delete [] Picture;
	Picture = TGA_Load("test.tga", 0, 0);
	delete [] Picture;
	Picture = TGA_Load("test.tga", 0, 0);

	if ( Picture == 0 )
	{
		cout << SYS_QuitMsg;
	}
	 else

//---- Viewing picture (greyscale)
	__asm{
		pushad

       		mov   ah, 0                     //Init screen number
       		mov   al, 13h                   //320x200x256
       		int   10h                       //Do it!

		mov     esi, Picture
		mov	edi, 0A0000h

		mov 	ecx, 64000
	  PixLoop:
		xor	eax, eax
		mov   	ax, [esi]

		mov	ebx, 01Fh
		and	ebx, eax

		mov	edx, 03E0h
		and	edx, eax
		shr	edx, 5

		and	eax, 07C00h
		shr	eax, 10

		add	eax, ebx
		add	eax, edx

		mov	ebx, 3
		xor	edx, edx
		idiv	ebx

		shr	eax, 1

		add	eax, 16
		mov	[edi], al

		add	esi, 2
		inc	edi

		dec	ecx
		jnz	PixLoop

		mov   	ah, 01h
		int     21h

       		mov   	ah, 0                     //Init screen number
       		mov   	al, 3h                   //320x200x256
       		int   	10h                       //Do it!

		popad
	}//asm*/
}//main
