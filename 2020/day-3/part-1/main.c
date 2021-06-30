#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

	// Count trees in path
	int count = 0;
	int x = 0;
	int y = 0;
	while (y <= height)
	{
		x += 3;
		y++;
		if (map[x%width][y]) count++;
	}

	// Print result
	printf("%i\n", count);

	// Free stuff
	freeinputfile(input);
	for (int i = 0; i < width; i++) free(map[i]);
	free(map);

	return 0;
}
