#include "w:\midas112\include\midasdll.h"
#include <stdio.h>

void main(void)
{

//---- Starting MIDAS...
	printf("Running MIDASstartup()\n");
	if ( MIDASstartup() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		return;
	}//if

//---- Configuring MIDAS...
	printf("Running MIDASconfig()\n");
	if ( MIDASconfig() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		if ( ErrorCode == 0 ) printf("MIDAS: User escaped setup\n");
		else
		{
			ErrorStr = MIDASgetErrorMessage(ErrorCode);
			printf(ErrorStr);
		}//else
		MIDASclose();
		return;
	}//if


//---- Initializing MIDAs
	printf("Running MIDASinit()\n");
	if ( MIDASinit() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASclose();
		return;
	}//if


//---- Starting backgroundplay...
	printf("Running MIDASstartBackgroundPlay()\n");
	if ( MIDASstopBackgroundPlay() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASclose();
		return;
	}//if


//---- Loading module...
	printf("Loading module...\n");
	printf("Running MIDASloadModule(" ")\n");

	MIDASmodule Mod = NULL;
	MIDASmodulePlayHandle ModPlayHandle = NULL;

	Mod = MIDASloadModule("H:\\tekonly7.it");
	if ( Mod == 0 )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASclose();
		return;
	}//if

	printf("Playing module... - press any key\n");
	printf("Running MIDASplayModule(  )\n");
	__asm{
		mov	ah, 01
		int	21h
	}//

	ModPlayHandle = MIDASplayModuleSection(Mod, 44, 81, 1, TRUE);
	if ( ModPlayHandle == 0 )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);

		MIDASfreeModule(Mod);
		MIDASclose();
		return;
	}

	__asm
	{
		pushad

		mov	ah, 01h
		int	21h

		popad
	}//

//---- Stopping module...
	printf("Running MIDASstopModule( )\n");
	if ( MIDASstopModule(ModPlayHandle) == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASfreeModule(Mod);
		MIDASclose();
		return;
	}//if



//---- Removing module...
	printf("Running MIDASfreeModule( )\n");
	if ( MIDASfreeModule(Mod) == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASclose();
		return;
	}//if



//---- Stopping backgroundplay...
	printf("Running MIDASstopBackgroundPlay(0)\n");
	if ( MIDASstopBackgroundPlay() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);

		MIDASfreeModule(Mod);
		MIDASclose();
		return;
	}//if



//---- Closing MIDAS...
	printf("Running MIDASclose()\n");
	if ( MIDASclose() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		return;
	}//if
}//main
