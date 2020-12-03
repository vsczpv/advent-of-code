#include "../../shared-code.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

	// Get input file
	inputFile* input = getinputfile("input.txt");

	// This is what holds how many valid passwords there are
	int valid = 0;
	// Get pointer to file buffer
	char* index = input->buffer;

	// Loop until end
	while (index[0] != '\0')
	{
		// Get min and max from file
		long int min = strtol(index, &index, 10);
		long int max = -strtol(index, &index, 10);

		// Get the special character
		index++;
		char c = index[0];
		index += 2;

		// Count how many special characters there are
		int ccount = 0;
		while (index[0] != '\n')
		{
			if (index[0] == c) ccount++; index++;
		}
		index++;

		// If value is between min and max increment valid.
		if (ccount >= min && ccount <= max) valid++;
	}

	// Print result
	printf("%i\n", valid);

	// Dispense input file
	freeinputfile(input);

	return 0;
}
