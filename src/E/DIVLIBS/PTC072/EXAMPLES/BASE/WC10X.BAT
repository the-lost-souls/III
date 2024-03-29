@echo off
echo:

rem ************************************************
rem *      Watcom C++ 10.x PTC example build       *
rem * -------------------------------------------- *
rem * Syntax: wc10x [example] (platform) (library) *
rem *                                              *
rem * [example]  = example source name             *
rem * (platform) = "dos" or "win32"                *
rem * (library)  = "debug" or "release"            *
rem *                                              *
rem * NOTE: to compile the examples for the win32  *
rem * platform it is necessary to use the /binnt   *
rem * watcom binaries. this is because the dos     *
rem * binaries have a limit on the size of command *
rem * line arguments.                              *
rem ************************************************

if not exist "%1.cpp" goto done

if "%2" == "dos"   goto dos
if "%2" == "win32" goto win32




rem ****************
rem * Dos platform *
rem ****************
:dos

if "%3" == "debug" goto dos-debug

echo Building "%1.cpp" : Watcom C++ 10.x dos release
echo:
wpp386 %1.cpp -i=..\..\source -bt=dos -oneatxl+ -5r -fp5 -fpi87
wlink name %1.exe file %1.obj library ..\..\library\dos\wc10.x\release.lib system dos4g option stack=256k
goto done

:dos-debug
echo Building "%1.cpp" : Watcom C++ 10.x dos debug
echo:
wpp386 %1.cpp -i=..\..\source -bt=dos
wlink name %1.exe file %1.obj library ..\..\library\dos\wc10.x\debug.lib system dos4g option stack=256k
goto done




rem ******************
rem * Win32 platform *
rem ******************
:win32

if "%3" == "debug" goto win32-debug

echo Building "%1.cpp" : Watcom C++ 10.x win32 release
echo:
rc -r windows.rc
wpp386 %1.cpp -i=..\..\source -bm -bt=nt -oneatxl+ -5r -fp5 -fpi87
wlink name %1.exe file %1.obj library ..\..\library\win32\wc10.x\release.lib system nt option stack=256k, resource=windows.res
goto done

:win32-debug
echo Building "%1.cpp" : Watcom C++ 10.x win32 debug
echo:
rc -r windows.rc
wpp386 %1.cpp -i=..\..\source -bm -bt=nt
wlink name %1.exe file %1.obj library ..\..\library\win32\wc10.x\debug.lib system nt option stack=256k, resource=windows.res
goto done




:done
