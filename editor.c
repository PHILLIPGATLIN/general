#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
unsigned int row=0, column=0;
char page[1000][1000];
unsigned int istext(int);
unsigned int istext(int c)
{
	if ((c >= 0x20 && c <= 0x7E) || c == 0x09)
		return 1;
	else
		return 0;
}
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
void deletecharinpage(void);
void deletecharinpage()
{
	if (column > 0)
	{
		--column;
		page[row][column] = 0;
		return;
	}
	else if ((column == 0) && (row > 0))
	{
		--row;
		column = 999;
		while (!(istext(page[row][column])))
			--column;
		page[row][column] = 0;
		return;
	}
	else
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
	int n=0, pger=0, pgec=0, kbd=0;
	int c=0;
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
	while (1)
	{
		c = getchar();
		if (isalnum(c) || isspace(c))
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
	return 0;
}