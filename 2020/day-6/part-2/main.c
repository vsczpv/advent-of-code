#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	int len = strlen(index);
	// yes holds how many answers were yes'd.
	int yes = 0;
	int i = 0;
	while (i < len)
	{
		// answers holds how many times a questions was answered.
		int answers[26] = { 0 };
		// people counts how many people there are in a group.
		// This starts at one because the count is newline based,
		// and the very first person wont have a newline.
		int people = 1;

		// Cycle through group
		while ((index[i] != '\n') || (index[i+1] != '\n'))
		{
			// Get char and increment i.
			char c = index[i]; i++;
			// If string out of range then break.
			if (i > len) break;
			// If newline then continue & increment people.
			// Every new line counts as someone.
			// There is one exception, which is the very last person;
			// the code gets confused and increments it more than once apparently,
			// so we add a check for a null terminator.
			if (c == '\n' && (index[i+1] != '\0')) { people++; continue; }
			// Get character as index from 0-25.
			int j = c - (int) 'a';
			// Increment respective answer.
			answers[j]++;
		}

		// We check if everyone aswered the same question if the.
		// number of aswers in a question matches that of people.
		// This is done in assumtion that everyone answers just once.
		// which is the case for this puzzle input.
		// abcde -> valid.
		// abcad -> corrupted.
		for (int j = 0; j < 26; j++) if (answers[j] == people) yes++;

		// Increment into next group.
		i += 2;
	}

	// Print result.
	printf("%i\n", yes);

	freeinputfile(input);
	return 0;
}
