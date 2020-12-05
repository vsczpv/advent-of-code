#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 7
#define HEIGHT 127
#define HEIGHTDEPTH 7
#define WIDTHDEPTH 3

// Integer vector structure
typedef struct intxy
{
	int x;
	int y;
} intxy;

// Decode binary space thingy
void decodebsp(char* str, intxy* res)
{
	// Ceiling and floor variables
	double yf = 0;
	double yc = HEIGHT;
	// Calculate it HEIGHDEPTH times
	for (int i = 0; i < HEIGHTDEPTH; i++)
		// If F then lower the ceiling
		if (str[i] == 'F') yc -= floor((yc-yf)/2.f) + 1;
		// Else raise the floor
		else yf += floor((yc-yf)/2.f) + 1;

	// Do the same but for X.
	double xf = 0;
	double xc = WIDTH;
	for (int i = HEIGHTDEPTH; i < WIDTHDEPTH + HEIGHTDEPTH; i++)
		if (str[i] == 'L') xc -= floor((xc-xf)/2.f) + 1;
		else xf += floor((xc-xf)/2.f) + 1;

	// Return x,y.
	res->x = (int) xc;
	res->y = (int) yc;

	return;
}

bool seats[WIDTH+1][HEIGHT+1] = { false };

int main(int argc, char* argv[])
{

	for (int x = 0; x <= WIDTH; x++)
		for (int y = 0; y <= HEIGHT; y++)
			seats[x][y] = false;

	inputFile* input = getinputfile("input.txt");

	char* index = input->buffer;

	int highid = 0;
	int i = 0;
	while (i < strlen(index))
	{
		intxy coord;
		decodebsp(&(index[i]), &coord);
		while (index[i] != '\n') i++; i++;
		seats[coord.x][coord.y] = true;
	}

	for (int y = 0; y <= HEIGHT; y++)
		for (int x = 0; x <= WIDTH; x++)
			//{if (x == 0) printf("\n");printf("%i ", seats[x][y]);}
			if (seats[x][y] == false)
			{
				int sx = x;
				int sy = y;
				int id = y * (HEIGHTDEPTH + 1) + x;

				int a1id = id - 1;
				int a2id = id + 1;

				int a1sx = a1id % (HEIGHTDEPTH + 1);
				int a1sy = a1id / (HEIGHTDEPTH + 1);

				int a2sx = a2id % (HEIGHTDEPTH + 1);
				int a2sy = a2id / (HEIGHTDEPTH + 1);

				printf("Seat %i %i %i, Adj1 %i is %i, Adj2 %i is %i\n", x, y, id, a1id, seats[a1sx][a1sy], a2id, seats[a2sx][a2sy]);
			}

	freeinputfile(input);

	return 0;
}
