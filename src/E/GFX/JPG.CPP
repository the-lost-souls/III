/*
旼컴컴컴컴컴컴컴컴컴컴컴컴 WATCOM C++ SOURCE 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
쿙ame         : TGA.CPP
쿌uthor       : Cyberfish/TLS
쿗ast update  : 05.01.1999
쿌ction       : Loads/decodes a TGA-picture (type 2/10) into memory...
�
쿙otes :
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include <stdio.h>
#include <mem.h>
#include "w:\general\var.h"
#include "w:\pak\pak.h"
#include "w:\divlibs\jpeg\jpeglib2.h"
#include "w:\gfx\jpg.h"
#include "w:\divlibs\jpeg\jinclude.h"
#include "w:\divlibs\jpeg\jerror.h"
#include "w:\pak\pak.h"

int JPG_Width 	= 0;
int JPG_Heigth 	= 0;
int JPG_Bits 	= 0;

int JPEG_mem_src(j_decompress_ptr cinfo, const char *FileName);



void *JPG_Load(const char *FileName)
{
//---- Setting up jpeg-objects...
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);

	if ( !JPEG_mem_src(&cinfo, FileName) )
	{
		SYS_Error(1, "JPG_Load: Error initializing jpeg-source.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return 0;
	}//if


//---- Reading jpeg-header..,
	jpeg_read_header(&cinfo, TRUE);


//---- Starting decompression...
	jpeg_start_decompress(&cinfo);

	cinfo.output_width = cinfo.image_width;
	cinfo.output_height = cinfo.image_height;
	cinfo.out_color_components = 3;
	cinfo.output_components = 3;


//---- decompressing...
	JSAMPARRAY scanlines = new JSAMPROW[cinfo.output_height];
	if ( !scanlines )
	{
		SYS_Error(1, "JPG: Not enough memory to decompress picture.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
	}
	char *Picture = new char[cinfo.output_height*cinfo.output_width*3];
	if ( !Picture )
	{
		SYS_Error(1, "JPG: Not enough memory to decompress picture.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
	}
	memset(Picture, 0, cinfo.output_height*cinfo.output_width*3);

	for ( int C = 0; C < cinfo.output_height; C++ )
		scanlines[C] = &Picture[C*cinfo.output_width*3];

	C = 0;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		int k = jpeg_read_scanlines(&cinfo, &scanlines[C], cinfo.output_height);
		C += k;
	}//while

	jpeg_finish_decompress(&cinfo);

	JPG_Width = cinfo.output_width;
	JPG_Heigth = cinfo.output_height;
	JPG_Bits = 24;

	__asm{
		mov	ecx, JPG_Width
		imul	ecx, JPG_Heigth

		mov	esi, Picture
	JPG_FlipLoop:

		mov	al, [esi]
		mov	ah, [esi + 2]

		mov	[esi + 2], al
		mov	[esi], ah

		add	esi, 3
		dec	ecx
		jnz	JPG_FlipLoop
	}


//---- Destroy jpeg-objects...
	jpeg_destroy_decompress(&cinfo);

	return Picture;
}






typedef struct {
  struct jpeg_source_mgr pub;	/* public fields */

  byte 	*jpeg;
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;


void JPEG_init_source (j_decompress_ptr cinfo)
{
	//Don't need anything here... File is already loaded.

}


boolean JPEG_fill_input_buffer (j_decompress_ptr cinfo)
{
	ERREXIT(cinfo, JERR_INPUT_EMPTY);
	return FALSE;
}


void JPEG_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;

	src->pub.next_input_byte += (size_t) num_bytes;
	src->pub.bytes_in_buffer -= (size_t) num_bytes;
}


void JPEG_term_source (j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;

	delete src->jpeg;
	delete src;

}//term_source


int JPEG_mem_src(j_decompress_ptr cinfo, const char *FileName)
{
	if ( strcmp(FileName, "X:\\DEMO\\ROOM\\SCENES\\SHOT21.JPG") == 0 )
	{
		__asm int 3
	}

	my_src_ptr src;

	if (cinfo->src == NULL) {	/* first time for this JPEG object? */
		src = new my_source_mgr;
		if ( src == NULL )
		{
			SYS_Error(1, "JPEG_mem_src: Not enough memory to decompress jpeg.\n");
			SYS_Error(3, "  ( ", FileName, " )\n");
			return 0;
		}//if
		cinfo->src = (jpeg_source_mgr*)src;
	}//if

//---- Loading file...
	src->jpeg = (byte*)PAK_ReadFile(FileName);
	if ( !src->jpeg )
	{
		SYS_Error(1, "JPEG_mem_src: Error loading jpeg.\n");
		SYS_Error(3, "  ( ", FileName, " )\n");
		return 0;
	}//if

	src->pub.init_source = JPEG_init_source;
	src->pub.fill_input_buffer = JPEG_fill_input_buffer;
	src->pub.skip_input_data = JPEG_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = JPEG_term_source;

//---- Setting public data...
	src->pub.bytes_in_buffer = PAK_FileSize(FileName); /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = src->jpeg; /* until buffer loaded */

	return 1;
}
