/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ INCLUDEFILE 컴컴컴컴컴컴컴컴컴컴컴컴컴
쿙ame         : PAK.H
쿌uthor       : Cyberfish/TLS
쿗ast update  : 23.05.1998
쿌ction       : Definitions for PAK-system.
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#ifndef PAK_included
#define PAK_included

#include "w:\general\var.h"
#include <stdio.h>

const int PAK_FILE_ID = 'FKAP';	//chunk-ID for PAK fileheader




class PAK_FILE_Header {		//Structure for the PAK-file-header.
public:
	PAK_FILE_Header();

	char    IDString[256];  //ID-string. Should be ended with EOF (1Ah)
	int 	ID;		//Chunk-ID
	int	FileSize;	//Total size of file
	int	NrFiles;	//Number of files in PAK
	uint32	MapChecksum;	//Checksum of compressed filemap
	int	MapOffset;	//Fileoffset to filemap

private:
};






typedef struct {				//Structure for the PAK-file-blockinfo
	char    FileName[SYS_FileNameLength];   //Original filename (uppercase)
	int	Length_Raw;			//Original filesize
	int	Length_Compressed;		//Compressed filesize
	int	Checksum;			//Checksum of compressed block
	int	Offset;				//Offset in PAK
}PAK_FILE_BlockInfo;







class PAK_BlockInfo {				//Structure for the PAK-blockinfo (memory)
public:
	PAK_BlockInfo();

	char    FileName[SYS_FileNameLength];   //Original filename (uppercase)
	int	Length_Raw;			//Original filesize
	int	Length_Compressed;		//Compressed filesize
	int	Checksum;			//Checksum of compressed block
	int	Offset;				//Offset in PAK

	PAK_BlockInfo *NextBlock;
private:

};






class PAK_FileInfo {				//Structure for an open PAK-file (memory)

public:
	PAK_FileInfo();				//constructor
	~PAK_FileInfo();			//destructor

	FILE   *PAKFile;			//File
	char    FileName[SYS_FileNameLength];  	//Filename of PAK
	int	NrFiles;			//Number of files in PAK
	PAK_BlockInfo	*FirstBlock;    	//Offset to first block
	PAK_BlockInfo   *LastBlock;		//Offset to last block

	PAK_FileInfo	*NextPAK;		//Offset to next PAK
private:
};//PAK_FileInfo;









void *PAK_Encode( byte *Data,				//Encodes data (zlib)
		  int RawLength,
		  int &CompressedLength);


void *PAK_Decode( byte *Data,				//Decodes data (zlib)
		  int &RawLength,
		  int CompressedLength);


PAK_FileInfo *PAK_Create( const char *FileName,		//Creates a new PAK-file
			  const char *IDString);



int PAK_AddFile( PAK_FileInfo *PAK,			//Adds a file to a PAK
		 const char *FileName);



PAK_FileInfo *PAK_Open( const char *FileName); 		//Opens a PAK-file




PAK_FILE_Header *PAK_Verify( const char *FileName);	//Verifies a PAK-file




void *PAK_ReadFile( const char *FileName );		//Reads a file




int PAK_FileSize( const char *FileName);		//Gets filesize

#endif  //ifdef PAK_included

