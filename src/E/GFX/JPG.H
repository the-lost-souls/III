/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : JPG.H
�Author       : Cyberfish/TLS
�Last update  : 15.03.99
�Action       : Definition of JPG_Load-routine + public variables.
�
�Notes :
����������������������������������������������������������������������������*/

#ifndef JPG_included
#define JPG_included


void *JPG_Load(const char *FileName);

extern  "C" int		JPG_Width;              //Image width
extern  "C" int 	JPG_Heigth;             //Image heigth
extern  "C" int 	JPG_Bits;               //Bits per pixel

#endif
