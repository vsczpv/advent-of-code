#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Yes holds how many times the questions were answered
	int len = strlen(index);
	int yes = 0;
	int i = 0;
	while (i < len)
	{
		// isDuplicate holds if a question was answered or not
		// This is reset each new groupt
		bool isDuplicate[26] = { false };

		// Cycle through group
		while ((index[i] != '\n') || (index[i+1] != '\n'))
		{
			// Get character and increment i
			char c = index[i]; i++;
			// If we got past the end of string then break
			if (i > len) break;
			// If this is a newline char skip.
			if (c == '\n') continue;
			// Get character as a number from 0-25
			int j = c - (int) 'a';
			// Feed character as index of isDuplicate, mark it as answered
			isDuplicate[j] = true;
		}

		// Increment yes for each answered question
		for (int j = 0; j < 26; j++) if (isDuplicate[j]) yes++;

		// Increment i to go to next group
		i += 2;
	}

	// Print result
	printf("%i\n", yes);

	freeinputfile(input);
	return 0;
}
