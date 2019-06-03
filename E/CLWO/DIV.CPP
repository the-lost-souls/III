#include <string.h>

void Flip_32(void *P)
{
	__asm{
		push	eax
		push	ebx
		pushfd

		mov	esi, P

		xor	eax, eax
		xor	ebx, ebx

		mov	ax, [esi]
		mov	bx, [esi + 2]

		xchg	ah, bl
		xchg	al, bh
		shl	ebx, 16
		or	eax, ebx

		mov	[esi], eax

		popfd
		pop	ebx
		pop	eax

       	}//asm
}//Flip_32


int Flip_32(int N)
{
	int Temp;
	__asm{
		push	eax
		push	ebx
		pushfd

		xor	eax, eax
		xor	ebx, ebx

		mov	ebx, N

		mov	ax, bx
		shr	ebx, 16

		xchg	ah, bl
		xchg	al, bh
		shl	ebx, 16
		or	eax, ebx

		mov	Temp, eax

		popfd
		pop	ebx
		pop	eax

       	}//asm

	return Temp;
}//Flip_32



void Flip_16(void *P)
{
	__asm{

		push	eax
		push	esi

		mov	esi, P

		mov	ax, [esi]
		xchg    al, ah

		mov	[esi], ax

		pop	esi
		pop	eax
	}
}//Flip_16


int Flip_16(short int N)
{
	short int Temp;
	__asm{

		push	eax
		push	esi

		mov	ax, N
		xchg    al, ah

		mov	Temp, ax

		pop	esi
		pop	eax
	}
	return Temp;
}//Flip_16


int Str2Int(const char *String, int &Number, char EndChar)
{
	char *P = (char*)String;
	int Result;

	int Length = 0;
	while ( String[Length] != EndChar ) Length++;
	if ( Length == 0 )
	{
		Number = 0;
		return 0;
	}

	__asm{
		pushad
		pushfd

		mov	esi, P
		mov	ecx, Length
		mov	edx, 1
		xor	eax, eax

	CharLoop:
		dec	ecx

		movzx   ebx, BYTE PTR [esi + ecx]
		sub	ebx, 48			//ascii 0

		cmp	ebx, 0			//check if number is OK
		jl	Error
		cmp	ebx, 9
		jg	Error

		imul	ebx, edx		//mul by 1, 10, 100 etc...

		add	eax, ebx

		imul	edx, 10
		or	ecx, ecx
		jnz	CharLoop

		mov	ebx, Number
		mov	[ebx], eax
		mov	edi, 1
		jmp	OK

	Error:	xor	edi, edi

	OK:	mov	Result, edi
		popfd
		popad
	}//asm

	return Result;  //OK = 0, error = 1
}//Str2Int


int SearchStr(const char *String, const char Letter)
{
	int Length = strlen(String);
	for ( int C = 0; C < Length; C++ )
		if ( String[C] == Letter ) return 1;
	return 0;
}//SearchStr



int Str2Float(const char *String, float &Number, char EndChar)
{
	int FracP = 0;

	int Length = 0;
	while ( String[Length] != EndChar ) Length++;
	if ( Length == 0 )
	{
		Number = 0;
		return 0;
	}//if

//---- Testing sign...
	int IntegerP = 0;
	int Sign = 1;
	if ( String[0] == '-' )
	{
		IntegerP = 1;		//Dump -
		Sign = -1;
	}//if


//---- Searching for point...
	int Point = 0;
	while ( (String[Point] != '.') && (Point < Length) ) Point++;


//---- Converting numbers(integer/frac)...
	Number = 0;
	int Frac = 0;
	int Integer = 0;
	if ( Point < Length ) 		//Number contain a fractional part
	{
		if ( !Str2Int(&String[Point + 1], Frac, EndChar) ) return 0;

		Number = Frac;
		int C = 0;
		while ( String[Point + 1 + C] != EndChar )
		{
			Number = Number/ 10;
			C++;
		}

		if ( !Str2Int(&String[IntegerP], Integer, '.') ) return 0;


	}//if
	else if ( !Str2Int(&String[IntegerP], Integer, EndChar) ) return 0;

	Number += Integer;
	Number *= Sign;

	return 1;
}//Str2Float





/*void *ReadFile(const char *FileName)
{
//---- Open file...
	FILE *InFile;
	InFile = fopen(FileName, "rb");
	if (InFile == NULL)
	{
		SYS_Error(1, "Error opening file\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if

//---- Getting filesize and allocating mem...
	fpos_t _FSize;
	fseek(InFile, 0, SEEK_END);
	fgetpos(InFile, &_FSize);
	int FSize = (int)_FSize;

	void *P = new unsigned char[FSize];

	if ( P == NULL )
	{
		SYS_Error(1, "Error allocating memory to read file\n");
		SYS_Error(3, "  (", FileName, ")\n");
		fclose(InFile);
		return NULL;
	}//if


//---- Read entire file into memory...
	fseek(InFile, 0, SEEK_SET);
	if (fread(P, FSize, 1, InFile) < 1)
	{
		printf("ERR: Error reading file ");
		delete [] P;
		fclose(InFile);
		return 0;
	}

	fclose(InFile);
	return P;
}*/



/*#include <stdio.h>
#include <string.h>

void GotoXY(unsigned char X, unsigned char Y)
{
	__asm{
		pushad
		pushfd

		mov	ah, 02h
		xor	bx, bx
		mov     dh, Y
		mov	dl, X
		int	10h

		popfd
		popad

	}//asm


}//GotoXY


void Box(	int X1, int Y1, int X2, int Y2,
		unsigned char TextColor, unsigned char BackColor)
{
	__asm{
		pushad
		pushfd

		mov	edi, 0x0B8000

		mov	ebx, Y1			//mul by 80
		mov	edx, Y1			//
		shl	ebx, 6			//
		shl	edx, 4			//
		lea	ebx, [ebx + edx]	//

		add	ebx, X1
		shl	ebx, 1

		mov	ah, BackColor
		shl	ah, 4
		or	ah, TextColor
		mov	al, 'Ú'
		mov	[ebx + 0xB8000], ax	//upper left corner

		lea	edi, [ebx + 0xB8000 + 2]

		mov	al, 'Ä'
		mov	ecx, X2
		sub	ecx, X1
		dec	ecx
		rep	stosw			//upper horisontal border

		mov	al, '¿'
		mov	[edi], ax		//upper right corner


		add	ebx, 160

		mov	edx, Y2
		sub	edx, Y1
		dec	edx

		mov	ecx, X2
		shl	ecx, 1

		mov	al, '³'

	YLoop:
		mov	[ebx + 0xB8000], ax	//left vertical border
		mov	[ebx + ecx + 0xB8000], ax //right vertical border

		add	ebx, 160
		dec	edx
		jnz	YLoop

		mov	ebx, Y2			//mul by 80
		mov	edx, Y2			//
		shl	ebx, 6			//
		shl	edx, 4			//
		lea	ebx, [ebx + edx]	//

		add	ebx, X1
		shl	ebx, 1
		mov	al, "À"
		mov	[ebx + 0xB8000], ax

		lea	edi, [ebx + 0xB8000 + 2]
		mov	al, "Ä"
		mov	ecx, X2
		sub	ecx, X1
		dec	ecx
		rep	stosw			//lower horisontal border

		mov	al, "Ù"
		mov	[edi], ax		//lower right corner

		popfd
		popad



	}//asm
}//box

void KeyPressed(void)
{
	__asm
	{
		pushad
		pushfd

		mov	ah, 01
		int	21h

		popfd
		popad
	}//asm
}

void ClrScr(unsigned char TextColor, unsigned char BackColor)
{
	__asm{
		pushad
		pushfd

		mov	edi, 0xB8000
		mov	ecx, 2000
		mov	ah, BackColor
		shl	ah, 4
		or	ah, TextColor
		mov	al, ' '
		rep	stosw

		popfd
		popad
	}//asm

	GotoXY(0,0);
}





int CInt(void *P)
{
	int Temp = *(int*)P;

	__asm{
		push	eax
		push	ebx
		pushfd

		xor	eax, eax
		xor	ebx, ebx

		mov	ax, word ptr Temp
		mov	bx, word ptr Temp + 2

		xchg	ah, bl
		xchg	al, bh
		shl	ebx, 16
		or	eax, ebx

		mov	Temp, eax


		popfd
		pop	ebx
		pop	eax

       	}//asm

	return Temp;
}//CInt


int CSInt(void *P)
{
	short int Temp = *(short int*)P;
	__asm{
		push	eax

		mov	ax, Temp
		xchg    al, ah

		mov	Temp, ax

		pop	eax
	}

	return Temp;
}//CSInt






int Str2Float(const char *String, float &Number, char EndChar)
{
	int FracP = 0;

	int Length = 0;
	while ( String[Length] != EndChar ) Length++;
	if ( Length == 0 )
	{
		Number = 0;
		return 0;
	}//if

//---- Testing sign...
	int IntegerP = 0;
	int Sign = 1;
	if ( String[0] == '-' )
	{
		IntegerP = 1;		//Dump -
		Sign = -1;
	}//if


//---- Searching for point...
	int Point = 0;
	while ( (String[Point] != '.') && (Point < Length) ) Point++;


//---- Converting numbers(integer/frac)...
	Number = 0;
	int Frac = 0;
	int Integer = 0;
	if ( Point < Length ) 		//Number contain a fractional part
	{
		if ( Str2Int(&String[Point + 1], Frac, EndChar) != 0 ) return 1;

		Number = Frac;
		int C = 0;
		while ( String[Point + 1 + C] != EndChar )
		{
			Number = Number/ 10;
			C++;
		}

		if ( Str2Int(&String[IntegerP], Integer, '.') != 0 ) return 1;


	}//if
	else if ( Str2Int(&String[IntegerP], Integer, EndChar) != 0 ) return 1;

	Number += Integer;
	Number *= Sign;

	return 0;
}//Str2Float



int strcmp_c(const char *S1, const char *S2, int Count)
{
	int C1 = 0;
	while ( (S1[C1] == S2[C1]) && C1 < Count ) C1++;
	if ( C1 < Count ) return 1;
	return 0;

}//strcmp_c*/


void Int2Str(int Number, char *String, int Length)
{
	__asm{
		pushad
		pushfd

		mov	eax, Number
		mov	esi, String
		mov	ecx, Length

	NumLoop:
		xor	edx, edx
		mov	ebx, 10
		idiv	ebx
		add	edx, 48			//ascii '0'
		push	edx			//save character

		dec	ecx
		jnz	NumLoop

		mov	ecx, Length
		mov	edi, String
	CharLoop:
		pop	eax
		mov	[edi], al
		inc	edi
		dec	ecx
		jnz	CharLoop

		popfd
		popad
	}//asm
}//Int2Str


