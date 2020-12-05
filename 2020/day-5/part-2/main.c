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

// Seats table
// This holds what seats are used or not.
bool seats[WIDTH+1][HEIGHT+1] = { false };

int main(int argc, char* argv[])
{

	// Old cleanup code because i am paranoid
	//for (int x = 0; x <= WIDTH; x++)
	//	for (int y = 0; y <= HEIGHT; y++)
	//		seats[x][y] = false;

	// Get input file, etc...
	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Go through input file
	int i = 0;
	while (i < strlen(index))
	{
		// Convert Binary spaced partitioning to X,Y coordinate.
		intxy coord;
		decodebsp(&(index[i]), &coord);
		// Go to the end of the line
		while (index[i] != '\n') i++; i++;
		// Mark seat as used
		seats[coord.x][coord.y] = true;
	}

	// Cycle through seats table
	for (int y = 0; y <= HEIGHT; y++)
		for (int x = 0; x <= WIDTH; x++)
			// Uncomment this and comment that other if for a visualisation of the seats
			//{if (x == 0) printf("\n");printf("%i ", seats[x][y]);}
			// Is seat empty?
			if (seats[x][y] == false)
			{
				// Get id from coordinates
				int id = y * (HEIGHTDEPTH + 1) + x;

				// Get adjecent ids
				int a1id = id - 1;
				int a2id = id + 1;

				// Get X,Y coordiantes for adj1
				int a1sx = a1id % (HEIGHTDEPTH + 1);
				int a1sy = a1id / (HEIGHTDEPTH + 1);

				// Get X,Y coordinates for adj2
				int a2sx = a2id % (HEIGHTDEPTH + 1);
				int a2sy = a2id / (HEIGHTDEPTH + 1);

				// If both adjecent seats are booked, then this is the right one.
				if (seats[a1sx][a1sy] && seats[a2sx][a2sy])
					printf("Your seat is %i, at row %i column %i.\n", id, y, x);
			}

	// Free stuff
	freeinputfile(input);

	return 0;
}
