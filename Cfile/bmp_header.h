#pragma once
#include"main.h"


void save_bmp_head(FILE*, PBITMAPINFO);


RGBQUAD** read_color(FILE*, int);


void write_color(FILE*, RGBQUAD**, int);


PBITMAPINFO read_bmp_head(FILE*);
