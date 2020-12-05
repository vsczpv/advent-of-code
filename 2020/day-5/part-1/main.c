#include "../../shared-code.h"
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
	double xf = 0;
	double xc = HEIGHT;
	// Calculate it HEIGHDEPTH times
	for (int i = 0; i < HEIGHTDEPTH; i++)
		// If F then lower the ceiling
		if (str[i] == 'F') xc -= floor((xc-xf)/2.f) + 1;
		// Else raise the floor
		else xf += floor((xc-xf)/2.f) + 1;

	// Do the same but for Y.
	double yf;
	double yc = WIDTH;
	for (int i = HEIGHTDEPTH; i < WIDTHDEPTH + HEIGHTDEPTH; i++)
		if (str[i] == 'L') yc -= floor((yc-yf)/2.f) + 1;
		else yf += floor((yc-yf)/2.f) + 1;

	// Return x,y.
	res->x = (int) xc;
	res->y = (int) yc;

	return;
}

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");

	char* index = input->buffer;

	int highid = 0;
	int i = 0;
	while (i < strlen(index))
	{
		intxy coord;
		decodebsp(&(index[i]), &coord);
		while (index[i] != '\n') i++; i++;
		int id = coord.x * (HEIGHTDEPTH + 1) + coord.y;
		highid = id > highid ? id : highid;
	}

	printf("%i\n", highid);

	freeinputfile(input);

	return 0;
}
