
#define Aquarium_ENGINE

#include "w:\general\var.h"
#include "w:\vectors\scn_file.h"
#include "w:\vectors\scn_run.h"
#include "w:\divlibs\midas112\include\midasdll.h"
#include "w:\general\sort.h"
#include "w:\vectors\lights.h"
#include "w:\general\canvas.h"
#include "w:\general\main.h"
#include "w:\pak\pak.h"
#include "w:\gfx\gfx.h"
#include "w:\general\2d.h"
#include "w:\general\filters.h"
#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <fstream.h>

extern "C" void MIDAS_CALL TimerTick();
MIDASmodule Module = NULL;
MIDASmodulePlayHandle ModPlayHandle = NULL;
MIDASplayStatus status;
int C;
int time, starttime;
int row;
int times[50];

fstream OutFile;

void main(int ArgC, char *ArgV[], char *EnvP[])
{
	SYS_XRes = 320;
	SYS_YRes = 240;


	if ( Aquarium() == NULL ) return;

//	PAK = PAK_Open("island.pak");
//	if ( PAK == NULL ) goto error;

//---- Setting up MIDAS-timer-stuff...
	if ( MIDASstartup() == FALSE ) goto error;
	if ( MIDASloadConfig("soundset") == FALSE ) goto error;
//	if ( MIDASsetOption(MIDAS_OPTION_FORCE_NO_SOUND, 1) == FALSE) goto error;
	if ( MIDASinit() == FALSE ) goto error;

	if ( MIDASsetTimerCallbacks(SYS_TicksPerSec*1000, FALSE, &TimerTick, NULL, NULL) == FALSE) goto error;//*/

//---- Starting backgroundplay...
	if ( MIDASstartBackgroundPlay(0) == FALSE ) goto error;


//---- Loading module...
	Module = MIDASloadModule("c:\\frityr\\whammer7.xm");
	if ( Module == 0 ) goto error;

	status.position = 0;
	row = -1;

	ModPlayHandle = MIDASplayModuleSection(Module, 0, 40, 1, TRUE);
	starttime = SYS_TimerTick;
	if ( ModPlayHandle == 0 ) goto error;//*/

	while ( status.position < 40 )
	{
		MIDASgetPlayStatus(ModPlayHandle, &status);
		if ( status.position != row )
		{
			time = SYS_TimerTick - starttime;
			row = status.position;
			times[row] = time;
			printf("Order %d: %d\n", row, time);
		}
		__asm{
	                mov    ah, 0Bh
	                int    21h
			xor	edx, edx
			mov	dl, al
			mov	C, edx
		}
		if ( C ) break;
	}//while

	OutFile.open("times.txt", ios::out);
	if ( OutFile.fail() ) goto error;

	for ( C = 0; C < row; C++ )
	{
		OutFile << "Order " << C << ": " << times[C] << endl;
	}//for

	OutFile.close();
error:

	MIDASclose();

	int ErrorCode = MIDASgetLastError();
	char *ErrorString = MIDASgetErrorMessage(ErrorCode);
	printf("MIDAS: ");
	printf(ErrorString);
	printf("\n");
	printf(SYS_QuitMsg_);
}//main
