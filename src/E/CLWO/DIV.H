/*
�������������������������� WATCOM C++ INCLUDEFILE ��������������������������
�Name         : DIV.H
�Author       : Cyberfish/TLS
�Last update  : 30.07.1998
�Action       :
�
�Notes :
����������������������������������������������������������������������������*/


int Flip_32(int N);
void Flip_32(void *P);

int Flip_16(short int N);
void Flip_16(void *P);

void Int2Str(int Number, char *String, int Length);
int Str2Int(const char *String, int &Number, char EndChar);
int Str2Float(const char *String, float &Number, char EndChar);
int SearchStr(const char *String, const char Letter);