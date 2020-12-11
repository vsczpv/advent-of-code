#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// GAME-OF-LIFE PUZZLE YAAAAAAAY

// States
// SETTOC and OCCTST are seat to occupied and occupied to seat
// middle states, so that we cut down one table
// i could add more states to remove the lstable, but its 3am rn.
#define EMPTY 0
#define SEAT 1
#define OCCP 2
#define SETTOC 3
#define OCCTST 4

void printtable(char** table, int lcount, int width)
{

	for (int y = 0; y < lcount; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char c = table[x][y];
			c = c == SEAT ? 'L' : c;
			c = c == OCCP ? '#' : c;
			c = c == EMPTY ? '.' : c;
			printf("%c", c);
		}
		printf("\n");
	}

	return;
}

char getcell(char** table, int px, int py, int vx, int vy, int lcount, int width)
{
	char res = EMPTY;

	while (res == EMPTY)
	{
		px += vx; py += vy;
		if (px < 0 || px >= width) return SEAT;
		if (py < 0 || py >= lcount) return SEAT;
		res = table[px][py];
	}

	res = res == SETTOC ? SEAT : res;
	res = res == EMPTY ? SEAT : res;
	res = res == OCCTST ? OCCP : res;
	return res;
}

bool comparetable(char** table, char** lstable, int lcount, int width)
{
	bool res = false;
	for (int y = 0; y < lcount; y++)
		for (int x = 0; x < width; x++)
			if (table[x][y] != lstable[x][y]) return res;
	res = true;
	return res;
}

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	int width = 0;
	while (index[0] != '\n') { width++; index++; }
	index = input->buffer;

	int lcount = countlines(index);
	char** table = (char**) malloc(sizeof(char*)*width);
	for (int j = 0; j < width; j++) table[j] = (char*) malloc(sizeof(char)*lcount);

	char** lstable = (char**) malloc(sizeof(char*)*width);
	for (int j = 0; j < width; j++) lstable[j] = (char*) malloc(sizeof(char)*lcount);

	int len = strlen(index);
	int i = 0;
	int k = 0;
	while (i < len)
	{
		int j = 0;
		while (index[i] != '\n')
		{
			table[j][k] = index[i] == 'L' ? SEAT : EMPTY;
			j++; i++;
		}
		k++; i++;
	}

	while (true)
	{
		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				if (table[i][k] == SEAT)
				{
					if (getcell(table, i, k, -1, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 0, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, -1, 0, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, 0, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, -1, 1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 0, 1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, 1, lcount, width) != SEAT) continue;
					table[i][k] = SETTOC;
				} else if (table[i][k] == OCCP) {
					int thrslhd = 0;
					if (getcell(table, i, k, -1, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 0, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, -1, 0, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, 0, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, -1, 1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 0, 1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, 1, lcount, width) == OCCP) thrslhd++;
					if (thrslhd < 5) continue;
					table[i][k] = OCCTST;
				}

		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				{
					table[i][k] = table[i][k] == SETTOC ? OCCP : table[i][k];
					table[i][k] = table[i][k] == OCCTST ? SEAT : table[i][k];
				}

		if (comparetable(table, lstable, lcount, width)) break;
		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				lstable[i][k] = table[i][k];
	}

	int res = 0;
	for (k = 0; k < lcount; k++)
		for (i = 0; i < width; i++)
			if (table[i][k] == OCCP) res++;

	printtable(table, lcount, width);
	printf("\n%i\n", res);

	for (int j = 0; j < width; j++) free(table[j]);
	for (int j = 0; j < width; j++) free(lstable[j]);
	free(table);
	free(lstable);
	freeinputfile(input);

	return 0;
}
