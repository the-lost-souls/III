
FLR_FlagDefault 		EQU 0h
FLR_FlagFadeBehindObject        EQU 1h

FLR_Data      STRUC

FLR_NextOffset	DD ?
FLR_SortValue	DD ?
FLR_Type	DD ?
FLR_Priority	DD ?

FLR_Flags	DD ?

FLR_Scale       DD ?

FLR_R           DD ?
FLR_G           DD ?
FLR_B           DD ?

FLR_Intensity   DD ?
FLR_RealIntensity DD ?

FLR_X		DD ?
FLR_Y		DD ?
FLR_Z		DD ?

FLR_XMin        DD ?
FLR_YMin        DD ?
FLR_XMax        DD ?
FLR_YMax        DD ?

FLR_MapWidth    DD ?
FLR_MapHeigth   DD ?

FLR_Map         DD ?

	      ENDS
