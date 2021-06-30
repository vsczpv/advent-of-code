#include "shared-code.h"
#include <string.h>
#include <stdio.h>

#define TOKAMNT 7

// The tokens we're looking for
static const char tokens[TOKAMNT][5] =
{
	"byr:",
	"iyr:",
	"eyr:",
	"hgt:",
	"hcl:",
	"ecl:",
	"pid:"
};

int main(int argc, char* argv[])
{
	// Initializing input file...
	inputFile* input = getinputfile("input.txt");

	// .. and other variables.
	char* index = input->buffer;
	char* endindex = index;
	int valid = 0;

	while (endindex)
	{
		int count = 0;
		// Get end of batch
		endindex = strstr(index, "\n\n");
		// Check for tokens
		for (int i = 0; i < TOKAMNT; i++)
			if (strnstr(index, tokens[i], endindex-index)) count++;
		// If found TOKAMNT tokens, then it's valid
		if (count == TOKAMNT) valid++;
		index = endindex + 1;
	}

	// Print result
	printf("%i\n", valid);

	// Free stuff
	freeinputfile(input);

	return 0;
}
