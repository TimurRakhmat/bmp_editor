#pragma once

#include<stdio.h>
#include<stdlib.h>
#include "string.h"
#include<math.h>


typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef long LONG;

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, * LPBITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER;


typedef struct tagBITMAPINFO {
	PBITMAPFILEHEADER bf;
	PBITMAPINFOHEADER bi;
} BITMAPINFO, * PBITMAPINFO;


typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserv;
} RGBQUAD;


typedef struct PIXEL24BIT {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
} PX24, * PPX24;