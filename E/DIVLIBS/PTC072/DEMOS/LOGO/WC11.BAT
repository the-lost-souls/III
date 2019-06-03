echo Building ".cpp" : Watcom C++ 11.0 dos release
wpp386 logo.cpp -d2 -i=..\..\source -i=c:\code\prog\watcom11\h\ -bt=dos -oneatxhl+ -5r -fp5 -fpi87
wlink debug dwarf name logo.exe file logo.obj library ..\..\library\dos\wc11.0\release.lib system dos4g option stack=256k
