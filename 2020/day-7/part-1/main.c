#include "../../shared-code.h"
#include <../../linkmacro.h>
#include <../../link.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct sbag
{
	char* type;
	vlink* holds;
} sbag;

typedef struct dbag
{
	char* type;
	int amnt;
} dbag;

int skipspace(int spaces, int i, char* str)
{
	int j = i;
	int count = 0;
	while (count < spaces)
	{
		if (str[j] == '\n') break;
		if (str[j] == ' ') count++; j++;
	}
	return j;
}

vlink* removedummylink(vlink* entry)
{
	vlink* temp = entry;
	entry = splitlink(entry, LINK_SPLITAFT);
	purgelink(temp);
	return entry;
}

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input2.txt");
	char* index = input->buffer;

	int i = 0;
	int len = strlen(index);
	vlink* bagtypes = createlink(NULL);
	while (i < len)
	{
		int j = skipspace(2, i, index);
		sbag* nbag = (sbag*) malloc(sizeof(sbag));
		nbag->type = (char*) malloc(sizeof(char)*(j-i) + 1);
		strncpy(nbag->type, &index[i], j-i-1);
		j = skipspace(2, j, index); i = j;
		nbag->holds = createlink(NULL);
		while (index[i] != '\n')
		{
			int amnt = strtol(&index[i], NULL, 10);
			j = skipspace(1, i, index); i = j;
			j = skipspace(2, i, index);
			dbag* nnbag = (dbag*) malloc(sizeof(dbag));
			nnbag->type = (char*) malloc(sizeof(char)*(j-i) + 1);
			nnbag->amnt = amnt;
			strncpy(nnbag->type, &index[i], j-i-1);
			addlink(nnbag, nbag->holds, LINK_APPEND);
			i = skipspace(1, j, index);
		}
		nbag->holds = removedummylink(nbag->holds);
		addlink(nbag, bagtypes, LINK_APPEND);

		i++;
	}
	bagtypes = removedummylink(bagtypes);

	for (int j = 0; j <= countlink(bagtypes); j++)
	{
		sbag* nbag = (sbag*) (seeklink(bagtypes, LINK_FROMFIRST, j)->data);
		printf("\n%s:\n", nbag->type);
		for (int k = 0; k <= countlink(nbag->holds); k++)
		{
			dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, k)->data);
			printf("\t%s\n", nnbag->type);
		}
	}

	purgelink(bagtypes);
	freeinputfile(input);

	return 0;
}
