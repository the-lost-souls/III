#include "w:\vectors\vec_file.h";
#include <stdio.h>


void main(void)
{
	if ( Aquarium() == NULL ) return;

	VEC_Header *VEC = VEC_Load((const char*)"w:\\test\\box_10.vec");
	if ( VEC == NULL )
	{
		printf("%s", SYS_QuitMsg_);
		printf("%s", SYS_QuitMsg);
		return;
	}


	delete VEC;
}
