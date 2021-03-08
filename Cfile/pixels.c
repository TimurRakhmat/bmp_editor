#include "pixels.h"


//with color table
//read
BYTE** read_bmpbit(FILE** f, int seed, int y, int x, int bit)
{
    BYTE** px = NULL;
    px = (BYTE**)calloc(y, sizeof(BYTE*));
    int row = x / (8 / bit) + ost(x % (8 / bit));
    row = row + (4 - row % 4) % 4;
    for (int i = 0; i < y; ++i)
    {
        px[i] = (BYTE*)calloc(row, sizeof(BYTE));
    }
    fseek(f, seed, 0);

    for (int i = 0; i < y; ++i)
        for (int j = 0; j < row; ++j)
        {
            fread(&px[i][j], sizeof(BYTE), 1, f);
        }

    return px;
}


//save
void save_bmpbit(FILE** f, int seed, int y,\
    int x, int bit, BYTE** px)
{
    int row = x / (8 / bit) + ost(x % (8 / bit));
    row = row + (4 - row % 4) % 4;
    fseek(f, seed, 0);

    for (int i = 0; i < y; ++i)
        for (int j = 0; j < row; ++j)
        {
            fwrite(&px[i][j], sizeof(BYTE), 1, f);
        }
}


//without color table
// read
PPX24* read_bmp16bit(FILE** f, int seed, int y, int x)
{
    PPX24* px = NULL;
    WORD twoBYTE;
    px = (PPX24*)calloc(y, sizeof(PPX24));
    fseek(f, seed, 0);
    for (int i = 0; i < y; ++i)
    {
        px[i] = (PPX24)calloc(x, sizeof(PX24));
        for (int j = 0; j < x; ++j)
        {
            fread(&twoBYTE, sizeof(WORD), 1, f);
            px[i][j].rgbBlue = (twoBYTE & 0x001F) << 3;
            px[i][j].rgbGreen = (twoBYTE & 0x03E0) >> 2;
            px[i][j].rgbRed = (twoBYTE & 0x7C00) >> 7;
        }
        fseek(f, (2 * x) % 4, 1);
    }

    return px;
}


PPX24* read_bmp24bit(FILE** f, int seed, int y, int x)
{
    PPX24* px = NULL;
    px = (PPX24*)calloc(y, sizeof(PPX24));
    fseek(f, seed, 0);
    for (int i = 0; i < y; ++i)
    {
        px[i] = (PPX24)calloc(x, sizeof(PX24));
        for (int j = 0; j < x; ++j)
        {
            fread(&px[i][j].rgbBlue, sizeof(BYTE), 1, f);
            fread(&px[i][j].rgbGreen, sizeof(BYTE), 1, f);
            fread(&px[i][j].rgbRed, sizeof(BYTE), 1, f);
        }
        fseek(f, x % 4, 1);
    }

    return px;
}


PPX24* read_bmpbyte(FILE** f, int seed, int y, int x, int bit)
{
    if (bit == 24)
        return read_bmp24bit(f, seed, y, x);
    else
        return read_bmp16bit(f, seed, y, x);
}


// save
void save_bmp16bit(FILE** f, int seed, int y, int x, PPX24* px)
{
    WORD twoBYTE;
    fseek(f, seed, 0);
    for (int i = 0; i < y; ++i)
    {
        for (int j = 0; j < x; ++j)
        {
            twoBYTE = (px[i][j].rgbRed << 7 | px[i][j].rgbGreen << 2 \
                | px[i][j].rgbBlue >> 3);
            fwrite(&twoBYTE, sizeof(WORD), 1, f);
        }
        if ((2 * x) % 4)
            fwrite(&twoBYTE, sizeof(WORD), 1, f);
    }
}


void save_bmp24bit(FILE** f, int seed, int y, \
    int x, PPX24* px)
{
    fseek(f, seed, 0);
    BYTE zero[3] = { 0, 0, 0 };
    for (int i = 0; i < y; ++i)
    {
        for (int j = 0; j < x; ++j)
        {
            fwrite(&px[i][j].rgbBlue, sizeof(BYTE), 1, f);
            fwrite(&px[i][j].rgbGreen, sizeof(BYTE), 1, f);
            fwrite(&px[i][j].rgbRed, sizeof(BYTE), 1, f);
        }
        fwrite(zero, sizeof(BYTE), x % 4, f);
    }
}


void save_bmpbyte(FILE** f, int seed, int y, \
    int x, int bit, PPX24* px)
{
    if (bit == 24)
        save_bmp24bit(f, seed, y, x, px);
    else
        save_bmp16bit(f, seed, y, x, px);
}
