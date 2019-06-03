/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : JPG.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 15.03.99
쿌ction       : Definition of JPG_Load-routine + public variables.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef JPG_included
#define JPG_included


void *JPG_Load(const char *FileName);

extern  "C" int		JPG_Width;              //Image width
extern  "C" int 	JPG_Heigth;             //Image heigth
extern  "C" int 	JPG_Bits;               //Bits per pixel

#endif

