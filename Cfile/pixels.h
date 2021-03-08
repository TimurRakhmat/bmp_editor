#pragma once
#include "main.h"
#define ost(x) ((x) ? 1 : 0)


BYTE** read_bmpbit(FILE**, int, int, int, int);


PPX24* read_bmpbyte(FILE**, int, int, int, int);


void save_bmpbit(FILE**, int, int, int, int, BYTE**);


void save_bmpbyte(FILE**, int, int, int, int, PPX24*);
