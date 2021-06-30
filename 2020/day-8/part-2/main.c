#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OPTS 3

#define NOP 0
#define ACC 1
#define JMP 2

// I guess i should stop putting a big WARNING in all caps at the start of the file
// oh...
// anyway, since now i'm write comments here, might just aswell do it in this one

// This solution calculates all possible variations of NOP/JMP in a brute force way

// Instruction strucutre
// Holds instruction id, argument and checked state
typedef struct inst
{
	char instruction;
	long int argument;
	char checked;
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
		// also initialize checked
		code[k].instruction = (char) o;
		code[k].argument = arg;
		code[k].checked = (char) 0;
		// Go to next line
		i = skipspace(1, i, index); i++;
		// Advance code index
		k++;
	}

	// Accumulator variable
	long int acc;
	// isRepeat keeps track of executed instructions
	bool* isRepeat = (bool*) malloc(sizeof(bool)*nlcount);
	// Keep going until we find the answer
	while (true)
	{
		// didntFlip holds if we flipped a instruction or not
		bool didntFlip = true;
		// Cycle through code
		for (int k = 0; k < nlcount; k++)
		{
			// If failed flip
			// When a checked is at 1 it means that it was flipped
			// but the code still failed to terminate
			if (code[k].checked == 1)
			{
				// Flip it again
				code[k].instruction = code[k].instruction == NOP ? JMP : NOP;
				// Mark it as tested
				// 2 means it was re-flipped and can be ignored
				code[k].checked = 2;
			}
			// If not fliped, not acc instruction and didn't flip another instruction yet
			// checked == zero means it has not been flipped yet
			else if (code[k].checked == 0 && code[k].instruction != 1 && didntFlip)
			{
				// Flip it
				code[k].instruction = code[k].instruction == NOP ? JMP : NOP;
				// Mark it as under-progress/failed flip
				code[k].checked = 1;
				// Mark it flipped something
				didntFlip = false;
			}
		}

		// Check if we failed to terminate execution
		bool fail = false;
		// Instruction index
		int j = 0;
		// Clear isRepeat
		for (int k = 0; k < nlcount; k++) isRepeat[k] = false;
		// Clear accumulator
		acc = 0;
		// Run code
		while (j < nlcount)
		{
			// If instruction is repeating then break
			if (isRepeat[j]) { fail = true; break; };
			// Mark instruction as repeated
			isRepeat[j] = true;
			// Switch for instructions
			switch (code[j].instruction)
			{
				case NOP:
					j++;
					break;
				case ACC:
					acc += code[j].argument;
					j++;
					break;
				case JMP:
					j += code[j].argument;
					break;
				default:
					printf("error: invalid instruction.\n");
					exit(1);
			}
		}
		// If it quitted without failing it means it reached the end
		// of code, so it's a success, whe break the loop and return
		if (!fail) break;
	}

	// print result
	printf("%li\n", acc);

	// free stuff
	free(code);
	free(isRepeat);
	freeinputfile(input);

	return 0;
}
