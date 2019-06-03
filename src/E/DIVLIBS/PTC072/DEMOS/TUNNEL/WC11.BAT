echo Building "%1.cpp" : Watcom C++ 11.0 dos release
wpp386 tunnel.cpp -i=..\..\source -i=c:\code\prog\watcom11\h -bt=dos -oneatxhl+ -5r -fp5 -fpi87
wlink name tunnel.exe file tunnel.obj library ..\..\library\dos\wc11.0\release.lib system dos4g option stack=256k
