#include "w:\midas111\include\midasdll.h"
#include <stdio.h>

void main(void)
{

//---- Starting MIDAS...
	if ( MIDASstartup() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		return;
	}//if

//---- Configuring MIDAS...
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

	MIDASsaveConfig("soundset");

//---- Initializing MIDAs
	if ( MIDASinit() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		MIDASclose();
		return;
	}//if


//---- Closing MIDAS...
	if ( MIDASclose() == FALSE )
	{
		int ErrorCode = MIDASgetLastError();
		char *ErrorStr;
		ErrorStr = MIDASgetErrorMessage(ErrorCode);
		printf(ErrorStr);
		return;
	}//if
	printf("Soundsetup updated successfully...\n");
}//main
