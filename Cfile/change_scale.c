#include "change_scale.h"
#pragma warning(disable:4996)


PPX24* scalebyte(PBITMAPINFO bm, PPX24* px)
{
	int width = bm->bi->biWidth, height = bm->bi->biHeight;
	int nx, ny;
	int flag = 0;
	char answer[128];

	printf("your image scale: width(%d), height(%d)\n", width, height);
	printf("new width: ");
	while (!flag)
	{
		flag = scanf("%d", &nx);
		if (!flag)
		{
			scanf("%s", answer);
			printf("retry: ");
		}
		else if (nx < 1 || nx > 1980)
		{
			printf("use width in [1, 1980]: ");
			flag = 0;
		}
	}
	flag = 0;
	printf("new height: ");
	while (!flag)
	{
		flag = scanf("%d", &ny);
		if (!flag)
		{
			scanf("%s", answer);
			printf("retry: ");
		}
		else if (ny < 1 || ny > 1080)
		{
			printf("use height in [1, 1980]: ");
			flag = 0;
		}
	}

	float scalex = (float)width / (float)nx;
	float scaley = (float)height / (float)ny;
	PPX24* npx = (PPX24*)calloc(ny, sizeof(PPX24));

	for (int i = 0; i < ny; ++i)
	{
		npx[i] = (PPX24)calloc(nx, sizeof(PX24));
		for (int j = 0; j < nx; ++j)
		{
			npx[i][j].rgbBlue = px[(int)(i * scaley)]\
				[(int)(j * scalex)].rgbBlue;
			npx[i][j].rgbGreen = px[(int)(i * scaley)]\
				[(int)(j * scalex)].rgbGreen;
			npx[i][j].rgbRed = px[(int)(i * scaley)]\
				[(int)(j * scalex)].rgbRed;
		}
	}

	for (int i = 0; i < height; ++i)
		free(px[i]);
	free(px);
	bm->bi->biWidth = nx, bm->bi->biHeight = ny;
	bm->bf->bfSize = 54 + \
		(nx + ((bm->bi->biBitCount / 8) * nx) % 4) \
		* ny * (bm->bi->biBitCount / 8);
	bm->bi->biSizeImage = bm->bf->bfSize - 54;
	return npx;
}


BYTE** scalebit(PBITMAPINFO bm, BYTE** px)
{
	int width = bm->bi->biWidth, height = bm->bi->biHeight;
	int nx, ny;
	int bit = bm->bi->biBitCount;
	int flag = 0;
	char answer[128];

	printf("your image scale: width(%d), height(%d)\n", width, height);
	printf("new width: ");
	while (!flag)
	{
		flag = scanf("%d", &nx);
		if (!flag)
		{
			scanf("%s", answer);
			printf("retry: ");
		}
		else if (nx < 1 || nx > 1980)
		{
			printf("use width in [1, 1980]: ");
			flag = 0;
		}
	}
	flag = 0;
	printf("new height: ");
	while (!flag)
	{
		flag = scanf("%d", &ny);
		if (!flag)
		{
			scanf("%s", answer);
			printf("retry: ");
		}
		else if (ny < 1 || ny > 1080)
		{
			printf("use height in [1, 1080]: ");
			flag = 0;
		}
	}

	float scalex = (float)width / (float)nx;
	float scaley = (float)height / (float)ny;
	int row = nx / (8 / bit) + ost(nx % (8 / bit));
	row = row + (4 - row % 4) % 4;
	BYTE** npx = (BYTE**)calloc(ny, sizeof(BYTE*));
	int buf;
	int shift;
	int equat;

	for (int i = 0; i < ny; ++i)
	{
		npx[i] = (BYTE*)calloc(row, sizeof(BYTE));
		for (int j = 0; j < nx; ++j)
		{
			shift = (((1 << (bit)) - 1) << \
				(bit * ((8 / bit - 1) - j % (8 / bit))));
			equat = px[(int)(i * scaley)][(int)(j * scalex / (8 / bit))];
			buf = equat & shift;
			npx[i][j / (8 / bit)] += buf;
		}
		
	}

	for (int i = 0; i < height; ++i)
		free(px[i]);
	free(px);
	bm->bi->biWidth = nx, bm->bi->biHeight = ny;
	bm->bf->bfSize = 54 + 4 * (2 << (bit - 1)) + row * ny;
	bm->bi->biSizeImage = row * ny;
	return npx;
}
