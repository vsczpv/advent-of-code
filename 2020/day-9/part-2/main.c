#include "shared-code.h"
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

	// Cycle through input file (after preemble)
	// to find invalid number
	long int inval;
	while (i < len)
	{
		// Get num
		inval = strtol(&index[i], NULL, 10);
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
						if ((magicnums[k] + magicnums[j]) == inval)
						{ isValid = true; j = 25; break; }
					}
		// If not valid then we found it, break;
		if (!isValid) break;
		// Slide magicnumbers upward
		for (int j = 0; j < 24; j++)
			magicnums[j] = magicnums[j+1];
		// Slot in new number into magicnums
		magicnums[24] = inval;
	}

	// Oldi is so we rollback the input the current number+1
	// So if we have a input of: 23 61 34 61
	// First we start at 23, then in the next cycle we rollback to 61, etc..
	// Highn and Lown store the highest and lowest numbers from the sum
	i = 0;
	int oldi = i;
	long int highn;
	long int lown;
	while (i < len)
	{
		// Get num and populate highn and lown
		long int num = strtol(&index[i], NULL, 10);
		// Set sum, lown and highn to num as this is the start of the loop
		highn = num; lown = num;
		long int sum = num;
		// we use found to break this loop
		bool found = false;
		// Advance to next number
		i = skipspace(1, i, index); i++;
		// Cycle through input until the sum is higher than inval
		while (sum < inval)
		{
			// Get the num
			num = strtol(&index[i], NULL, 10);
			// Advance into next num
			i = skipspace(1, i, index); i++;
			// Check lown and highn
			highn = num > highn ? num : highn;
			lown = num < lown ? num : lown;
			// Sum sum
			sum += num;
			// Check if it is the correct result
			if (sum == inval) { found = true; break; }
		}
		// If found then break
		if (found) break;
		// Revert back to start of index + 1 line;
		i = skipspace(1, oldi, index); i++; oldi = i;
	}

	// Print result
	printf("%li\n", lown + highn);

	freeinputfile(input);

	return 0;
}
