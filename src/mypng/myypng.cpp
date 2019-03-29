#include "myypng.h"
#include <malloc.h>

int myypng::writeImage(char * filename)
{
	int code = 1;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;

	png_bytep buff = buffer;

	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 0;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 0;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 0;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 0;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep)malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;

	for (y = 0; y<height; y++) {
		for (x = 0; x<width; x++) {
			row[x * 3 + 0] = buff[y*width * 3 + x * 3 + 0];
			row[x * 3 + 1] = buff[y*width * 3 + x * 3 + 1];
			row[x * 3 + 2] = buff[y*width * 3 + x * 3 + 2];
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}

float clamp(float val)
{
	val = val > 1 ? 1 : val;
	val = val < 0 ? 0 : val;
	return val;
}

png_byte clamp(png_byte val)
{
	val = val > 255 ? 255 : val;
	val = val < 0 ? 0 : val;
	return val;
}

void myypng::setImagePixel(int x, int y, Color color)
{
	png_byte r = clamp((png_byte)(clamp(color.r) * 255.f));
	png_byte g = clamp((png_byte)(clamp(color.g) * 255.f));
	png_byte b = clamp((png_byte)(clamp(color.b) * 255.f));

	y = height - 1 - y;

	png_bytep buff = buffer + 3 * (y * width + x);
	buff[0] = r;
	buff[1] = g;
	buff[2] = b;
}
