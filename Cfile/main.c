#include "bmp_header.h"
#include "info.h"
#include "pixels.h"
#include "change_scale.h"
#include "change_format.h"
#include "filter_commander.h"
#pragma warning(disable:4996)


int main(int argc, char** argv)
{
	
	FILE* f = NULL;
	FILE* out = NULL;
	char fname[128];
	char oname[128] = {'\0'};
	int key_o = 0;

	//command line arguments
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (!strcmp(argv[i], "-i"))
			{
				print_info();
				return 0;
			}

			else if (!strcmp(argv[i], "-h"))
			{
				help_();
				return 0;
			}

			else if (!strcmp(argv[i], "-f"))
			{
				if (i != argc - 1)
				{
					strcpy(fname, argv[i + 1]);
					f = fopen(fname, "rb");
					if (!f)
					{
						printf("ERROR - couldn't \
open file %s", fname);
						exit(1);
					}
					i++;
				}
				else
				{
					printf("too few arguments, \
one arguments was expected");
					exit(1);
				}
			}

			else if (!strcmp(argv[i], "-o"))
			{
				if (i != argc - 1)
				{
					strcpy(oname, argv[i + 1]);
					key_o = 1;
					i++;
				}
				else
				{
					printf("too few arguments, one \
arguments was expected\n");
					exit(1);
				}
			}
		}
	}


	// hello
	printf("bmp editor\nby Rahmatullin Timur\n \
Simple editor for bmp images\nsimple methods are \
implemented: bloor, median filter, sharpness\n");
	printf("you can also change the scale and\n \
save the image in monochrome, 256 color, 16 bit and 24 bit format\n");


	while (1)
	{

		//menu
		char answer[128];
		if (!f)
		{
			while (1)
			{
				printf("\nchoose:\n");
				printf("1 - add image\n");
				printf("0 - exit\n");

				scanf("%s", answer);
				if (!strcmp(answer, "1"))
					break;
				if (!strcmp(answer, "0"))
					exit(0);
				else
					printf("pls try again\n");
			}
		}


		//add bmp image
		while (!f)
		{
			printf("write the path to bmp image: ");
			scanf("%s", fname);

			if (!strcmp(fname, "exit"))
				exit(1);

			f = fopen(fname, "rb");
			if (!f)
			{
				printf("couldn't open file %s, try \
again or write 'exit' to exit\n", fname);
			}
		}
		PBITMAPINFO pbm = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFO));
		pbm = read_bmp_head(f);
		PBITMAPFILEHEADER pbfh = pbm->bf;
		PBITMAPINFOHEADER pbih = pbm->bi;


		//for bmp(8, 4, 2, 1) bit format
		RGBQUAD** color = NULL;
		int bitCount = pbih->biBitCount;
		if (bitCount <= 8)
		{
			color = read_color(f, bitCount);
		}


		//read pixels
		PPX24* pxbyte = (PPX24*)calloc(1, sizeof(PPX24));
		BYTE** pxbit = NULL;
		if (bitCount <= 8)
			pxbit = read_bmpbit(f, pbfh->bfOffBits, \
				pbih->biHeight, pbih->biWidth, bitCount);
		else
			pxbyte = read_bmpbyte(f, pbfh->bfOffBits, \
				pbih->biHeight, pbih->biWidth, bitCount);


		//main cycle
		while (1)
		{
			printf("\nEditor tools:\n");
			if (bitCount >= 8)
				printf("1 - filters\n");
			printf("2 - change scale\n");
			printf("3 - change bmp format\n");
			printf("0 - save image\n");

			scanf("%s", answer);
			if (!strcmp(answer, "1")) //filters
			{
				if (bitCount == 8)
					filter8bit(pbih, pxbit, color);
				else if (bitCount > 8)
					filter(pbih, pxbyte);
				else
					printf("pls try again\n");
			}
			else if (!strcmp(answer, "2")) //change scale;
			{
				if (bitCount >= 16)
					pxbyte = scalebyte(pbm, pxbyte);
				else
					pxbit = scalebit(pbm, pxbit);
			}
			else if (!strcmp(answer, "3")) //change bmp format;
			{
				PX* px = (PX*)calloc(1, sizeof(PX));
				px->pxbyte = pxbyte;
				px->pxbit = pxbit;
				px->color = color;
				bitCount = change_format(pxbit, pxbyte, pbm, color, px);
				pxbyte = px->pxbyte;
				pxbit = px->pxbit;
				color = px->color;
				free(px);
			}
			else if (!strcmp(answer, "0")) // exit
				break;
			else
				printf("pls try again\n");
		}


		// close input file and create outputfile
		fclose(f);
		f = NULL;
		while (!out)
		{
			if (!key_o)
			{
				printf("write the path to save bmp image: ");
				scanf("%s", oname);
			}

			if (!strcmp(oname, "exit"))
			{
				out = fopen(fname, "wb");
			}

			out = fopen(oname, "wb");
			if (!out)
			{
				printf("couldn't create file %s, try again \
or write 'exit' to rewrite %s\n", oname, fname);
			}
		}


		// save bmp header and RGBtable
		save_bmp_head(out, pbm);
		if (bitCount <= 8)
		{
			write_color(out, color, bitCount);
		}


		//save pixels
		if (bitCount <= 8)
			save_bmpbit(out, pbfh->bfOffBits, pbih->biHeight,\
				pbih->biWidth, bitCount, pxbit);
		else
			save_bmpbyte(out, pbfh->bfOffBits, pbih->biHeight,\
				pbih->biWidth, bitCount, pxbyte);


		//clear and save;
		fclose(out);
		out = NULL;
		if (!key_o)
			oname[0] = '\0';

		if (bitCount >= 16)
		{
			for (int i = 0; i < pbih->biHeight; i++)
				free(pxbyte[i]);
			free(pxbyte);
		}
		else
		{
			for (int i = 0; i < pbih->biHeight; i++)
				free(pxbit[i]);
			free(pxbit);
		}

		free(pbfh);
		free(pbih);
		free(pbm);
	}
	
	return 0;
}
