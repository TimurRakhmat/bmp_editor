#pragma once
#include "main.h"
#define min(x) ((x) > 255 ? 255 : x)
#define max(x) ((x) <   0 ?   0 : x)

void filter8bit(PBITMAPINFOHEADER, BYTE**, RGBQUAD**);


void filter(PBITMAPINFOHEADER, PPX24*);


void bloor(PBITMAPINFOHEADER, PPX24*);

void median(PBITMAPINFOHEADER, PPX24*);

void sharp(PBITMAPINFOHEADER, PPX24*);

void bloor8(PBITMAPINFOHEADER, BYTE**, RGBQUAD**);

void sharp8(PBITMAPINFOHEADER, BYTE**, RGBQUAD**);

void median8(PBITMAPINFOHEADER, BYTE**, RGBQUAD**);
