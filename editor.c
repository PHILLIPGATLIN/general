#include <stdio.h>
#include <string.h>
#include <ctype.h>
unsigned int row=0, column=0;
char page[1000][1000];
void drawpage(void);
void drawpage ()
{
	int c=0, rowcpy, columncpy;
	rowcpy = row;
	columncpy = column;
	for (row=0; row < 1000; row++)
	{
		for (column=0; column < 1000; column++)
		{
			c = page[row][column];
			if (c == 0)
				;
			else
				putchar(c);
		}
	}
	row = rowcpy;
	column = columncpy;
	return;
}
void pagetofile(FILE *);
void pagetofile(FILE *f)
{
	int c=0;
	for (row=0; row < 1000; row++)
	{
		for (column=0; column < 1000; column++)
		{
			c = page[row][column];
			if (c == 0)
				;
			else
				fputc(c, f);
		}
	}
	return;
}
void filetopage(FILE *);
void filetopage(FILE *F)
{
	int c=0;
	rewind(F);
	for (row = 0; row < 1000; row++)
	{
		for (column = 0; column < 1000; column++)
		{
			c = fgetc(F);
			if (c == EOF)
				return;
			else
				page[row][column] = c;
		}
	}
}
void chartopage(int);
void chartopage(int c)
{
	page[row][column] = c;
	if (c == '\n')
	{
		++row;
		column = 0;
	}
	else
		++column;
	return;
}
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("ERROR: expecting more arguments.\n"
		       "USAGE: txt [file]\n"
		       "e.g. txt /home/terry/document.txt\n");
		return -1;
	}
	int n=0, c=0;
	FILE *file;
	if ((file = fopen(argv[1], "a+")) == NULL)
	{
		printf("ERROR: the file was not able to be opened by the program.\n"
		       "Does the program have permissions to open the file? Is it a directory?\n");
		return -2;
	}
	for (row=0; row < 1000; row++)
	{
		for (column=0; column < 1000; column++)
			page[row][column] = 0;
	}
	filetopage(file);
	drawpage();
	for (row = 0; row < 1000; row++)
	{
		for (column = 0; column < 1000; column++)
		{
			page[row][column] = 0;
		}
	}
	row = 0;
	column = 0;
	while (1)
	{
		c = getchar();
		if ((c > = 32 && c <= 126) || c == '\t' || c == '\n')
		{
			chartopage(c);
		}
		else if (c == EOF)
		{
			pagetofile(file);
			printf("SAVED!\n");
			break;
		}
		else
			;
	}
	fclose(file);
	return 0;
}
