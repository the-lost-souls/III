/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ SOURCE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK.CPP
³Author       : Cyberfish/TLS
³Last update  : 03.06.1998
³Action       : Div. routines for PAK-system.
³
³Notes :
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <mem.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>
#include <fstream.h>
#include "w:\divlibs\zlib\zlib.h"
#include "w:\pak\pak.h"

PAK_FileInfo	*PAK_FirstPAK = NULL;
PAK_FileInfo	*PAK_LastPAK  = NULL;


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Encode
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: Data to encode
³	Param2: Length of data to be encoded
³	Param3: Length of compressed block
³
³Out:  Returns offset to encoded block. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void *PAK_Encode( byte *Data,
		  int RawLength,
		  int &CompressedLength)
{
	CompressedLength = (int)( (float)RawLength*1.2) + 12;
	byte *Output = new byte[CompressedLength];
	if ( Output == NULL )
	{
		SYS_Error(1, "PAK_Encode: Not enough memory for compression\n");
		return NULL;
	}//if

	int err = compress(Output, (uLong*)&CompressedLength, Data, (uLong)RawLength);

	if ( err != 0 )
	{
		SYS_Error(1, "PAK_Encode: zlib returned an error (");
		switch ( err )
		{
			case -2: SYS_Error(1, "Z_STREAM_ERROR)\n");
				 break;
			case -4: SYS_Error(1, "Z_MEM_ERROR)\n");
				 break;
			case -5: SYS_Error(1, "Z_BUF_ERROR)\n");
				 break;
		}//switch
		delete [] Output;
		return NULL;
	}//if

	byte *FinalOutput = new byte[CompressedLength];
	if ( FinalOutput == NULL )
	{
		SYS_Error(1, "PAK_Encode: Not enough memory for compression.\n");
		delete [] Output;
		return NULL;
	}//if

	memcpy(FinalOutput, Output, CompressedLength);
	delete [] Output;

	return FinalOutput;
}//PAK_Encode



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Decode
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: Data to decode
³	Param2: Length of uncompressed block
³	Param3: Length of compressed block
³
³Out:  Returns offset to decoded block. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void *PAK_Decode( byte *Data,
		  int &RawLength,
		  int CompressedLength)
{
	RawLength = 1024*1024*3;
	byte *Output = new byte[RawLength];		//one mb
	if ( Output == NULL )
	{
		SYS_Error(1, "PAK_Decode: Not enough memory for decompression.\n");
		return NULL;
	}//if

	int err = uncompress(Output, (uLong*)&RawLength, Data, (uLong)CompressedLength);

	if ( err != 0 )
	{
		SYS_Error(1, "PAK_Decode: zlib returned an error (");
		switch ( err )
		{
			case -3: SYS_Error(1, "Z_DATA_ERROR)\n");
				 break;
			case -4: SYS_Error(1, "Z_MEM_ERROR)\n");
				 break;
			case -5: SYS_Error(1, "Z_BUF_ERROR)\n");
				 break;
		}//switch
		delete [] Output;
		return NULL;
	}//if

	byte *FinalOutput = new byte[RawLength];
	if ( FinalOutput == NULL )
	{
		SYS_Error(1, "PAK_Decode: Not enough memory for decompression.\n");
		delete [] Output;
		return NULL;
	}//if

	memcpy(FinalOutput, Output, RawLength);
	delete [] Output;

	return FinalOutput;
}//PAK_Decode





 /*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Create
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³In:	Param1: Filename of PAK-file to create
³
³Out:  Returns pointer to PAK-file-info. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_FileInfo *PAK_Create( const char *FileName, const char *IDString)
{
//---- Checking strings...
	if ( strlen(FileName) > SYS_FileNameLength )
	{
		SYS_Error(1, "PAK_Create: Filename too long.\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if

	if ( strlen(IDString) > 255 )
	{
		SYS_Error(1, "PAK_Create: IDString too long.\n");
		SYS_Error(3, "  (", FileName, ")\n");

		return NULL;
	}//if


//---- Allocating PAK_FileInfo structure...
	PAK_FileInfo *NewPAK = new PAK_FileInfo;
	if ( NewPAK == NULL )
	{
		SYS_Error(1, "PAK_Create: Not enough memory.\n");
		SYS_Error(3, "  (", FileName, ")\n");

		return NULL;
	}//if

	strcpy( (char*)NewPAK->FileName, FileName);
	for ( int C = 0; C < strlen(FileName); C++ ) NewPAK->FileName[C] = toupper(NewPAK->FileName[C]);


//---- Setting up fileheader...
	PAK_FILE_Header FileHeader;

	strcpy(FileHeader.IDString, IDString);
	FileHeader.IDString[strlen(IDString)] = 0x1A;		//EOF-marker


//---- Opening new file, writing fileheader, closing file.
	NewPAK->PAKFile = fopen(FileName, "wb");
	if ( NewPAK->PAKFile == NULL )
	{
		SYS_Error(1, "PAK_Create: Couldn't create PAK-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		delete NewPAK;
		return NULL;
	}//if

	if ( fwrite(&FileHeader, sizeof(FileHeader), 1, NewPAK->PAKFile) < 1 )
	{
		SYS_Error(1, "PAK_Create: Couldn't write to PAK-file.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");

		fclose(NewPAK->PAKFile);
		delete NewPAK;
		return NULL;
	}//if

	fclose(NewPAK->PAKFile);


//---- Updating PAK-file-list
	if ( PAK_FirstPAK == NULL )
	{
		PAK_FirstPAK = NewPAK;
		PAK_LastPAK  = NewPAK;
	}//if
	else
	{
		PAK_LastPAK->NextPAK = NewPAK;
		PAK_LastPAK = NewPAK;
	}//else

	return NewPAK;
}//PAK_Create




 /*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Addfile
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³In:	Param1: Filename of PAK-file to create
³
³Out:  Returns pointer to PAK-file-info. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int PAK_AddFile(PAK_FileInfo *PAK, const char *FileName)
{
	PAK_BlockInfo *NextBlock;
	int C;

	if ( strlen(FileName) > SYS_FileNameLength )
	{
		SYS_Error(1, "PAK_AddFile: Filename too long.\n");
		SYS_Error(5, "  (", FileName, " -> ", PAK->FileName, ")\n");

		return NULL;
	}//if

	char FNameUppercase[SYS_FileNameLength];
	for ( C = 0; C <= strlen(FileName); C++ )
		FNameUppercase[C] = toupper(FileName[C]);

//---- Check if file already is in PAK...
	NextBlock = PAK->FirstBlock;
	while ( NextBlock != NULL )
	{
		if ( strcmp(FNameUppercase, NextBlock->FileName) == 0 )
		{
			printf("PAK_AddFile: Warning, file already exists in PAK. Ignoring.\n");
			printf("  ( %s )\n", PAK->FileName);
			return 1;
//			SYS_Error(1, "PAK_AddFile: File already exist in PAK\n");
//			SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

//			return NULL;
		}//if
		NextBlock = NextBlock->NextBlock;
	}//while

//---- Opening and reading new file
	FILE *NewFile = fopen(FNameUppercase, "rb");
	if ( NewFile == NULL )
	{
		SYS_Error(1, "PAK_AddFile: Couldn't find new file\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		return NULL;
	}//if

	fseek(NewFile, 0, SEEK_END);
	int FileSize = ftell(NewFile);
	fseek(NewFile, 0, SEEK_SET);

	byte *FileData = new byte[FileSize];
	if ( FileData == NULL )
	{
		SYS_Error(1, "PAK_AddFile: Not enough memory\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		fclose(NewFile);
		return NULL;
	}//if

	if ( fread(FileData, FileSize, 1, NewFile) < 1 )
	{
		SYS_Error(1, "PAK_AddFile: Error reading new file\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		delete [] FileData;
		fclose(NewFile);
		return NULL;
	}//if

	fclose(NewFile);


//---- Compressing file...
	int CompressedSize;
	byte *CompressedFile = (byte*)PAK_Encode(FileData, FileSize, CompressedSize);
	delete [] FileData;		//Don't need this anymore

	if ( CompressedFile == NULL ) 	//error compressing...
	{
		SYS_Error(1, "PAK_AddFile: PAK_Encode() returned an error\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		return NULL;
	}//if



//---- Reading header from PAK-file...
	PAK_FILE_Header *FileHeader = PAK_Verify(PAK->FileName);
	if ( FileHeader == NULL )
	{
		SYS_Error(1, "PAK_AddFile: PAK_Verify() returned an error");
		SYS_Error(5, "  (", FileName, " -> ", PAK->FileName, ")\n");

		delete [] CompressedFile;
		return NULL;
	}//if


//---- Adding new blockinfo-structure into PAK_FileInfo
	PAK_BlockInfo *NewBlock = new PAK_BlockInfo;
	if ( NewBlock == NULL )
	{
		SYS_Error(1, "PAK_AddFile: Not enough memory\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		delete FileHeader;
		return NULL;
	}//if


//---- Updating new blockinfo
	strcpy(NewBlock->FileName, FNameUppercase);	//Set filename
	NewBlock->Length_Raw = FileSize;		//Original filesize
	NewBlock->Length_Compressed = CompressedSize;	//Compressed filesize
	NewBlock->Checksum = 0;				//Getting checksum
	for ( C = 0; C < CompressedSize; C++ )
		NewBlock->Checksum += (unsigned int)CompressedFile[C];
	NewBlock->Offset = FileHeader->MapOffset;	//Offset in PAK-file

	PAK_FILE_BlockInfo *AllBlocks = new PAK_FILE_BlockInfo[FileHeader->NrFiles + 1];
	if ( AllBlocks == NULL )
	{
		SYS_Error(1, "PAK_AddFile: Not enough memory\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		delete [] CompressedFile;
		delete FileHeader;
		return NULL;

	}//if

	if ( PAK->FirstBlock != NULL )
	{
		PAK->LastBlock->NextBlock = NewBlock;
		PAK->LastBlock = NewBlock;
	}//if
	else
	{
		PAK->FirstBlock = NewBlock;
		PAK->LastBlock = NewBlock;
	}//else

	C = 0;
	NextBlock = PAK->FirstBlock;
	while ( NextBlock != NULL )
	{
	      AllBlocks[C] = *( (PAK_FILE_BlockInfo*)NextBlock );
	      C++;
	      NextBlock = NextBlock->NextBlock;
	}//while


//---- Opening and saving new data to PAK-file...
	PAK->PAKFile = fopen(PAK->FileName, "r+b");
	fseek(PAK->PAKFile, FileHeader->MapOffset, SEEK_SET);
	if ( fwrite(CompressedFile, CompressedSize, 1, PAK->PAKFile) < 1 )
	{
		SYS_Error(1, "PAK_AddFile: Error writing new file. !!! PAK-file corrupted !!!\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		fclose(PAK->PAKFile);
		delete [] CompressedFile;
		delete FileHeader;
		return NULL;
	}//if

	delete [] CompressedFile;

	FileHeader->NrFiles++;
	PAK->NrFiles++;

//---- Saving new filemap...
	int Dump;
	byte *Temp = (byte*)PAK_Encode((byte*)AllBlocks, sizeof(PAK_FILE_BlockInfo)*FileHeader->NrFiles, Dump);
	delete [] AllBlocks;
	if ( Temp == NULL )
	{
		fclose(PAK->PAKFile);
		SYS_Error(1, "PAK_AddFile: Error compressing new data. !!! PAK-file corrupted !!!\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		delete FileHeader;
		return NULL;
	}//if

	if ( fwrite(Temp, Dump, 1, PAK->PAKFile) < 1 )
	{
		SYS_Error(1, "PAK_AddFile: Error writing new filemap. !!! PAK-file corrupted !!!\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		fclose(PAK->PAKFile);
		delete FileHeader;
		delete [] Temp;
		return NULL;
	}//if

	FileHeader->FileSize = FileHeader->MapOffset + CompressedSize + Dump;
	FileHeader->MapOffset += CompressedSize;
	FileHeader->MapChecksum = 0;
	for ( C = 0; C < Dump; C++ )
		FileHeader->MapChecksum += (unsigned int)Temp[C];

	delete [] Temp;

	fseek(PAK->PAKFile, 0, SEEK_SET);
	if ( fwrite(FileHeader, sizeof(*FileHeader), 1, PAK->PAKFile) < 1)
	{
		SYS_Error(1, "PAK_AddFile: Error writing new fileheader. !!! PAK-file corrupted !!!\n");
		SYS_Error(5, "  (", FNameUppercase, " -> ", PAK->FileName, ")\n");

		fclose(PAK->PAKFile);
		delete FileHeader;
		return NULL;
	}//if
	delete FileHeader;

	fclose(PAK->PAKFile);

	return 1;
}//PAK_AddFile





 /*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Open
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³In:	Param1: Filename of PAK-file to open
³
³Out:  Returns pointer to PAK-file-info. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_FileInfo *PAK_Open(const char *FileName)
{
//---- Checking filename...
	if ( strlen(FileName) > SYS_FileNameLength )
	{
		SYS_Error(1, "PAK_Open: Filename too long\n");
		SYS_Error(3, "  (", FileName, ")\n");

		return NULL;
	}//if


//---- Verifying PAK...
	PAK_FILE_Header *FileHeader = PAK_Verify(FileName);
	if ( FileHeader == NULL )
	{
		SYS_Error(1, "PAK_OpenFile: PAK_Verify() returned an error\n");
		SYS_Error(3, "  (", FileName, ")\n");

		return NULL;
	}//if

//---- Allocating memory for new PAK_FileInfo
	PAK_FileInfo *NewPAK = new PAK_FileInfo;
	if ( NewPAK == 0 )
	{
		SYS_Error(1, "PAK_Open: Not enough memory\n");
		SYS_Error(3, "  (", FileName, ")\n");

		delete FileHeader;
		return NULL;
	}//if

	NewPAK->NrFiles = FileHeader->NrFiles;
	for ( int C = 0; C <= strlen(FileName); C++ )
		NewPAK->FileName[C] = toupper(FileName[C]);

	if ( FileHeader->NrFiles != 0 )
	{

	//---- Reading filemap...
		NewPAK->PAKFile = fopen(FileName, "rb");
		fseek(NewPAK->PAKFile, FileHeader->MapOffset, SEEK_SET);

		byte *Temp = new byte[FileHeader->FileSize - FileHeader->MapOffset];
		if ( Temp == NULL )
		{
			SYS_Error(1, "PAK_Open: Not enough memory\n");
			SYS_Error(3, "  (", FileName, ")\n");

			fclose(NewPAK->PAKFile);
			delete NewPAK;
			delete FileHeader;
			return NULL;
		}//if

		if ( fread(Temp, FileHeader->FileSize - FileHeader->MapOffset, 1, NewPAK->PAKFile) < 1 )
		{
			SYS_Error(1, "PAK_Open: Error reading filemap\n");
			SYS_Error(3, "  (", FileName, ")\n");

			fclose(NewPAK->PAKFile);
			delete NewPAK;
			delete FileHeader;
			delete [] Temp;
			return NULL;
		}//if
		fclose(NewPAK->PAKFile);


	//---- Decompressing filemap....
		int Dump;
		PAK_FILE_BlockInfo *TempBlocks = (PAK_FILE_BlockInfo*)PAK_Decode(Temp, Dump, FileHeader->FileSize - FileHeader->MapOffset);
		delete [] Temp;

		if ( TempBlocks == NULL )
		{
			SYS_Error(1, "PAK_OpenFile:\n");
			SYS_Error(3, "  (", FileName, ")\n");

			delete NewPAK;
			delete FileHeader;
			return NULL;
		}//if


	//---- Setting up linked list with blockinfo...
		NewPAK->FirstBlock = new PAK_BlockInfo;
		if ( NewPAK->FirstBlock == NULL )
		{
			SYS_Error(1, "PAK_Open: Not enough memory\n");
			SYS_Error(3, "  (", FileName, ")\n");

			delete NewPAK;
			delete FileHeader;
			delete TempBlocks;
			return NULL;
		}//if

		PAK_FILE_BlockInfo *TempBlock = (PAK_FILE_BlockInfo*)NewPAK->FirstBlock;
		*TempBlock = TempBlocks[0];
		NewPAK->LastBlock = NewPAK->FirstBlock;

		for ( int C = 1; C < FileHeader->NrFiles; C++ )
		{
			NewPAK->LastBlock->NextBlock = new PAK_BlockInfo;
			if ( NewPAK->LastBlock->NextBlock == NULL )
			{
				SYS_Error(1, "PAK_Open: Not enough memory\n");
				SYS_Error(3, "  (", FileName, ")\n");

				delete NewPAK;
				delete TempBlocks;
				delete FileHeader;
				return NULL;
			}//if

			TempBlock = (PAK_FILE_BlockInfo*)NewPAK->LastBlock->NextBlock;
			*TempBlock = TempBlocks[C];
			NewPAK->LastBlock = NewPAK->LastBlock->NextBlock;
		}//for
	}//if

//---- Updating PAK-file-list
	if ( PAK_FirstPAK == NULL )
	{
		PAK_FirstPAK = NewPAK;
		PAK_LastPAK  = NewPAK;
	}//if
	else
	{
		PAK_LastPAK->NextPAK = NewPAK;
		PAK_LastPAK = NewPAK;
	}//else

	return NewPAK;
}//PAK_Open







/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Verify
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³In:	Param1: Filename of PAK-file to verify
³
³Out:  Returns pointer to Fileheader. NULL if error.
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

PAK_FILE_Header *PAK_Verify(const char *FileName)
{
	PAK_FILE_Header *TempHeader = new PAK_FILE_Header;
	if ( TempHeader == NULL )
	{
		SYS_Error(1, "PAK_Verify: Not enough memory to verify PAK-file\n");
		SYS_Error(3, "  (", FileName, ")\n");

		return NULL;
	}//if

	FILE *PAKFile = fopen(FileName, "rb");
	if ( PAKFile == NULL )
	{
		SYS_Error(1, "PAK_Verify: Couldn't open PAK-file\n");
		SYS_Error(3, "  (", FileName, ")\n");

		delete TempHeader;
		return NULL;
	}//if

	if ( fread(TempHeader, sizeof(PAK_FILE_Header), 1, PAKFile) < 1 )
	{
		SYS_Error(1, "PAK_Verify: Couldn't read PAK-header\n");
		SYS_Error(3, "  (", FileName, ")\n");

		delete TempHeader;
		fclose(PAKFile);
		return NULL;
	}//if

	fclose(PAKFile);

	if ( TempHeader->ID != PAK_FILE_ID )
	{
		SYS_Error(1, "PAK_Verify: Invalid PAK-file\n");
		SYS_Error(3, "  (", FileName, ")\n");

		delete TempHeader;
		return NULL;
	}//if

	return TempHeader;
}//PAK_Verify




/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_FindFile
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: PAK-fileinfo for file to search in
³	Param2: Filename to search for
³
³Out:  Returns pointer to blockinfo, NULL if file not found
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_BlockInfo *PAK_FindFile( const PAK_FileInfo *PAK, const char *FileName )
{
	char FNameUppercase[SYS_FileNameLength];
	for ( int C = 0; C <= strlen(FileName); C++ )
		FNameUppercase[C] = toupper(FileName[C]);

	PAK_BlockInfo *CurrentBlock = PAK->FirstBlock;
	while ( CurrentBlock != NULL )
	{
		if ( strcmp(FNameUppercase, CurrentBlock->FileName) == 0  )
			return CurrentBlock;

		CurrentBlock = CurrentBlock->NextBlock;
	}//while

	return NULL;
}//PAK_FindFile


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_ReadFile
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: PAK-file to read from
³	Param2: blockinfo for file to read
³
³Out:  Returns pointer filedata
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void *PAK_ReadFile( PAK_FileInfo *PAK, const PAK_BlockInfo *BlockInfo)
{

	byte *CompressedData = new byte[BlockInfo->Length_Compressed];
	if ( CompressedData == NULL )
	{
		SYS_Error(1, "PAK_ReadFile: Not enough memory\n");
		SYS_Error(5, "  (extracting ", BlockInfo->FileName, " from ", PAK->FileName, ")\n");
		return NULL;
	}//if

//---- Opening PAK-file
	PAK->PAKFile = fopen(PAK->FileName, "rb");
	if ( PAK->PAKFile == NULL )
	{
		SYS_Error(1, "PAK_ReadFile: Couldn't open PAK-file\n");
		SYS_Error(5, "  (extracting ", BlockInfo->FileName, " from ", PAK->FileName, ")\n");

		delete CompressedData;
		return NULL;
	}//if

//---- Reading compressed data...
	fseek(PAK->PAKFile, BlockInfo->Offset, SEEK_SET);
	if ( fread( CompressedData, BlockInfo->Length_Compressed, 1, PAK->PAKFile) < 1 )
	{
		SYS_Error(1, "PAK_ReadFile: Error reading compressed data from PAK-file\n");
		SYS_Error(5, "  (extracting ", BlockInfo->FileName, " from ", PAK->FileName, ")\n");

		fclose(PAK->PAKFile);
		delete CompressedData;

		return NULL;
	}//if

	fclose(PAK->PAKFile);

//---- Testing checksum...
	int Checksum = 0;
	for ( int C = 0; C < BlockInfo->Length_Compressed; C++ )
		Checksum += CompressedData[C];

	if ( Checksum != BlockInfo->Checksum )
	{
		SYS_Error(1, "PAK_ReadFile: Corrupt data (checksum error)\n");
		SYS_Error(5, "  (extracting ", BlockInfo->FileName, " from ", PAK->FileName, ")\n");

		delete CompressedData;
		return NULL;
	}//if

	int Dump;
	byte *FileData = (byte*)PAK_Decode( CompressedData, Dump, BlockInfo->Length_Compressed);
	delete CompressedData;
	if ( FileData == NULL )
	{
		SYS_Error(1, "PAK_ReadFile: PAK_Decode returned an error\n");
		SYS_Error(5, "  (extracting ", BlockInfo->FileName, " from ", PAK->FileName, ")\n");

		return NULL;
	}//if

	return FileData;
}//PAK_ReadFile


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_ReadFile
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: Filename to read
³
³Out:  Returns pointer to filedata, NULL if error
³
³Notes: Reads file from PAK OR disk
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int filflagg = 0;
fstream OutFile;

void *PAK_ReadFile( const char *FileName )
{

//---- Checking filename...
	if ( strlen( FileName ) >= SYS_FileNameLength  )
	{
		SYS_Error(1, "PAK_ReadFile: Filename too long\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if


/*//---- asdlfkjasl›dkfj
	if ( filflagg == 0 )
	{
		OutFile.open("x:\\demopak.txt", ios::out);

		if ( OutFile.fail() )
		{
			SYS_Error(1, "tullefeil.\n");
			return NULL;
		}//if
		filflagg = 1;
	}


	OutFile << "Pak_AddFile " << FileName << endl;//*/
//	printf("Loading: %s\n", FileName);



//---- Searching through PAKs for file...
	PAK_FileInfo *CurrentPAK = PAK_FirstPAK;
	PAK_BlockInfo *BlockInfo = NULL;

	while ( CurrentPAK != NULL )
	{
		BlockInfo = PAK_FindFile(CurrentPAK, FileName);
		if ( BlockInfo != NULL ) break;

		CurrentPAK = CurrentPAK->NextPAK;
	}//while


	void *FileData = NULL;

//---- Evt. reading file from PAK...

	if ( BlockInfo != NULL )
	{
		FileData = PAK_ReadFile( CurrentPAK, BlockInfo );
		if ( FileData == NULL )
		{
			SYS_Error(1, "PAK_ReadFile: PAK_ReadFile returned an error\n");
			SYS_Error(3, "  (", FileName, ")\n");
			return NULL;
		}//if
		return FileData;
	}//if


//---- File not found in PAKs, reading from disk...
	FILE *DataFile = fopen(FileName, "rb");
	if ( DataFile == NULL )
	{
		SYS_Error(1, "PAK_ReadFile: File not found\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if

	fseek(DataFile, 0, SEEK_END);
	int FileSize = ftell(DataFile);
	fseek(DataFile, 0, SEEK_SET);

	byte *Data = new byte[FileSize];
	if ( Data == NULL )
	{
		SYS_Error(1, "PAK_ReadFile: Not enough memory\n");
		SYS_Error(3, "  (", FileName, ")\n");
		fclose(DataFile);
		return NULL;
	}//if

	if ( fread(Data, FileSize, 1, DataFile) < 1 )
	{
		SYS_Error(1, "PAK_ReadFile: Error reading file\n");
		SYS_Error(3, "  (", FileName, ")\n");
		fclose(DataFile);
		delete [] Data;
		return NULL;
	}//if

	fclose(DataFile);
	return Data;
}//PAK_ReadFile



/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_FileSize
³ID           : PAK
³Last update  : 03.06.1998
³Optimized    : -
³
³In:	Param1: Filename
³
³Out:  Returns int = filesize
³
³Notes: Works with both PAK and disk
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int PAK_FileSize( const char *FileName)
{
//---- Checking filename...
	if ( strlen( FileName ) >= SYS_FileNameLength  )
	{
		SYS_Error(1, "PAK_FileSize: Filename too long.\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if


//---- Searching through PAKs for file...
	PAK_FileInfo *CurrentPAK = PAK_FirstPAK;
	PAK_BlockInfo *BlockInfo = NULL;

	while ( CurrentPAK != NULL )
	{
		BlockInfo = PAK_FindFile(CurrentPAK, FileName);
		if ( BlockInfo != NULL ) break;

		CurrentPAK = CurrentPAK->NextPAK;
	}//while

	if ( BlockInfo != NULL ) return BlockInfo->Length_Raw;


//---- File not found in PAKs, searching disk...
	FILE *File = fopen(FileName, "rb");
	if ( File == NULL )
	{
		SYS_Error(1, "PAK_FileSize: File not found.\n");
		SYS_Error(3, "  (", FileName, ")\n");
		return NULL;
	}//if

	fseek(File, 0, SEEK_END);
	int FileSize = ftell(File);
	fseek(File, 0, SEEK_SET);

	fclose(File);

	return FileSize;
}//PAK_FileSize





/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_Fileinfo (constructor)
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_FileInfo::PAK_FileInfo()
{
	PAKFile 	= NULL;
	memset(&FileName, 0, SYS_FileNameLength);
	NrFiles 	= 0;
	FirstBlock 	= NULL;
	LastBlock 	= NULL;

	NextPAK 	= NULL;
}//constructor


/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : ~PAK_Fileinfo (destructor)
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_FileInfo::~PAK_FileInfo()
{
	PAK_BlockInfo *NextBlock = FirstBlock;;
	PAK_BlockInfo *CurrentBlock;
	while ( NextBlock != NULL )
	{
		CurrentBlock = NextBlock;
		NextBlock = CurrentBlock->NextBlock;
		delete CurrentBlock;

	}

}//destructor




/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_BlockInfo (constructor)
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_BlockInfo::PAK_BlockInfo()
{
	memset(&FileName, 0, SYS_FileNameLength);

	Length_Raw 	  = 0;
	Length_Compressed = 0;
	Checksum 	  = 0;
	Offset 		  = 0;
	NextBlock 	  = NULL;
}//constructor





/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ WATCOM C++ ROUTINE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
³Name         : PAK_FILE_Header (constructor)
³ID           : PAK
³Last update  : 23.05.1998
³Optimized    : -
³
³Notes:
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
PAK_FILE_Header::PAK_FILE_Header()
{
	memset(&IDString, 0, 256);
	ID 		= PAK_FILE_ID;
	FileSize 	= sizeof(PAK_FILE_Header);
	NrFiles 	= 0;
	MapChecksum	= 0;				//no map
	MapOffset	= sizeof(PAK_FILE_Header);	//end of file
}//constructor










/*void main(void)
{
	if ( Aquarium() == 0 ) return;

	PAK_FileInfo *NewPAK = PAK_Create("jodling.pak", "Testing 123. Cyberfish/TLS 1998\n");
	if ( NewPAK == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		return;
	}//if


	if ( PAK_AddFile(NewPAK, "x:\\object\\box1.vec") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	if ( PAK_AddFile(NewPAK, "x:\\object\\box2.vec") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if


	if ( PAK_AddFile(NewPAK, "t.scn") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	if ( PAK_AddFile(NewPAK, "t2.scn") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	if ( PAK_AddFile(NewPAK, "w:\\test\\sides.tga") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	if ( PAK_AddFile(NewPAK, "w:\\test\\top.tga") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	if ( PAK_AddFile(NewPAK, "w:\\test\\bottom.tga") == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		delete NewPAK;
		return;
	}//if

	printf("OK\n");
}//main*/
