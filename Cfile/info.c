#include "info.h"


void print_info()
{
	printf("student - Rahmatullin Timur Sergeevich\n");
	printf("FIIT, group m80-110-20\n");
	printf("lecturer - Egorova Evgeniya Kirillovna\n");
	printf("Department 813 Computer Mathematics, Moscow \
Aviation Institute\n");
}


void help_()
{
	printf("information about command line keys:\n");
	printf(" -h - (optional) information about program launch\n");
	printf(" -i - (optional) information about author\n");
	printf(" -f - (optional) need one argument: path to bmp image\n");
	printf(" -o - (optional) need one argument: output file name\n");

	printf("\ninformation about program launch:\n");
	printf("just launch this app without arguments in command line\n");
	printf("use only bmp format image\n");
	printf("also this app work only with formats:\nbmp1bit, \
bmp2bit, bmp4bit, bmp8bit, bmp16(555)bit and bmp24bit\n");
	printf("if you want to work with your image use the key -f\n");
	printf("if you need to save your image use the key -o\n");
	printf("these keys are optional\nyou can set all parameters\
 while the application is running\n");
	printf("you should have the 'color_table.col' file\n to\
 change palette to 8bit");
}
