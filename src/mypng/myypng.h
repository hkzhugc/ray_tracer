#pragma once
#include "png/png.h"
#include "../math/color.h"
class myypng
{
public:
	myypng() { buffer = NULL; }
	myypng(int _width, int _height) : 
		width(_width), height(_height)
	{
		buffer = new png_byte[width * height * 3];
	}
	~myypng() 
	{
		if (buffer)
			delete buffer;
		buffer = NULL;
	}

	void init_png(int _width, int _height)
	{
		width = _width;
		height = _height;
		buffer = new png_byte[width * height * 3];
	}
	int writeImage(const char* filename);
	void setImagePixel(int x, int y, Color color);
private:
	png_bytep buffer;
	int width, height;
};

