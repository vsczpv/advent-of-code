#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Function to get tree amount
// Takes a map, width, height, horizontal and vertical speed
int slide(bool** map, int width, int height, int vx, int vy)
{
	int count = 0;
	int x = 0;
	int y = 0;
	while (y <= height)
	{
		x += vx;
		y += vy;
		if (map[x%width][y]) count++;
	}
	return count;
}

int main(int argc, char* argv[])
{

	// Get input file
	inputFile* input = getinputfile("input.txt");

	// Initialize some variables
	char* index = input->buffer;
	int width = 0;
	int height = 0;

	// Get width and height of the forest
	while (index[0] != '\n') { width++; index++; } index = input->buffer;
	while (index[0] != '\0')
	{
		while (index[0] != '\n') index++; index++;
		height++;
	}

	// Allocate space for the map
	bool** map = (bool**) malloc(sizeof(bool*)*width);
	for (int i = 0; i < width; i++) map[i] = (bool*) malloc(sizeof(bool)*height);

	// Make a bool map of the forest, true being tree, false being empty.
	index = input->buffer;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++) { map[x][y] = index[0] == '#' ? true : false; index++; }
		index++;
	}

	// Get trees for each slope
	int t1 = slide(map, width, height, 1, 1);
	int t2 = slide(map, width, height, 3, 1);
	int t3 = slide(map, width, height, 5, 1);
	int t4 = slide(map, width, height, 7, 1);
	int t5 = slide(map, width, height, 1, 2);

	// Print result
	
	printf("%li\n", (long int) t1*t2*t3*t4*t5);

	// Free stuff
	freeinputfile(input);
	for (int i = 0; i < width; i++) free(map[i]);
	free(map);

	return 0;
}
