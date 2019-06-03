////////////////////////////////////////
// prometheus truecolor configuration //
////////////////////////////////////////

#ifndef __PTC_CONFIG_H
#define __PTC_CONFIG_H

#include "lang.h"







#ifdef __DOS32__

    // system
    #define __X86__

    // interfaces
    #define __VGA__
    #define __VESA__

    // options
    //#define __MATROX__

    // modules
    #define __DPMI__

    // converter config
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__
    #define __FAKEMODE__

#endif



#ifdef __WIN32__

    // system
    #define __X86__

    // interfaces
    #define __GDI__
    #define __DIRECTX__

    // options
    #define __VFW__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__


#endif



#ifdef __LINUX__

    // system
    #define __X86__
    #define __GNU__

    // interfaces
    #define __SVGALIB__
    #define __X11__
    #define __GGI__

    // options
    #define __MITSHM__
    #define __FAKEMODE__

    // converters
    #define __8BIT__
    #define __16BIT__   
    #define __32BIT__

#endif







#ifdef __X86__

    // intel little endian
    #define __LITTLE_ENDIAN__
    #undef  __BIG_ENDIAN__    

#else

    // configure...
    #define __LITTLE_ENDIAN__
    #undef  __BIG_ENDIAN__

#endif






// cygnus cannot handle DirectX yet
#ifdef __CYGNUS__
#undef __DIRECTX__
#endif








#endif
