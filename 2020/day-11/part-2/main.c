#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// conway would be proud ;)

// States
// SETTOC and OCCTST are seat to occupied and occupied to seat
// middle states, so that we cut down one table
// i could add more states to remove the lstable, but its 3am rn.
#define EMPTY 0
#define SEAT 1
#define OCCP 2
#define SETTOC 3
#define OCCTST 4

// Prints table
void printtable(char** table, int lcount, int width)
{

	for (int y = 0; y < lcount; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Get cell
			char c = table[x][y];
			// Convert to correct character and print
			c = c == SEAT ? 'L' : c;
			c = c == OCCP ? '#' : c;
			c = c == EMPTY ? '.' : c;
			printf("%c", c);
		}
		printf("\n");
	}

	return;
}

// getcell get the value of a cell, handles underflow/overflow and also
// the conversion of SETTOC/ OCCTST to SEAT/OCCP
char getcell(char** table, int px, int py, int vx, int vy, int lcount, int width)
{
	char res = EMPTY;

	// while not found anything
	while (res == EMPTY)
	{
		// update position
		px += vx; py += vy;
		// return if underflow/oveflow
		if (px < 0 || px >= width) return SEAT;
		if (py < 0 || py >= lcount) return SEAT;
		// get cell
		res = table[px][py];
	}

	// convert into correct data
	res = res == SETTOC ? SEAT : res;
	res = res == EMPTY ? SEAT : res;
	res = res == OCCTST ? OCCP : res;
	return res;
}

// compares two table together
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

	// Conut width of the input
	int width = 0;
	while (index[0] != '\n') { width++; index++; }
	index = input->buffer;

	// Count height of the input
	int lcount = countlines(index);
	// Allocate first table
	char** table = (char**) malloc(sizeof(char*)*width);
	for (int j = 0; j < width; j++) table[j] = (char*) malloc(sizeof(char)*lcount);
	// Allocate second table
	char** lstable = (char**) malloc(sizeof(char*)*width);
	for (int j = 0; j < width; j++) lstable[j] = (char*) malloc(sizeof(char)*lcount);

	// Cycle through input
	int len = strlen(index);
	int i = 0;
	int k = 0;
	while (i < len)
	{
		// Cycle through line
		int j = 0;
		while (index[i] != '\n')
		{
			// Add cell to table
			table[j][k] = index[i] == 'L' ? SEAT : EMPTY;
			j++; i++;
		}
		k++; i++;
	}

	// While the simulation lasts
	while (true)
	{
		// Cycle through table
		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				if (table[i][k] == SEAT)
				{
					// Check for seats in view
					// Skip if any found
					if (getcell(table, i, k, -1, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 0, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, -1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, -1, 0, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, 0, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, -1, 1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 0, 1, lcount, width) != SEAT) continue;
					if (getcell(table, i, k, 1, 1, lcount, width) != SEAT) continue;
					// Set to occupied
					table[i][k] = SETTOC;
				} else if (table[i][k] == OCCP) {
					// Check for occupied seats in view
					int thrslhd = 0;
					if (getcell(table, i, k, -1, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 0, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, -1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, -1, 0, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, 0, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, -1, 1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 0, 1, lcount, width) == OCCP) thrslhd++;
					if (getcell(table, i, k, 1, 1, lcount, width) == OCCP) thrslhd++;
					// Skip if less than five found
					if (thrslhd < 5) continue;
					// Set to seat
					table[i][k] = OCCTST;
				}

		// Convert intermediary SETTOC/OCCTST to SEAT/OCCP respectively
		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				{
					table[i][k] = table[i][k] == SETTOC ? OCCP : table[i][k];
					table[i][k] = table[i][k] == OCCTST ? SEAT : table[i][k];
				}

		// Compare laststate with current state
		if (comparetable(table, lstable, lcount, width)) break;
		// Save current state
		for (k = 0; k < lcount; k++)
			for (i = 0; i < width; i++)
				lstable[i][k] = table[i][k];
	}

	// Count occupied seats
	int res = 0;
	for (k = 0; k < lcount; k++)
		for (i = 0; i < width; i++)
			if (table[i][k] == OCCP) res++;

	// Print result
	printtable(table, lcount, width);
	printf("\n%i\n", res);

	// Free stuff
	for (int j = 0; j < width; j++) free(table[j]);
	for (int j = 0; j < width; j++) free(lstable[j]);
	free(table);
	free(lstable);
	freeinputfile(input);

	return 0;
}
