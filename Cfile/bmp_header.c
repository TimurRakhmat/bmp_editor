#include"bmp_header.h"


PBITMAPINFO read_bmp_head(FILE* f)
{
	//read bmp info header and return pointer to struct PBITMAPINFO

	int s1 = sizeof(WORD);
	int s2 = sizeof(DWORD);
	int s3 = sizeof(LONG);

	PBITMAPINFO bm = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFO));
	PBITMAPFILEHEADER bf = (PBITMAPFILEHEADER)calloc(1, \
		sizeof(BITMAPFILEHEADER));
	PBITMAPINFOHEADER bi = (PBITMAPINFOHEADER)calloc(1, \
		sizeof(BITMAPINFOHEADER));

	// read fileheader
	fread(&bf->bfType, s1, 1, f);
	if (bf->bfType != 0x4D42) //bmp format
	{
		printf("\nERROR - file not in bmp format\n", f);
		free(bf);
		free(bi);
		free(bm);
		fclose(f);
		exit(1);
	}
	fread(&bf->bfSize, s2, 1, f);
	fread(&bf->bfReserved1, s1, 1, f);
	fread(&bf->bfReserved2, s1, 1, f);
	fread(&bf->bfOffBits, s2, 1, f);

	// read infoheader
	fread(&bi->biSize, s2, 1, f);
	fread(&bi->biWidth, s3, 1, f);
	fread(&bi->biHeight, s3, 1, f);
	fread(&bi->biPlanes, s1, 1, f);
	fread(&bi->biBitCount, s1, 1, f);
	if (bi->biBitCount > 24)
	{
		printf("\nsorry this program support only bmp24 \
format and lower\n");
		free(bf);
		free(bi);
		free(bm);
		fclose(f);
		exit(1);
	}
	fread(&bi->biCompression, s2, 1, f);
	fread(&bi->biSizeImage, s2, 1, f);
	fread(&bi->biXPelsPerMeter, s3, 1, f);
	fread(&bi->biYPelsPerMeter, s3, 1, f);
	fread(&bi->biClrUsed, s2, 1, f);
	fread(&bi->biClrImportant, s2, 1, f);

	bm->bf = bf;
	bm->bi = bi;

	return bm;
}


RGBQUAD** read_color(FILE* f, int bit)
{
	RGBQUAD** color = (RGBQUAD**)calloc(2 << (bit - 1), sizeof(RGBQUAD*));
	for (int i = 0; i < 2 << (bit - 1); ++i)
	{
		color[i] = (RGBQUAD*)calloc(1, sizeof(RGBQUAD));
	}

	for (int i = 0; i < 2 << (bit - 1); ++i)
	{
		fread(&color[i]->rgbBlue, sizeof(BYTE), 1, f);
		fread(&color[i]->rgbGreen, sizeof(BYTE), 1, f);
		fread(&color[i]->rgbRed, sizeof(BYTE), 1, f);
		fread(&color[i]->rgbReserv, sizeof(BYTE), 1, f);
	}

	return color;
}


void write_color(FILE* f, RGBQUAD** color, int bit)
{
	for (int i = 0; i < 2 << (bit - 1); ++i)
	{
		fwrite(&color[i]->rgbBlue, sizeof(BYTE), 1, f);
		fwrite(&color[i]->rgbGreen, sizeof(BYTE), 1, f);
		fwrite(&color[i]->rgbRed, sizeof(BYTE), 1, f);
		fwrite(&color[i]->rgbReserv, sizeof(BYTE), 1, f);
		free(color[i]);
	}
	free(color);
}


void save_bmp_head(FILE* f, PBITMAPINFO bm)
{
	//write bmp info header to file

	int s1 = sizeof(WORD);
	int s2 = sizeof(DWORD);
	int s3 = sizeof(LONG);

	PBITMAPFILEHEADER bf = bm->bf;
	PBITMAPINFOHEADER bi = bm->bi;

	// write fileheader
	fwrite(&bf->bfType, s1, 1, f);
	fwrite(&bf->bfSize, s2, 1, f);
	fwrite(&bf->bfReserved1, s1, 1, f);
	fwrite(&bf->bfReserved2, s1, 1, f);
	fwrite(&bf->bfOffBits, s2, 1, f);

	// write infoheader
	fwrite(&bi->biSize, s2, 1, f);
	fwrite(&bi->biWidth, s3, 1, f);
	fwrite(&bi->biHeight, s3, 1, f);
	fwrite(&bi->biPlanes, s1, 1, f);
	fwrite(&bi->biBitCount, s1, 1, f);
	fwrite(&bi->biCompression, s2, 1, f);
	fwrite(&bi->biSizeImage, s2, 1, f);
	fwrite(&bi->biXPelsPerMeter, s3, 1, f);
	fwrite(&bi->biYPelsPerMeter, s3, 1, f);
	fwrite(&bi->biClrUsed, s2, 1, f);
	fwrite(&bi->biClrImportant, s2, 1, f);
}
