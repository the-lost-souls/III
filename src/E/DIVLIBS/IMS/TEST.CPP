#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "binfstd.h"
#include "xmplay.h"
#include "gmdplay.h"
#include "mcp.h"
#include "ims.h"
#include "itplay.h"
#include <iostream.h>

void main(int argc, char **argv)
{

  imsinitstruct is;
  imsFillDefaults(is);
  is.bufsize=65536; // 1sec buffer
  is.pollmin=61440; // use polling only after 0.9375sec
//  is.usersetup=0;
  int i;
  for (i=1; i<argc; i++)
    if (!stricmp(argv[i], "nosound"))
      is.usequiet=1;
  if (!imsInit(is))
  {
    cputs("could not init\r\n");
    return;
  }




/*  gmdmodule mod;
  sbinfile fil;
  fil.open("c:\\prog\\st\\songs\\play1.s3m", fil.openro);
  mpLoadS3M(mod, fil);
  fil.close();
  mpReduceSamples(mod);
  mpLoadSamples(mod);
  mpPlayModule(mod);

  while (!kbhit());
  while (kbhit())
    getch();

  mpStopModule();
  mpFree(mod);//*/



/*  xmodule mod;
  sbinfile fil;
  if (!fil.open("c:\\prog\\st\\songs\\backup.xm", fil.openro))
  {
    cputs("could not open\r\n");
    return;
  }
  if (xmpLoadMOD(mod, fil))
  {
    cputs("could not load\r\n");
    return;
  }
  fil.close();

  if (!xmpLoadSamples(mod))
  {
    cputs("could not upload\r\n");
    return;
  }
  if (!xmpPlayModule(mod))
  {
    cputs("could not play\r\n");
    return;
  }

  xmpSetEvPos(0, 0x20000, 2, 8);
  xmpSetEvPos(1, 0x20400, 2, 8);
  while (!kbhit())
  {
    int time1,time2,time3;
    if (mcpIdle)
      mcpIdle();
    while (inp(0x3da)&8);
    while (!(inp(0x3da)&8));
    cerr << "\r" << hex << xmpGetRealPos();
    xmpGetEvPos(0, time1);
    xmpGetEvPos(1, time2);
    outp(0x3c8,0);
    outp(0x3c9,(time1<4096)?(63-(time1/64)):0);
    outp(0x3c9,0);
    outp(0x3c9,(time2<8192)?(63-(time2/128)):0);
  }
  while (kbhit())
    getch();

  xmpStopModule();
  xmpFreeModule(mod);*/



  static itplayerclass itplayer;
  itplayerclass::module mod;

  sbinfile fil;
  //cputs("opening\r\n");
  if (!fil.open(argv[1], fil.openro))
  {
    cputs("could not open\r\n");
    return;
  }
  //cputs("loading\r\n");
  if (mod.load(fil))
  {
    cputs("could not load\r\n");
    return;
  }
  fil.close();

  //cputs("uploading\r\n");
  if (!itplayer.loadsamples(mod))
  {
    cputs("could not upload\r\n");
    return;
  }
  //cputs("playing\r\n");
  if (!itplayer.play(mod, 64))
  {
    cputs("could not play\r\n");
    return;
  }

  itplayer.setevpos(0, 0, 2, 4);
  while (!kbhit())
  {
    int time;
    cerr << "\r" << itplayer.getrowtime() << "frames/row\t";
  }

  while (kbhit())
    getch();

  itplayer.stop();
  mod.free();//*/


  imsClose();
}
