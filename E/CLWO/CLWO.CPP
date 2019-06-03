/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : CLWO.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 29.09.1998
쿌ction       : Converts a .lwo-file into vec-format using LWO_VEC
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include "div.h"
#include "lwo.h"
#include "w:\pak\pak.h"
#include "w:\general\var.h"
#include "w:\vectors\vec_file.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int CLWO_Param(int ArgC, char *ArgV[], char *EnvP[]);

char InFile[256] = "\0";
char OutFile[256] = "\0";
int Type;

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	if ( Aquarium() == 0 ) return;
	printf("LWO-converter v2.10 (011) Cyberfish/TLS 1998\n\n");

	if (CLWO_Param(ArgC, ArgV, EnvP) == 0)
	{
		SYS_Error(1, "CLWO: Error analyzing commandline parameters.\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	void *LWOFile;
	LWOFile = PAK_ReadFile(InFile);
	if ( LWOFile == 0 )
	{
		SYS_Error(1, "CLWO: Error reading .lwo-file.\n");
		SYS_Error(3, "  (", InFile, ")\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	VEC_FILE_Header *VECFile;
	VECFile = LWO_VEC(LWOFile, Type);

	delete [] LWOFile;

	if ( VECFile == NULL )
	{
		SYS_Error(1, "CLWO: Error converting .lwo-file\n");
		SYS_Error(5 , "  (", InFile, " -> ", OutFile, ")\n");
		printf("%s", SYS_QuitMsg_);
		return;
	}//if


//---- Saving file...
	FILE *BINFile;
	BINFile = fopen(OutFile, "wb");
	if (BINFile == NULL)
	{
		SYS_Error(1, "CLWO: Error opening file.\n");
		SYS_Error(3, "  (", OutFile, ")\n");
		printf("%s", SYS_QuitMsg_);
		delete VECFile;
		return;
	}//if

	if (fwrite(VECFile, VECFile->FileSize, 1, BINFile) < 1)
	{
		SYS_Error(1, "CLWO: Error writing file.\n");
		SYS_Error(3, "  (", OutFile, ")\n");
		delete [] VECFile;
		fclose(BINFile);
		printf("%s", SYS_QuitMsg_);
		return;
	}//if

	printf("Converted %s into %s\n", InFile, OutFile);
	printf("Type      : %d\n", VECFile->Type);
	printf("Vertices  : %d\n", VECFile->Vertices);
	printf("Faces     : %d\n", VECFile->Faces);
	printf("Materials : %d\n", VECFile->Materials);

	delete [] VECFile;
	fclose(BINFile);
}//main


int CLWO_Param(int ArgC, char *ArgV[], char *EnvP[])
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
				case 'T' :      if (!Str2Int(&ArgV[C1][2], Type, '\0') )
						{
							SYS_Error(1, "CLWO_Param: Error in type-parameter.\n");
							return 0;
						}//if
						break;

				default: 	SYS_Error(1, "CLWO_Param: Stupid parameter detected!\n");
						SYS_Error(3, "  (", ArgV[C1], " )\n");
						return 0;

			}//switch
		}//if

		else if ( InFile[0] == 0 ) strcpy(InFile, ArgV[C1]);
		else if ( OutFile[0] == 0 ) strcpy(OutFile, ArgV[C1]);
		else
		{
			SYS_Error(2, "CLWO_Param: Stupid parameter detected!\n", ArgV[C1]);
			return 0;
		}//else
	}//for


	if (InFile[0] == '\0')
	{
		SYS_Error(1, "CLWO_Param: No .lwo-file defined on command line.\n");
		return 0;
	}//if


//---- Extending filenames...

	if ( SearchStr(InFile, '.') == 0) *(int*)&InFile[strlen(InFile)] = 'OWL.';
	if ( OutFile[0] == '\0' )
	{
		strcpy(OutFile, InFile);
		*(int*)&OutFile[strlen(InFile) - 4] = 'CEV.';
	}

	return 1;
}//AnalyzeParameters
