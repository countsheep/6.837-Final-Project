#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

// #pragma pack(2) // found this as a 2 byte alignment... no idea what this is

// bitmap header
struct bmpFileHeader
{
	unsigned short bfType; // file type
	unsigned int bfSize; // size in bytes of bmp
	unsigned short bfReserved1; // still don't fully understand this
	unsigned short bfReserved2; // or this...
	unsigned int bfOffBits; // or this... depends on color palette?
};

// bitmap information specific to image data
struct bmpInfoHeader
{
	unsigned int biSize; // number of bytes required by InfoHeader
	int biWidth; // width in pixels
	int biHeight; // height in pixels
	unsigned short biPlanes; // number of color (bit) planes (?)
	unsigned short biBitCount; // number of bits per pixel
	unsigned int biCompression; // type of compression
	unsigned int biSizeImage; // size of image in bytes
	int biXPixelsPerMeter; // pixels per meter in x axis
	int biYPixelsPerMeter; // pixels per meter in y axis
	unsigned int biColorsUsed; // number of colors used by the bitmap
	unsigned int biColorsImportant; // number of colors that are important
};

// color palette
// I found unsigned char to be equivalent to byte
struct RGBQUAD
{
	unsigned char rgbRed;
	unsigned char rgbGreen;
	unsigned char rgbBlue;
	unsigned char rgbReserved; // not sure about this one
};

class bitMap
{
public:
	bitMap();
	~bitMap();
	void loadBitMap(char *fileName); // loads the bmp file

private:
	bmpFileHeader bmfh;
	bmpInfoHeader bmih;
};

// #pragma pack() 