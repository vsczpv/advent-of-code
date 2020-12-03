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
		// Get offset1 and offset2 from file
		// Note that they're not zero indexed.
		long int o1 = strtol(index, &index, 10) - 1;
		long int o2 = -strtol(index, &index, 10) - 1;

		// Get the special character
		index++;
		char c = index[0];
		index += 3;

		// If one but not two charaters from the offsets
		// are a special character increment valid
		if ((index[o1] == c && index[o2] != c) || (index[o1] != c && index[o2] == c)) valid++;

		// Got to next line
		while (index[0] != '\n') index++; index++;

	}

	// Print result
	printf("%i\n", valid);

	// Dispense input file
	freeinputfile(input);

	return 0;
}
