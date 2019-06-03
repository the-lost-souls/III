CPP = wpp386.exe
ASM = tasm.exe
LIB = wlib.exe
EXE = wlink.exe

CPPFLAGS = -d2
ASMFLAGS = /m4 /ml /zi
LIBFLAGS =
CPPINCLUDE = -ic:\code\prog\watcom11\h
EXEFLAGS = debug dwarf


#############################################################################

midsetup.exe: midsetup.obj
       wlink $(EXEFLAGS) @midsetup.lnk

midsetup.obj: midsetup.cpp
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp

lists.obj: lists.asm
	$(ASM) $(ASMFLAGS) $*.asm





