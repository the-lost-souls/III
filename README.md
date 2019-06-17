# III - Live Tomorrow

[![screenshot](https://the-lost-souls.github.io/images/thumbnails/III.jpg)](https://youtu.be/rMkFe3xxZmA)

This is all the source code and material for the demo 'III - Live Tomorrow' made by The Lost Souls. The demo finished 2nd at The Gathering 1999 in Hamar, Norway.

Check out the [nfo](src/TLS_III.NFO) for credits and the [TLS pages](https://the-lost-souls.github.io) for more demos.

## Compiling
The source is a mix of C++ and hand optimized x86 assembler with dependencies on some rather archaic 3rd party libraries for loading jpegs, playing music etc. Originally, it was compiled for MSDOS using Watcom 11 and Turbo Assembler 5, so I building it today will be a bit of a challenge.

- src/E is the engine
- src/DEMOSTUF is the data + the demo-specific code

## The data files
All the 3D objects and scenes were done in Lightwave, then run through a homemade parser to produce binary files (.VEC and .SCN) that was then packaged into so-called 'PAK' files by yet another homemade zip-file-like system.
