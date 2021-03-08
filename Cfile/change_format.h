#pragma once
#include "main.h"
#define ost(x) ((x) ? 1 : 0)


typedef struct PIXELS {
	PPX24* pxbyte;
	BYTE** pxbit;
	RGBQUAD** color;
} PX;

int change_format(BYTE**, PPX24*, PBITMAPINFO, RGBQUAD**, PX*);
