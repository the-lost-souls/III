# III - Live Tomorrow

## Background
This is all the source code and material for the demo 'III - Live Tomorrow' made by The Lost Souls. The demo finished 2nd at The Gathering 1999 in Hamar, Norway.

## Credits
Code, graphics, 3d modeling: Cyberfish/TLS
Graphics, 3D modeling: Rawhead/TLS
Music: Kezoomer/TLS

## Compiling
The source is a mix of C++ and hand optimized x86 assembler with dependencies on some rather archaic 3rd party libraries for loading jpegs, playing music etc. Originally, it was compiled for MSDOS using Watcom 11 and Turbo Assembler 5, but I have no idea what could be used today to build the binaries.

## The data files
All the 3D objects and scenes were done in Lightwave, then run through a homemade parser to produce binary files (.VEC and .SCN) that was then packaged into so-called 'PAK' files by yet another homemade zip-file-like system.
