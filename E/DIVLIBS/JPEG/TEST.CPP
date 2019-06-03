#include <stdio.h>
#include "w:\divlibs\jpeg\jpeglib2.h"


void main(void)
{

//---- Setting up jpeg-objects...
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

//---- Opening file...
	FILE * infile;
	if ((infile = fopen("x:\\maps\\g1.jpg", "rb")) == NULL) {
	    fprintf(stderr, "can't open file\n");
	}
	jpeg_stdio_src(&cinfo, infile);


//---- Reading jpeg-header..,
	jpeg_read_header(&cinfo, TRUE);


//---- Testing...
	printf("Picture dimensions: (%dx%d)\n", cinfo.image_width, cinfo.image_height);



//---- Starting decompression...
	jpeg_start_decompress(&cinfo);

	cinfo.output_width = cinfo.image_width;
	cinfo.output_height = cinfo.image_height;
	cinfo.out_color_components = 4;
	cinfo.output_components = 0;



//---- decompressing...
//	char *picture = new char[500*500*4];

	void **scanlineptrs = new void*[500];
	char *Picture = new char[500*500*4];

	for ( int C = 0; C < 500; C++ )
	{
		scanlineptrs[C] = &Picture[C*500*4];
	}

	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)scanlineptrs, 500);
		printf("scanline %d\n", cinfo.output_scanline);
	}//while


	jpeg_finish_decompress(&cinfo);



//---- Destroy jpeg-objects...
	jpeg_destroy_decompress(&cinfo);


}
