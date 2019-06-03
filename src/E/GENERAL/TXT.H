/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : TXT.H
�Author       : Cyberfish/TLS
�Last update  : 08.08.1998
�Action       :
�
�Notes :
����������������������������������������������������������������������������*/

#ifndef TXT_included
#define TXT_included

#include "w:\general\var.h"

class TXT_Data
{
public:
	TXT_Data(void);
	~TXT_Data(void);

	void	*Texture;
	int	Width;
	int	Heigth;
	int	Bits;
	int	Users;
	int     UserID;
	char	FileName[SYS_FileNameLength];
	TXT_Data *NextTXT;
private:
};

TXT_Data *TXT_Load(	const char *FileName,
			uint32 UserID);

void *TXT_ForceLoad(	const char *FileName,
			int &Width,
			int &Heigth);

void TXT_Unload(void *Texture);

extern "C" float TXT_ScalingFactor;
extern "C" uint32 TXT_ScalingFlag;

#endif