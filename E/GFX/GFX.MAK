#######################################################################
# Makefile for all libraries used in 3d-project....
#
# Code:
#	TOE/ Decker's Union
#	Cyberfish/ TLS
#
#
#
# Libraries handled by this makefile:
#	w:\general\sys.lib
#	w:\gfx\gfx.lib
#	w:\render\render.lib
#	w:\vectors\vectors.lib
#	w:\vesa\vbe.lib
#
#######################################################################



#######################################################################
#Programs used:

CPP = wpp386.exe
ASM = tasm.exe
LIB = wlib.exe
EXE = wlink.exe

#######################################################################
#Program flags

CPPFLAGS = -d2
ASMFLAGS = /m4 /ml /zi
LIBFLAGS =
CPPINCLUDE = -ic:\code\prog\watcom11\h
EXEFLAGS = debug dwarf


#######################################################################
#
#Start of library dependencies
#
#######################################################################

#Path definitions

VBEPATH = w:\vbe
SYSPATH = w:\general
VECPATH = w:\vectors
RENPATH = w:\render
GFXPATH = w:\gfx

#Library names

SYSLIB = $(SYSPATH)\general.lib
VBELIB = $(VBEPATH)\vbe.lib
VECLIB = $(VECPATH)\vectors.lib
RENLIB = $(RENPATH)\render.lib
GFXLIB = $(GFXPATH)\gfx.lib



######################################################################
#library w:\gfx\gfx.lib

GFXOBJ = $(GFXPATH)\tga.obj

$(GFXLIB): $(GFXOBJ)
	%write $(GFXPATH)\gfx.lnk +-$(GFXPATH)\tga.obj
	wlib $(GFXLIB) @$(GFXPATH)\gfx.lnk

$(GFXPATH)\tga.obj: $(GFXPATH)\tga.cpp $(SYSPATH)\var.h
		    $(CPP) $(CPPFLAGS) $(CPPINCLUDE) $(GFXPATH)\tga.cpp -fo=$(GFXPATH)\tga.obj



