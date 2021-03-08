#include "change_format.h"
#include "bmp_header.h"
#pragma warning(disable:4996)


void byte_to_byte(PBITMAPINFO, int);
void bit_to_byte(PBITMAPINFO bm, PPX24* px, int nbit,\
	BYTE** bt, RGBQUAD** colors, PX*);
int byte_to_bit(PBITMAPINFO bm, int nbit, PX* px);
int bit_to_bit(PBITMAPINFO bm, int nbit , PX* px);


int change_format(BYTE** pxbit, PPX24* pxbyte,\
	PBITMAPINFO bm, RGBQUAD** color, PX* px)
{
	int bit = bm->bi->biBitCount;
	int tobit = 0;
	char answer[128];

	while (1)
	{
		printf("\nyou can save in format:\n");
		printf("24 - bmp24\n");
		printf("16 - bmp16\n");
		printf("8 - bmp8\n");
		printf("1 - bmp1\n");
		printf("0 - return\n");

		scanf("%s", answer);
		if (!strcmp(answer, "1"))
		{
			tobit = 1;
			break;
		}
		else if (!strcmp(answer, "8"))
		{
			tobit = 8;
			break;
		}
		else if (!strcmp(answer, "16"))
		{
			tobit = 16;
			break;
		}
		else if (!strcmp(answer, "24"))
		{
			tobit = 24;
			break;
		}
		else if (!strcmp(answer, "0"))
		{
			return bit;
		}
		else
			printf("pls try again\n");
	}

	if (bit != tobit || (bit < 16 && tobit < 16))
	{
		if (bit > 8 && tobit > 8)
			byte_to_byte(bm, tobit);
		else if (tobit > 8 && bit < 16)
			bit_to_byte(bm, pxbyte, tobit, pxbit, color, px);
		else if (tobit < 16 && bit > 8)
			tobit = byte_to_bit(bm, tobit, px);
		else
			tobit = bit_to_bit(bm, tobit, px);
		bit = tobit;
	}

	return bit;
}


void byte_to_byte(PBITMAPINFO bm, int nbit)
{
	bm->bf->bfSize = 54 + (bm->bi->biWidth + ((nbit / 8) * bm->bi->biWidth) % 4) * bm->bi->biHeight * (nbit / 8);
	bm->bi->biSizeImage = bm->bf->bfSize - 54;
	bm->bi->biBitCount = nbit;
}


void bit_to_byte(PBITMAPINFO bm, PPX24* px, \
	int nbit, BYTE** bt, RGBQUAD** colors, PX* st)
{
	int bit = bm->bi->biBitCount;
	RGBQUAD *color;
	px = (PPX24*)calloc(bm->bi->biHeight, sizeof(PPX24));
	for (int i = 0; i < bm->bi->biHeight; ++i)
		px[i] = (PPX24)calloc(bm->bi->biWidth, sizeof(PX24));

	for (int i = 0; i < bm->bi->biHeight; ++i)
	{
		for (int j = 0; j < bm->bi->biWidth; ++j)
		{
			color = colors[(bt[i][j / (8 / bit)] & \
				(((1 << (bit)) - 1) << ((8 / bit - 1) - j % (8 / bit))))\
				>> ((8 / bit - 1) - j % (8 / bit))];
			px[i][j].rgbBlue = color->rgbBlue;
			px[i][j].rgbGreen = color->rgbGreen;
			px[i][j].rgbRed = color->rgbRed;
		}
		free(bt[i]);
	}

	free(bt);
	for (int i = 0; i < 1 << bit; ++i)
		free(colors[i]);
	free(colors);
	bm->bf->bfOffBits = 54;
	bm->bf->bfSize = 54 + (bm->bi->biWidth + \
		((nbit / 8) * bm->bi->biWidth) % 4) *\
		bm->bi->biHeight * (nbit / 8);
	bm->bi->biSizeImage = bm->bf->bfSize - 54;
	bm->bi->biBitCount = nbit;

	st->pxbyte = px;
	st->pxbit = NULL;
	st->color = NULL;
}


int get_color_component()
{
	char answer[128];
	int color_int;
	int flag = 0;

	while (!flag)
	{
		flag = scanf("%d", &color_int);
		if (!flag)
		{
			scanf("%s", answer);
			printf("retry: ");
		}
		else if (color_int < 0 || color_int > 255)
		{
			printf("use color in [0, 255]: ");
			flag = 0;
		}
	}

	return color_int;
}


void set_gray(RGBQUAD** color)
{
	int gray_value = 0;
	for (int i = 0; i < 256; ++i)
	{
		gray_value = (color[i]->rgbBlue + color[i]->rgbGreen + color[i]->rgbRed) / 3;
		color[i]->rgbBlue = gray_value;
		color[i]->rgbGreen = gray_value;
		color[i]->rgbRed = gray_value;
	}
}


void set_invers(RGBQUAD** color)
{
	for (int i = 0; i < 256; ++i)
	{
		color[i]->rgbBlue = 255 - color[i]->rgbBlue;
		color[i]->rgbGreen = 255 - color[i]->rgbGreen;
		color[i]->rgbRed = 255 - color[i]->rgbRed;
	}
}


void set_one_color(RGBQUAD** color, int shift)
{
	for (int i = 0; i < 256; ++i)
	{
		if (shift == 0)
		{
			color[i]->rgbBlue = 0;
			color[i]->rgbGreen = 0;
		}
		else if (shift == 1)
		{
			color[i]->rgbBlue = 0;
			color[i]->rgbRed = 0;
		}
		if (shift == 2)
		{
			color[i]->rgbRed = 0;
			color[i]->rgbGreen = 0;
		}
	}
}


int set_color_table_from_byte(PX* px,\
	PBITMAPINFO bm, int nbit)
{
	char answer[128];
	FILE* color_file = fopen("color_table.col", "rb");
	if (!color_file)
	{
		printf("ERROR table file 'color_table.col' not found\n");
		return -1;
	}

	//write_regular_color
	px->color = read_color(color_file, 8);

	//make memmory
	px->pxbit = (BYTE**)calloc(bm->bi->biHeight, sizeof(BYTE*));
	int row = bm->bi->biWidth;
	row = row + (4 - row % 4) % 4;
	for (int i = 0; i < bm->bi->biHeight; ++i)
	{
		px->pxbit[i] = (BYTE*)calloc(row, sizeof(BYTE));
	}


	//convert color to palette
	//clear byte array and save bit array
	int rt = 255;
	float min_coef = 99999;
	float coef;
	int bl, gr, rd;
	int blb, grb, rdb;
	for (int i = 0; i < bm->bi->biHeight; ++i)
	{
		for (int j = 0; j < bm->bi->biWidth; ++j)
		{
			min_coef = 99999;
			blb = px->pxbyte[i][j].rgbBlue;
			grb = px->pxbyte[i][j].rgbGreen;
			rdb = px->pxbyte[i][j].rgbRed;
			for (int k = 0; k < 256; k++)
			{
				bl = px->color[k]->rgbBlue;
				gr = px->color[k]->rgbGreen;
				rd = px->color[k]->rgbRed;
				coef = 11 * (bl - blb) * (bl - blb) + \
					59 * (gr - grb) * (gr - grb) + \
					30 * (rd - rdb) * (rd - rdb);
				if (coef < min_coef)
				{
					min_coef = coef;
					rt = k;
				}
			}
			px->pxbit[i][j] = rt;
		}
		free(px->pxbyte[i]);
	}
	free(px->pxbyte);


	//change bm_header
	bm->bf->bfOffBits = 54 + 1024;
	bm->bf->bfSize = row * bm->bi->biHeight + 54 + 1024;
	bm->bi->biBitCount = 8;
	bm->bi->biSizeImage = row * bm->bi->biHeight;


	//choose palette
	printf("\nchoose color palette:\n");
	printf("1 - regular palette\n");
	printf("2 - gray palette\n");
	printf("3 - inversion\n");
	printf("4 - only red palette\n");
	printf("5 - only green palette\n");
	printf("6 - only blue palette\n");
	while(1)
	{
		printf("choose: ");
		scanf("%s", answer);
		if (!strcmp(answer, "1"))
		{
			break;
		}
		else if (!strcmp(answer, "2"))
		{
			set_gray(px->color);
			break;
		}
		else if (!strcmp(answer, "3"))
		{
			set_invers(px->color);
			break;
		}
		else if (!strcmp(answer, "4"))
		{
			set_one_color(px->color, 0);
			break;
		}
		else if (!strcmp(answer, "5"))
		{
			set_one_color(px->color, 1);
			break;
		}
		else if (!strcmp(answer, "6"))
		{
			set_one_color(px->color, 2);
			break;
		}
		else
			printf("pls try again/n");
	}

	return 0;
}


int byte_to_bit(PBITMAPINFO bm, int nbit, PX* px)
{
	char answer[128];
	int color_int;
	int flag = 0;
	if (nbit == 1)
	{
		//mem for color table
		px->color = (RGBQUAD**)calloc(1 << nbit, sizeof(RGBQUAD*));
		px->color[0] = (RGBQUAD*)calloc(1, sizeof(RGBQUAD));
		px->color[1] = (RGBQUAD*)calloc(1, sizeof(RGBQUAD));


		//ask to two color
		printf("write the first color\n");
		printf("red value: ");
		px->color[0]->rgbRed = get_color_component();
		printf("green value: ");
		px->color[0]->rgbGreen = get_color_component();
		printf("blue value: ");
		px->color[0]->rgbBlue = get_color_component();

		printf("write the second color\n");
		printf("red value: ");
		px->color[1]->rgbRed = get_color_component();
		printf("green value: ");
		px->color[1]->rgbGreen = get_color_component();
		printf("blue value: ");
		px->color[1]->rgbBlue = get_color_component();


		//mem to bit array
		px->pxbit = (BYTE**)calloc(bm->bi->biHeight, sizeof(BYTE*));
		int row = bm->bi->biWidth / (8 / nbit) + ost(bm->bi->biWidth % (8 / nbit));
		row = row + (4 - row % 4) % 4;
		for (int i = 0; i < bm->bi->biHeight; ++i)
		{
			px->pxbit[i] = (BYTE*)calloc(row, sizeof(BYTE));
		}


		//set pixel
		float bright;
		BYTE val;
		for (int i = 0; i < bm->bi->biHeight; ++i)
		{
			for (int j = 0; j < bm->bi->biWidth; ++j)
			{
				bright = px->pxbyte[i][j].rgbBlue * 0.08 + \
					px->pxbyte[i][j].rgbGreen * 0.71 + \
					px->pxbyte[i][j].rgbRed * 0.21;
				if (bright > 127)
					val = 1;
				else
					val = 0;
				px->pxbit[i][j / 8] += val << ( 7 - j % 8);
			}
			free(px->pxbyte[i]);
		}
		free(px->pxbyte);


		//change bm_header
		bm->bf->bfOffBits = 62;
		bm->bf->bfSize = row * bm->bi->biHeight + 62;
		bm->bi->biBitCount = 1;
		bm->bi->biSizeImage = row * bm->bi->biHeight;

		return 1;
	}
	else
	{
		if (set_color_table_from_byte(px, bm, nbit))
			return(bm->bi->biBitCount);

		return 8;
	}
}


int set_color_table_from_bit(PX* px,\
	PBITMAPINFO bm, int nbit)
{
	char answer[128];
	int bit = bm->bi->biBitCount;
	FILE* color_file = fopen("color_table.col", "rb");
	if (!color_file)
	{
		printf("ERROR table file 'color_table.col' not found\n");
		return -1;
	}

	//write_regular_color
	RGBQUAD** color;
	color = read_color(color_file, 8);

	//make memmory
	BYTE** pxbit = (BYTE**)calloc(bm->bi->biHeight, sizeof(BYTE*));
	int row = bm->bi->biWidth;
	row = row + (4 - row % 4) % 4;
	for (int i = 0; i < bm->bi->biHeight; ++i)
	{
		pxbit[i] = (BYTE*)calloc(row, sizeof(BYTE));
	}


	//convert color to palette
	//clear byte array and save bit array
	int rt = 255;
	float min_coef = 99999;
	float coef;
	int bl, gr, rd;
	int blb, grb, rdb;
	RGBQUAD* pcolor;
	for (int i = 0; i < bm->bi->biHeight; ++i)
	{
		for (int j = 0; j < bm->bi->biWidth; ++j)
		{
			min_coef = 99999;
			pcolor = px->color[(px->pxbit[i][j / (8 / bit)] & \
				(((1 << (bit)) - 1) << ((8 / bit - 1) - j % (8 / bit)))) \
				>> ((8 / bit - 1) - j % (8 / bit))];
			blb = pcolor->rgbBlue, grb = pcolor->rgbGreen, rdb = pcolor->rgbRed;
			for (int k = 0; k < 256; k++)
			{
				bl = color[k]->rgbBlue, gr = color[k]->rgbGreen, rd = color[k]->rgbRed;
				coef = 11 * (bl - blb) * (bl - blb) + 59 * (gr - grb) * \
					(gr - grb) + 30 * (rd - rdb) * (rd - rdb);
				if (coef < min_coef)
				{
					min_coef = coef;
					rt = k;
				}
			}
			pxbit[i][j] = rt;
		}
		free(px->pxbit[i]);
	}
	free(px->pxbit);
	for (int i = 0; i < 1 << bit; ++i)
		free(px->color[i]);
	free(px->color);
	px->color = color;
	px->pxbit = pxbit;


	//change bm_header
	bm->bf->bfOffBits = 54 + 1024;
	bm->bf->bfSize = row * bm->bi->biHeight + 54 + 1024;
	bm->bi->biBitCount = 8;
	bm->bi->biSizeImage = row * bm->bi->biHeight;
	

	//choose palette
	printf("\nchoose color palette:\n");
	printf("1 - regular palette\n");
	printf("2 - gray palette\n");
	printf("3 - inversion\n");
	printf("4 - only red palette\n");
	printf("5 - only green palette\n");
	printf("6 - only blue palette\n");
	while (1)
	{
		printf("choose: ");
		scanf("%s", answer);
		if (!strcmp(answer, "1"))
		{
			break;
		}
		else if (!strcmp(answer, "2"))
		{
			set_gray(px->color);
			break;
		}
		else if (!strcmp(answer, "3"))
		{
			set_invers(px->color);
			break;
		}
		else if (!strcmp(answer, "4"))
		{
			set_one_color(px->color, 0);
			break;
		}
		else if (!strcmp(answer, "5"))
		{
			set_one_color(px->color, 1);
			break;
		}
		else if (!strcmp(answer, "6"))
		{
			set_one_color(px->color, 2);
			break;
		}
		else
			printf("pls try again/n");
	}

	return 0;
}


int bit_to_bit(PBITMAPINFO bm, int nbit, PX* px)
{
	char answer[128];
	int flag = 0;
	if (nbit == 1)
	{
		//mem for color table
		RGBQUAD** color = (RGBQUAD**)calloc(1 << nbit, sizeof(RGBQUAD*));
		RGBQUAD* buf_color;
		color[0] = (RGBQUAD*)calloc(1, sizeof(RGBQUAD));
		color[1] = (RGBQUAD*)calloc(1, sizeof(RGBQUAD));


		//ask to two color
		printf("write the first color\n");
		printf("red value: ");
		color[0]->rgbRed = get_color_component();
		printf("green value: ");
		color[0]->rgbGreen = get_color_component();
		printf("blue value: ");
		color[0]->rgbBlue = get_color_component();

		printf("write the second color\n");
		printf("red value: ");
		color[1]->rgbRed = get_color_component();
		printf("green value: ");
		color[1]->rgbGreen = get_color_component();
		printf("blue value: ");
		color[1]->rgbBlue = get_color_component();


		//mem to bit array
		BYTE** pxbit = (BYTE**)calloc(bm->bi->biHeight, sizeof(BYTE*));
		int row = bm->bi->biWidth / (8 / nbit) + ost(bm->bi->biWidth % (8 / nbit));
		row = row + (4 - row % 4) % 4;
		for (int i = 0; i < bm->bi->biHeight; ++i)
		{
			pxbit[i] = (BYTE*)calloc(row, sizeof(BYTE));
		}


		//set pixel
		float bright;
		BYTE val;
		int bit = bm->bi->biBitCount;
		for (int i = 0; i < bm->bi->biHeight; ++i)
		{
			for (int j = 0; j < bm->bi->biWidth; ++j)
			{
				buf_color = px->color[(px->pxbit[i][j / (8 / bit)] & \
					(((1 << (bit)) - 1) << ((8 / bit - 1) - j % (8 / bit)))) \
					>> ((8 / bit - 1) - j % (8 / bit))];
				bright = buf_color->rgbBlue * 0.08 + \
					     buf_color->rgbGreen * 0.71 + \
					     buf_color->rgbRed * 0.21;
				if (bright > 127)
					val = 1;
				else
					val = 0;
				pxbit[i][j / 8] += val << (7 - j % 8);
			}
			free(px->pxbit[i]);
		}
		free(px->pxbit);

		for (int i = 0; i < 1 << bit; ++i)
			free(px->color[i]);
		free(px->color);

		//change bm_header
		bm->bf->bfOffBits = 62;
		bm->bf->bfSize = row * bm->bi->biHeight + 62;
		bm->bi->biBitCount = 1;
		bm->bi->biSizeImage = row * bm->bi->biHeight;

		px->color = color;
		px->pxbit = pxbit;

		return 1;
	}
	else
	{
		if (set_color_table_from_bit(px, bm, nbit))
			return(bm->bi->biBitCount);

		return 8;
	}
}
