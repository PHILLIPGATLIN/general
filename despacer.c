#include <stdio.h>
#include <string.h>
char old[255];
char new[255];
char c;
int despacer(int, char**);
int despacer(int agindex, char** ags)
{
    if (agindex == 0)
        return 1;
    int index1 = 0;
    int index2 = 0;
    strcpy(old, ags[agindex]);
    for (index1 = 0, index2 = 0; (c = old[index1]) != '\0'; index1++)
    {
        if (c == ' ' || c == '\t' || c == 39 || c == '(' || c == ')' || c == '"')
            continue;
        else
        {
            new[index2] = c;
            index2++;
        }
    }
    rename(old, new);
    agindex = agindex - 1;
    memset(old, 0, 255);
    memset(new, 0, 255);
    despacer(agindex,ags);
}
int main(int argc, char* argv[])
{
    if (argc == 1)
        return 1;
    memset(old, 0, 255);
    memset(new, 0, 255);
    c = 0;
    argc = argc - 1;
    despacer(argc, argv);
    return 1;
}
