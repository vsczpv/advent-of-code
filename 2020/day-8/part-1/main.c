#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OPTS 3

// Instruction strucutre
// Holds instruction id and argument
typedef struct inst
{
	char instruction;
	long int argument;
} inst;

// Op table
// used to decode Instructions from text to id's
static const char optable[OPTS][4] =
{
	"nop",
	"acc",
	"jmp"
};

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Get how many lines in file
	int nlcount = countlines(index);
	// Allocate code array based of line amount
	inst* code = (inst*) malloc(sizeof(inst)*nlcount);
	int len = strlen(index);
	int i = 0;
	// k holds code index
	int k = 0;
	// Cycle through input
	while (i < len)
	{
		// Get instruction
		int j = skipspace(1, i, index);
		// Convert it from text to id
		int o; for (o = 0; o < OPTS; o++)
			if (strncmp(&(index[i]), optable[o], j-i-1) == 0) break;
		i = j;
		// Get argument
		long int arg = strtol(&(index[i]), NULL, 10);
		// Store instruction id and argument into code structure
		code[k].instruction = (char) o;
		code[k].argument = arg;
		// Go to next line
		i = skipspace(1, i, index); i++;
		// Advance code index
		k++;
	}

	i = 0;
	// Accumulator variable
	long int acc = 0;
	// isRepeat keeps track of executed instructions
	bool* isRepeat = (bool*) malloc(sizeof(bool)*nlcount);
	while (i < nlcount)
	{
		// If instruction is repeating then break
		if (isRepeat[i]) break;
		// Mark instruction as repeated
		isRepeat[i] = true;
		// Switch for instructions
		switch (code[i].instruction)
		{
			case 0: // nop
				i++;
				break;
			case 1: // acc
				acc += code[i].argument;
				i++;
				break;
			case 2: // jmp
				i += code[i].argument;
				break;
			default:
				printf("error: invalid instruction.\n");
				exit(1);
		}
	}

	// print result
	printf("%li\n", acc);

	// free stuff
	free(code);
	free(isRepeat);
	freeinputfile(input);

	return 0;
}
