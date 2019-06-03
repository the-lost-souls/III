/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : CLWS.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 19.01.1999
쿌ction       : Converts a .lws-file into scn-format using LWS_SCN
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "w:\clwo\lws.h"
#include "w:\clwo\div.h"
#include "w:\pak\pak.h"
#include "w:\general\var.h"
#include "w:\vectors\scn_file.h"

int CLWS_Param(int ArgC, char *ArgV[], char *EnvP[]);

char InFile[256] = "\0";
char OutFile[256] = "\0";

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	if ( Aquarium() == 0 ) return;

	printf("LWS-converter v1.10 (012) Cyberfish/TLS 1998\n\n");

	if (CLWS_Param(ArgC, ArgV, EnvP) == 0)
	{
		SYS_Error(1, "CLWS: Error analyzing commandline parameters.\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	void *LWSFile;
	LWSFile = PAK_ReadFile(InFile);
	if ( LWSFile == 0 )
	{
		SYS_Error(1, "CLWS: Error reading .lws-file.\n");
		SYS_Error(3, "  ( ", InFile, " )\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	int FileSize = PAK_FileSize(InFile);
	if ( FileSize == 0 )
	{
		SYS_Error(1, "CLWS: Error getting filesize.\n");
		SYS_Error(3, "  ( ", InFile, " )\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	SCN_FILE_Header *SCNFile;
	SCNFile = LWS_SCN((byte*)LWSFile, FileSize);

	delete [] LWSFile;

	if ( SCNFile == NULL )
	{
		SYS_Error(1, "CLWS: Error converting .lws-file.\n");
		SYS_Error(5 , "  (", InFile, " -> ", OutFile, ")\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if


//---- Saving file...
	FILE *BINFile;
	BINFile = fopen(OutFile, "wb");
	if (BINFile == NULL)
	{
		SYS_Error(1, "CLWS: Error opening file.\n");
		SYS_Error(3, "  (", OutFile, ")\n");
		printf("%s", SYS_QuitMsg_);
		delete SCNFile;
		return;
	}//if

	if (fwrite(SCNFile, SCNFile->FileSize, 1, BINFile) < 1)
	{
		SYS_Error(1, "CLWS: Error writing file.\n");
		SYS_Error(3, "  (", OutFile, ")\n");
		delete [] SCNFile;
		fclose(BINFile);
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	printf("Converted %s into %s\n", InFile, OutFile);
	printf("Objects   : %d\n", SCNFile->NrObjects);
	printf("Lights    : %d\n", SCNFile->NrLights);

	delete [] SCNFile;
	fclose(BINFile);
}//main




int CLWS_Param(int ArgC, char *ArgV[], char *EnvP[])
{
	for ( int C1 = 1; C1 < ArgC; C1++ )
	{

	//---- Making parameter uppercase...
		int C3 = strlen(ArgV[C1]);
		for ( int C2 = 0; C2 < C3; C2++ ) ArgV[C1][C2] = toupper(ArgV[C1][C2]);


	//---- Switch?
		if ( ArgV[C1][0] == '-' )
		{
			switch ( ArgV[C1][1] )
			{
				default: 	SYS_Error(1, "CLWS_Param: Stupid parameter detected.\n");
						SYS_Error(3, "  (\"", ArgV[C1], "\")\n");
						return 0;

			}//switch
		}//if

		else if ( InFile[0] == 0 ) strcpy(InFile, ArgV[C1]);
		else if ( OutFile[0] == 0 ) strcpy(OutFile, ArgV[C1]);
		else
		{
			SYS_Error(1, "CLWS_Param: Stupid parameter detected.\n");
			SYS_Error(3, "  (\"", ArgV[C1], "\")\n");
			return 0;
		}//else
	}//for


	if (InFile[0] == '\0')
	{
		SYS_Error(1, "CLWS_Param: No .lws-file defined.\n");
		return 0;
	}//if


//---- Extending filenames...

	if ( SearchStr(InFile, '.') == 0) *(int*)&InFile[strlen(InFile)] = 'SWL.';
	if ( OutFile[0] == '\0' )
	{
		strcpy(OutFile, InFile);
		*(int*)&OutFile[strlen(InFile) - 4] = 'NCS.';
	}

	return 1;
}//AnalyzeParameters

