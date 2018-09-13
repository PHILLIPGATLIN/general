#include <stdio.h>
#include <string.h>
unsigned int row=0, column=0;
FILE *file;
FILE *fromthisfile;
FILE *tothatfile;
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
void pagetofile(void);
void pagetofile()
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
				fputc(c, file);
		}
	}
	return;
}
void filetopage(void);
void filetopage(void)
{
	int c=0;
	rewind(file);
	for (row = 0; row < 1000; row++)
	{
		for (column = 0; column < 1000; column++)
		{
			c = fgetc(file);
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
void merge(void);
void merge()
{
	int c=0;
	while ((c = fgetc(fromthisfile)) != EOF)
	{
		fputc(c, tothatfile);
	}
	return;
}
void help(void);
void help()
{
	printf("txt is a general purpose program for creating and manipulating\n"
	       "plain text documents.\n"
	       "Acceptable input forms are of the kind 'txt [command] [operands]'.\n"
	       "1.\tTo create a new plain text document, enter:\n"
	       "\t txt [filename]\n"
	       "2.\ttxt is able to append the contents of one file to another file:\n"
	       "\t txt merge [append this FILE] [to this FILE]\n"
	       "3.\tYou can also replace each instance of a word in a file with another word:\n"
	       "\t txt replace [this WORD] [with this WORD] [in this FILE]\n");
	return;
}
int input(void);
int input()
{
	int c=0;
	while (1)
	{
		c = getchar();
		if ((c >= 32 && c <= 126) || c == '\t' || c == '\n')
		{
			chartopage(c);
		}
		else if (c == EOF)
		{
			pagetofile();
			printf("SAVED!\n");
			return 0;
		}
		else
			;
	}
}
void emptypage(void);
void emptypage()
{
	for (row=0; row < 1000; row++)
	{
		for (column=0; column < 1000; column++)
			page[row][column] = 0;
	}
	return;
}
void FileError(void);
void FileError()
{
	printf("ERROR: the file was not able to be opened by the program.\n"
       	       "Does the program have permissions to open the file? Is it a directory?\n");
	return;
}
int main(int argc, char *argv[])
{
	int n=0, c=0;
	if (argc < 2 || argc > 4)
	{
		printf("ERROR: input was too much, too little, or otherwise not recognized.\n"
		       "USAGE: txt [file name or file path]\n"
		       "e.g. txt /home/terry/document.txt\n"
		       "enter 'txt help' for a more complete description of\n"
		       "the program and its abilities.\n");
		return -1;
	}
	emptypage();
	if (strcmp("help", argv[1]) == 0)
	{
		help();
		return 0;
	}
	else if (strcmp("merge", argv[1]) == 0)
	{
		if ((fromthisfile = fopen(argv[2], "r")) == NULL || (tothatfile = fopen(argv[3], "a+")) == NULL)
		{
			FileError();
			return -2;
		}
		file = tothatfile;
		merge();
		filetopage();
		drawpage();
		emptypage();
		input();
		fclose(fromthisfile);
		fclose(tothatfile);
		return 0;
	}
	else
	{
		if ((file = fopen(argv[1], "a+")) == NULL)
		{
			FileError();
			return -2;
		}
		else
		{
			filetopage();
			drawpage();
			emptypage();
			input();
			fclose(file);
			return 0;
		}
	}
	return 0;
}
