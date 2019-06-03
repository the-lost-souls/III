#include "w:\pak\pak.h"
#include "w:\general\var.h"
#include <fstream.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int PAKIT_ReadList(const char *FileName);


void main(int ArgC, char *ArgV[], char *EnvP[])
{
	if ( Aquarium() == 0 ) return;
	printf("PAKIT - v1.00 (001) Cyberfish/TLS 1998\n\n");

	if ( ArgC == 1 )
	{
		SYS_Error(1, "PAKIT: No command line parameters.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	if ( PAKIT_ReadList(ArgV[1]) == 0 )
	{
		SYS_Error(1, "PAKIT: Error reading listfile.\n");
		printf(SYS_QuitMsg_);
		return;
	}//if

	printf(SYS_QuitMsg_);


}//main


int PAKIT_ReadList(const char *FileName)
{
	fstream ListFile;
	char Line[256];
	PAK_FileInfo *PAK = NULL;

//---- Opening file...
	ListFile.open(FileName, ios::nocreate | ios::in ) ;
	if ( ListFile.fail() )
	{
		SYS_Error(1, "PAKIT_ReadList: Error opening listfile.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return 0;
	}//if

//---- Reading commands...
	while ( !ListFile.eof() )
	{
		ListFile >> Line;
		for ( int I = 0; I < strlen(Line); I++ ) Line[I] = toupper(Line[I]);

		if ( strcmp(Line, "PAK_CREATE") == 0 )
		{
			char NewFile[256];
			char IDString[256];
			ListFile >> NewFile;
			ListFile >> IDString;

			printf("Running PAK_Create( %s, %s )\n", NewFile, IDString);

			PAK = PAK_Create(NewFile, IDString);
			if ( PAK == NULL )
			{
				SYS_Error(1, "PAKIT_ReadList: Error creating new pak-file.\n");
				SYS_Error(3, "  ( ", Line, " )\n");
				return 0;
			}//if
		}//if

		if ( strcmp(Line, "PAK_OPEN") == 0 )
		{
			ListFile >> Line;
			printf("Running PAK_Open( %s )\n", Line);

			if ( PAK != NULL )
			{
				SYS_Error(1, "PAKIT_ReadList: A PAK-file is already opened.\n");
				return 0;
			}//if



			PAK = PAK_Open(Line);
			if ( PAK == NULL )
			{
				SYS_Error(1, "PAKIT_ReadList: Error opening pak-file.\n");
				SYS_Error(3, "  ( ", Line, " )\n");
				return 0;
			}//if
		}//if

		if ( strcmp(Line, "PAK_ADDFILE") == 0 )
		{
			ListFile >> Line;
			printf("Running PAK_AddFile( PAK, %s )\n", Line);

			if ( PAK == NULL )
			{
				SYS_Error(1, "PAKIT_ReadList: Trying to add file when no pak is opened.\n");
				SYS_Error(3, "  ( ", Line, " )\n");
				return 0;
			}//if


			if ( PAK_AddFile(PAK, Line) == 0 )
			{
				SYS_Error(1, "PAKIT_ReadList: Error adding file to pak.\n");
				SYS_Error(3, "  ( ", Line, " )\n");
				return 0;
			}//if
		}//if

		if ( strcmp(Line, "PAK_INFO") == 0 )
		{
			printf("Running PAK_Info\n", Line);
			if ( PAK == NULL )
			{
				SYS_Error(1, "PAKIT_ReadList: No PAK-file opened.\n");
				return 0;
			}//if

			printf("    FileName        : %s\n", PAK->FileName);
			printf("    PAK containing...\n");
			PAK_BlockInfo *Block = PAK->FirstBlock;

			int TotalCompressed = 0;
			int TotalRaw = 0;
			while ( Block != NULL )
			{
				TotalCompressed += Block->Length_Compressed;
				TotalRaw += Block->Length_Raw;

				printf("      %2.2f%% %s\n", 100*(1 - (float)Block->Length_Compressed/Block->Length_Raw), Block->FileName);
				Block = Block->NextBlock;

			}//while
			printf("    Number of files : %d\n\n", PAK->NrFiles);
			printf("    Uncompressed total size: %d\n", TotalRaw);
			printf("    Compressed total size: %d\n", TotalCompressed);
			printf("    Average compression ratio: %f\n", (float)TotalCompressed/TotalRaw);
			printf("\n");

		}//if
	}//while

	SYS_Error(1, "PAK_ReadList: No more commands.\n");
	return 1;
}//ReadList

