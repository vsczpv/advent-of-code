#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Array that holds the numbers to add together
	static long int magicnums[25] = { 0 };

	// Pretty standard procedure by now considering
	// I've done this same type of loop for every
	// solution so far.
	int len = strlen(index);
	int i = 0;
	int k = 0;
	// Add preemble to magicnums
	while (true)
	{
		// Get number
		long int num = strtol(&index[i], NULL, 10);
		// Advance to next number
		i = skipspace(1, i, index); i++;
		// Add num to array
		magicnums[k] = num; k++;
		// If we have all 25 then break
		if (k >= 25) break;
	}

	// Cycly through input file (after preemble)
	long int num;
	while (i < len)
	{
		// Get num
		num = strtol(&index[i], NULL, 10);
		// Advance to next number
		i = skipspace(1, i, index); i++;
		// isValid holds if it is valid or not
		bool isValid = false;
		// Cycle through magicnums twice
		for (int j = 0; j < 25; j++)
			for (k = 0; k < 25; k++)
					// If we are not checking the same number
					if (j != k)
					{
						// If the sum of them == num then mark valid
						// and break both loops
						if ((magicnums[k] + magicnums[j]) == num)
						{ isValid = true; j = 25; break; }
					}
		// If not valid then we found it, break;
		if (!isValid) break;
		// Slide magicnumbers upward
		for (int j = 0; j < 24; j++)
			magicnums[j] = magicnums[j+1];
		// Slot in new number into magicnums
		magicnums[24] = num;
	}

	// Print result
	printf("%li\n", num);

	freeinputfile(input);

	return 0;
}
