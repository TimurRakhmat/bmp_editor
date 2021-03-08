#include "filter_commander.h"
#pragma warning(disable:4996)


//filter menu
void filter(PBITMAPINFOHEADER bi, PPX24* px)
{
	char answer[128];

	while (1)
	{
		printf("\nchose filters:\n");
		printf("1 - bloor\n");
		printf("2 - median filter\n");
		printf("3 - sharpness\n");
		printf("0 - return\n");
		scanf("%s", answer);

		if (!strcmp(answer, "1"))
		{
			printf("loading...\n");
			bloor(bi, px);
			bloor(bi, px);
		}
		else if (!strcmp(answer, "2"))
		{
			printf("loading...\n");
			median(bi, px);
		}
		else if (!strcmp(answer, "3"))
		{
			printf("loading...\n");
			sharp(bi, px);
		}
		else if (!strcmp(answer, "0"))
			return;
		else
			printf("pls try again\n");
	}
}


//menu for 8bit
void filter8bit(PBITMAPINFOHEADER bi, BYTE** px, \
	RGBQUAD** color)
{
	char answer[128];

	while (1)
	{
		printf("\nchose filters:\n");
		printf("1 - bloor\n");
		printf("2 - median filter\n");
		printf("3 - sharpness\n");
		printf("0 - return\n");
		scanf("%s", answer);

		if (!strcmp(answer, "1"))
		{
			printf("loading...\n");
			bloor8(bi, px, color);
		}
		else if (!strcmp(answer, "2"))
		{
			printf("loading...\n");
			median8(bi, px, color);
		}
		else if (!strcmp(answer, "3"))
		{
			printf("loading...\n");
			sharp8(bi, px, color);
		}
		else if (!strcmp(answer, "0"))
			return;
		else
			printf("pls try again\n");
	}
}


//filters

//for bytes bmp
void bloor(PBITMAPINFOHEADER bi, PPX24* px)
{
	PPX24* buf = NULL;
	buf = (PPX24*)calloc(bi->biHeight, sizeof(PPX24));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (PPX24)calloc(bi->biWidth, sizeof(PX24));
	}
	int color[3];
	int change;

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			change = 0;
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;

			for (int y = -2; y < 3; y++)
			{
				//check if the row exists
				if (y + i > -1 && y + i < bi->biHeight)
				{
					for (int x = -2; x < 3; x++)
					{
						//check if the col exists
						if (x + j > -1 && x + \
							j < bi->biWidth)
						{
							color[0] += px[i + y]\
								[j + x].rgbBlue;
							color[1] += px[i + y]\
								[j + x].rgbGreen;
							color[2] += px[i + y]\
								[j + x].rgbRed;
							change++;
						}
					}
				}
			}

			//if change is 0 then no pixels were found
			if (change != 0)
			{
				buf[i][j].rgbBlue = color[0] / change;
				buf[i][j].rgbGreen = color[1] / change;
				buf[i][j].rgbRed = color[2] / change;
			}
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			px[i][j].rgbBlue = buf[i][j].rgbBlue;
			px[i][j].rgbGreen = buf[i][j].rgbGreen;
			px[i][j].rgbRed = buf[i][j].rgbRed;
		}

		free(buf[i]);
	}
	free(buf);
}


void quickSort(int* array, int low, int high)
{
	int i = low;
	int j = high;
	int pivot = array[(i + j) / 2];
	int temp;

	while (i <= j)
	{
		while (array[i] < pivot)
			i++;
		while (array[j] > pivot)
			j--;
		if (i <= j)
		{
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			i++;
			j--;
		}
	}
	if (j > low)
		quickSort(array, low, j);
	if (i < high)
		quickSort(array, i, high);
}


void median(PBITMAPINFOHEADER bi, PPX24* px)
{
	PPX24* buf = NULL;
	buf = (PPX24*)calloc(bi->biHeight, sizeof(PPX24));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (PPX24)calloc(bi->biWidth, sizeof(PX24));
	}
	int* bl = NULL, * gr = NULL, * rd = NULL;
	int change;
		 
	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			change = 0;

			for (int y = -2; y < 3; y++)
			{
				//check if the row exists
				if (y + i > -1 && y + i < bi->biHeight)
				{
					for (int x = -2; x < 3; x++)
					{
						//check if the col exists
						if (x + j > -1 && x + \
							j < bi->biWidth)
						{
							bl = (int*)realloc(bl,\
								sizeof(int) * (change + 1));
							gr = (int*)realloc(gr,\
								sizeof(int) * (change + 1));
							rd = (int*)realloc(rd,\
								sizeof(int) * (change + 1));
							bl[change] = px[i + y]\
								[j + x].rgbBlue;
							gr[change] = px[i + y]\
								[j + x].rgbGreen;
							rd[change] = px[i + y]\
								[j + x].rgbRed;
							change++;
						}
					}
				}
			}

			quickSort(bl, 0, change - 1);
			quickSort(gr, 0, change - 1);
			quickSort(rd, 0, change - 1);
			
			//if change is 0 then no pixels were found
			if (change != 0)
			{
				buf[i][j].rgbBlue = bl[change / 2];
				buf[i][j].rgbGreen = gr[change / 2];
				buf[i][j].rgbRed = rd[change / 2];
			}
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			px[i][j].rgbBlue = buf[i][j].rgbBlue;
			px[i][j].rgbGreen = buf[i][j].rgbGreen;
			px[i][j].rgbRed = buf[i][j].rgbRed;
		}

		free(buf[i]);
	}
	free(bl);
	free(gr);
	free(rd);
	free(buf);
}


void sharp(PBITMAPINFOHEADER bi, PPX24* px)
{
	PPX24* buf = NULL;
	buf = (PPX24*)calloc(bi->biHeight, sizeof(PPX24));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (PPX24)calloc(bi->biWidth, sizeof(PX24));
	}
	float color[3];
	int change;

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			if (i > 0 && i < bi->biHeight - 1 && j > 0 && j < bi->biWidth - 1)
			{
				change = 0;
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
				for (int y = -1; y < 2; y++)
				{
					//check if the row exists
					if (y + i > -1 && y + i < bi->biHeight)
					{
						for (int x = -1; x < 2; x++)
						{
							//check if the col exists
							if (x + j > -1 && x + j < bi->biWidth)
							{
								if (x != 0 || y != 0)
								{
									color[0] -= (float)px[i + y]\
										[j + x].rgbBlue * 0.1;
									color[1] -= (float)px[i + y]\
										[j + x].rgbGreen * 0.1;
									color[2] -= (float)px[i + y]\
										[j + x].rgbRed * 0.1;
								}
								else
								{
									color[0] += (float)px[i + y]\
										[j + x].rgbBlue * 1.8;
									color[1] += (float)px[i + y]\
										[j + x].rgbGreen * 1.8;
									color[2] += (float)px[i + y]\
										[j + x].rgbRed * 1.8;
								}
								change++;
							}
						}
					}
				}

				//if change is 0 then no pixels were found
				if (change != 0)
				{
					buf[i][j].rgbBlue = min(max(color[0]));
					buf[i][j].rgbGreen = min(max(color[1]));
					buf[i][j].rgbRed = min(max(color[2]));
				}
			}
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			if (i > 0 && i < bi->biHeight - 1 && j > 0 \
				&& j < bi->biWidth - 1)
			{
				px[i][j].rgbBlue = buf[i][j].rgbBlue;
				px[i][j].rgbGreen = buf[i][j].rgbGreen;
				px[i][j].rgbRed = buf[i][j].rgbRed;
			}
		}

		free(buf[i]);
	}
	free(buf);
}


//for 8bit bmp
void bloor8(PBITMAPINFOHEADER bi, BYTE** px, RGBQUAD** color)
{
	BYTE** buf = NULL;
	buf = (BYTE**)calloc(bi->biHeight, sizeof(BYTE*));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (BYTE*)calloc(bi->biWidth, sizeof(BYTE));
	}
	int colors[3];
	int change;

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			change = 0;
			colors[0] = 0;
			colors[1] = 0;
			colors[2] = 0;

			for (int y = -2; y < 3; y++)
			{
				//check if the row exists
				if (y + i > -1 && y + i < bi->biHeight)
				{
					for (int x = -2; x < 3; x++)
					{
						//check if the col exists
						if (x + j > -1 && x + j < bi->biWidth)
						{
							colors[0] += color[px[i + y]\
								[j + x]]->rgbBlue;
							colors[1] += color[px[i + y]\
								[j + x]]->rgbGreen;
							colors[2] += color[px[i + y]\
								[j + x]]->rgbRed;
							change++;
						}
					}
				}
			}

			colors[0] = colors[0] / change;
			colors[1] = colors[1] / change;
			colors[2] = colors[2] / change;

			int bl, gr, rd;
			int rt = 255;
			int min_coef = 99999;
			int coef;
			for (int k = 0; k < 2 << 7; ++k)
			{
				bl = color[k]->rgbBlue;
				gr = color[k]->rgbGreen;
				rd = color[k]->rgbRed;
				coef = 11 * (bl - colors[0]) * (bl - colors[0]) \
					+ 59 * (gr - colors[1]) * (gr - colors[1]) \
					+ 30 * (rd - colors[2]) * (rd - colors[2]);
				if (coef < min_coef)
				{
					min_coef = coef;
					rt = k;
				}
			}

			//if change is 0 then no pixels were found
			if (change != 0)
			{
				buf[i][j] = rt;
			}
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			px[i][j] = buf[i][j];
		}

		free(buf[i]);
	}
	free(buf);
}


void sharp8(PBITMAPINFOHEADER bi, BYTE** px, RGBQUAD** color)
{
	BYTE** buf = NULL;
	buf = (BYTE**)calloc(bi->biHeight, sizeof(BYTE*));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (BYTE*)calloc(bi->biWidth, sizeof(BYTE));
	}
	float colors[3];

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			colors[0] = 0;
			colors[1] = 0;
			colors[2] = 0;

			for (int y = -1; y < 2; y++)
			{
				//check if the row exists
				if (y + i > -1 && y + i < bi->biHeight)
				{
					for (int x = -1; x < 2; x++)
					{
						//check if the col exists
						if (x + j > -1 && x + j < bi->biWidth)
						{
							if (y || x)
							{
								colors[0] += color[px[i + y]\
									[j + x]]->rgbBlue * -0.1;
								colors[1] += color[px[i + y]\
									[j + x]]->rgbGreen * -0.1;
								colors[2] += color[px[i + y]\
									[j + x]]->rgbRed * -0.1;
							}
							else
							{
								colors[0] += color[px[i + y]\
									[j + x]]->rgbBlue * 1.8;
								colors[1] += color[px[i + y]\
									[j + x]]->rgbGreen * 1.8;
								colors[2] += color[px[i + y]\
									[j + x]]->rgbRed * 1.8;
							}
						}
					}
				}
			}

			colors[0] = min(max(colors[0]));
			colors[1] = min(max(colors[1]));
			colors[2] = min(max(colors[2]));

			int bl, gr, rd;
			int rt = 255;
			int min_coef = 99999;
			int coef;
			for (int k = 0; k < 2 << 7; ++k)
			{
				bl = color[k]->rgbBlue;
				gr = color[k]->rgbGreen;
				rd = color[k]->rgbRed;
				coef = 11 * (bl - colors[0]) * (bl - colors[0]) + \
					59 * (gr - colors[1]) * (gr - colors[1]) + \
					30 * (rd - colors[2]) * (rd - colors[2]);
				if (coef < min_coef)
				{
					min_coef = coef;
					rt = k;
				}
			}

			buf[i][j] = rt;
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			if (i > 0 && i < bi->biHeight - 1 && \
				j > 0 && j < bi->biWidth - 1)
				px[i][j] = buf[i][j];
		}

		free(buf[i]);
	}
	free(buf);
}


void median8(PBITMAPINFOHEADER bi, BYTE** px, RGBQUAD** color)
{
	BYTE** buf = NULL;
	buf = (BYTE**)calloc(bi->biHeight, sizeof(BYTE*));
	for (int i = 0; i < bi->biHeight; ++i)
	{
		buf[i] = (BYTE*)calloc(bi->biWidth, sizeof(BYTE));
	}
	int colors[3];
	int* bl = NULL, * gr = NULL, * rd = NULL;
	int change;

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			change = 0;

			for (int y = -2; y < 3; y++)
			{
				//check if the row exists
				if (y + i > -1 && y + i < bi->biHeight)
				{
					for (int x = -2; x < 3; x++)
					{
						//check if the col exists
						if (x + j > -1 && x + j < bi->biWidth)
						{
							bl = (int*)realloc(bl, \
								sizeof(int) * (change + 1));
							gr = (int*)realloc(gr, \
								sizeof(int) * (change + 1));
							rd = (int*)realloc(rd, \
								sizeof(int) * (change + 1));
							bl[change] = color[px[i + y]\
								[j + x]]->rgbBlue;
							gr[change] = color[px[i + y]\
								[j + x]]->rgbGreen;
							rd[change] = color[px[i + y]\
								[j + x]]->rgbRed;
							change++;
						}
					}
				}
			}

			quickSort(bl, 0, change - 1);
			quickSort(gr, 0, change - 1);
			quickSort(rd, 0, change - 1);

			colors[0] = bl[change / 2];
			colors[1] = gr[change / 2];
			colors[2] = rd[change / 2];

			int blu, gre, red;
			int rt = 255;
			int min_coef = 99999;
			int coef;
			for (int k = 0; k < 2 << 7; ++k)
			{
				blu = color[k]->rgbBlue;
				gre = color[k]->rgbGreen;
				red = color[k]->rgbRed;
				coef = 11 * (blu - colors[0]) * (blu - colors[0]) +\
					59 * (gre- colors[1]) * (gre - colors[1]) +\
					30 * (red - colors[2]) * (red - colors[2]);
				if (coef < min_coef)
				{
					min_coef = coef;
					rt = k;
				}
			}

			//if change is 0 then no pixels were found
			if (change != 0)
			{
				buf[i][j] = rt;
			}
		}
	}

	for (int i = 0; i < bi->biHeight; i++)
	{
		for (int j = 0; j < bi->biWidth; j++)
		{
			px[i][j] = buf[i][j];
		}

		free(buf[i]);
	}
	free(bl);
	free(gr);
	free(rd);
	free(buf);
}
