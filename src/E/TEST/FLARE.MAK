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

SYSLIB = $(SYSPATH)\sys.lib
VBELIB = $(VBEPATH)\vbe.lib
VECLIB = $(VECPATH)\vectors.lib
RENLIB = $(RENPATH)\render.lib
GFXLIB = $(GFXPATH)\gfx.lib


#####################################################################
#Exefile...

flare.exe: $(SYSLIB) $(GFXLIB) $(VECLIB) $(RENLIB) flare.obj lists.obj callback.obj w:\plugins\plugins.lib w:\vbe\vbe.lib
       wlink $(EXEFLAGS) @flare.lnk

flare.obj: flare.cpp $(SYSPATH)\var.h
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp

lists.obj: lists.asm
	$(ASM) $(ASMFLAGS) $*.asm

callback.obj: callback.asm $(SYSPATH)\var.inc $(SYSPATH)\var.asm
	$(ASM) $(ASMFLAGS) $*.asm


######################################################################
#library w:\general\sys.lib

SYSOBJ = $(SYSPATH)\var.obj $(SYSPATH)\main.obj $(SYSPATH)\sort.obj $(SYSPATH)\canvas.obj $(SYSPATH)\aquarium.obj

$(SYSLIB): $(SYSOBJ)
	$(LIB) $(LIBFLAGS) $(SYSLIB) @w:\general\general.lnk

$(SYSPATH)\var.obj: $(SYSPATH)\var.asm $(SYSPATH)\var.h $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(SYSPATH)\var.obj

$(SYSPATH)\main.obj: $(SYSPATH)\main.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(SYSPATH)\main.obj

$(SYSPATH)\sort.obj: $(SYSPATH)\sort.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(SYSPATH)\sort.obj

$(SYSPATH)\canvas.obj: $(SYSPATH)\canvas.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(SYSPATH)\canvas.obj

$(SYSPATH)\aquarium.obj: $(SYSPATH)\aquarium.cpp $(SYSPATH)\var.h
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(SYSPATH)\aquarium.obj



######################################################################
#library w:\vectors\vectors.lib

VECOBJ = $(VECPATH)\loadobj.obj $(VECPATH)\loadscn.obj $(VECPATH)\v01.obj $(VECPATH)\v03.obj $(VECPATH)\v04.obj $(VECPATH)\v05.obj $(VECPATH)\v06.obj $(VECPATH)\v07.obj $(VECPATH)\scene.obj $(VECPATH)\lights.obj

$(VECLIB): $(VECOBJ)
	$(LIB) $(LIBFLAGS) $(VECLIB) @$(VECPATH)\vectors.lnk

$(VECPATH)\loadobj.obj: $(VECPATH)\loadobj.cpp $(SYSPATH)\var.h
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(VECPATH)\loadobj.obj

$(VECPATH)\loadscn.obj: $(VECPATH)\loadscn.cpp $(SYSPATH)\var.h $(VECPATH)\scene.h $(VECPATH)\scene.obj
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(VECPATH)\loadscn.obj

$(VECPATH)\v01.obj: $(VECPATH)\v01.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v01macro.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v01.obj

$(VECPATH)\v03.obj: $(VECPATH)\v03.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v03macro.inc $(VECPATH)\lights.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v03.obj

$(VECPATH)\v04.obj: $(VECPATH)\v04.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v04macro.inc $(VECPATH)\lights.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v04.obj

$(VECPATH)\v05.obj: $(VECPATH)\v05.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v05macro.inc $(VECPATH)\lights.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v05.obj

$(VECPATH)\v06.obj: $(VECPATH)\v06.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v06macro.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v06.obj

$(VECPATH)\v07.obj: $(VECPATH)\v07.asm $(SYSPATH)\var.asm $(SYSPATH)\var.inc $(VECPATH)\v07macro.inc $(VECPATH)\lights.inc
	$(ASM) $(ASMFLAGS) $*.asm $(VECPATH)\v07.obj

$(VECPATH)\scene.obj: $(VECPATH)\scene.cpp $(SYSPATH)\var.inc $(VECPATH)\loadobj.obj $(VECPATH)\scene.h
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(VECPATH)\scene.obj

$(VECPATH)\lights.obj: $(VECPATH)\lights.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $(VECPATH)\lights.asm $(VECPATH)\lights.obj





######################################################################
#library w:\render\render.lib

RENOBJ = $(RENPATH)\t02_org.obj $(RENPATH)\t04_org.obj $(RENPATH)\t05.obj $(RENPATH)\t06_org.obj $(RENPATH)\t07_org.obj $(RENPATH)\t08_org.obj

$(RENLIB): $(RENOBJ)
	$(LIB) $(LIBFLAGS) $(RENLIB) @$(RENPATH)\render.lnk

$(RENPATH)\t02_org.obj: $(RENPATH)\t02_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $(RENPATH)\t02_org.asm $(RENPATH)\t02_org.obj

$(RENPATH)\t02.obj: $(RENPATH)\t02.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $(RENPATH)\t02.asm $(RENPATH)\t02.obj

$(RENPATH)\t04_org.obj: $(RENPATH)\t04_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t04_org.obj

$(RENPATH)\t05_org.obj: $(RENPATH)\t05_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t05_org.obj

$(RENPATH)\t05.obj: $(RENPATH)\t05.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t05.obj

$(RENPATH)\t06_org.obj: $(RENPATH)\t06_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t06_org.obj

$(RENPATH)\t07_org.obj: $(RENPATH)\t07_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t07_org.obj

$(RENPATH)\t08_org.obj: $(RENPATH)\t08_org.asm $(SYSPATH)\var.inc
	$(ASM) $(ASMFLAGS) $*.asm $(RENPATH)\t08_org.obj





######################################################################
#library w:\gfx\gfx.lib

GFXOBJ = $(GFXPATH)\tga.obj

$(GFXLIB): $(GFXOBJ)
	$(LIB) $(LIBFLAGS) $(GFXLIB) -+$?

$(GFXPATH)\tga.obj: $(GFXPATH)\tga.cpp $(GFXPATH)\tga.h $(SYSPATH)\var.h $(SYSPATH)\var.asm
			$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(GFXPATH)\tga.obj






######################################################################
#library w:\plugins\plugins.lib

w:\plugins\plugins.lib: w:\plugins\move.obj w:\plugins\glow.obj w:\plugins\loadglow.obj
	$(LIB) $(LIBFLAGS) w:\plugins\plugins.lib @w:\plugins\plugins.lnk

w:\plugins\move.obj: w:\plugins\move.asm $(SYSPATH)\var.h $(SYSPATH)\var.asm
	$(ASM) $(ASMFLAGS) $*.asm w:\plugins\move.obj

w:\plugins\loadglow.obj: w:\plugins\loadglow.cpp $(SYSPATH)\var.h $(GFXPATH)\tga.h
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=w:\plugins\loadglow.obj

w:\plugins\glow.obj: w:\plugins\glow.asm $(SYSPATH)\var.inc $(SYSPATH)\var.asm
	$(ASM) $(ASMFLAGS) $*.asm w:\plugins\glow.obj


######################################################################
#library w:\vbe\vbe.lib

VBEOBJ = $(VBEPATH)\vbe.obj $(VBEPATH)\svga.lib $(VBEPATH)\uvbelib.lib $(VBEPATH)\pmode.lib

$(VBELIB): $(VBEOBJ)
	$(LIB) -n $(LIBFLAGS) $(VBELIB) @$(VBEPATH)\vbe.lnk

$(VBEPATH)\vbe.obj: $(VBEPATH)\vbe.cpp $(VBEPATH) $(SYSPATH)\var.obj
	$(CPP) $(CPPFLAGS) $(CPPINCLUDE) $*.cpp -fo=$(VBEPATH)\vbe.obj

