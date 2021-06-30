#include "shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Get size of file
	int len = strlen(index);
	// Get end of file
	char* endindex = index + len;
	// Get timestamp value
	long int timestamp = strtol(index, &index, 10); index++;

	// Count how many buses there are
	// This starts at 1, as the count is ',' based.
	// We also decrement each time we find a x to disconsider its coresponding ','.
	// I forgot what those are called lol
	int ncount = 1;
	int k = 0;
	while (index[k] != '\0')
		if (index[k] == 'x') { k++; ncount--; continue; }
		else if (index[k] == ',') { k++; ncount++; }
		else k++;

	// Allocate space for numbers
	k = 0;
	long int* nums = (long int*) malloc(sizeof(long int)*ncount);
	// Get numbers
	while (index < endindex)
	{
		// If 'x' then ignore
		if (index[0] == 'x') { index += 2; continue; }
		// Get number and advance indexes
		nums[k] = strtol(index, &index, 10); index++; k++;
	}

	// Id holds the result bus id
	long int id = 0;
	// time holds the current time were checking
	// we cannot use the original timestamps as we need it
	// to calculate delta time later
	long int time = timestamp;
	while (!id)
	{
		// Cycle through numbers
		for (int i = 0; i < ncount; i++)
			// Bus arrived, get id and break
			if ((time % nums[i]) == 0)
				{ id = nums[i]; break; }
		// Advance time
		// (NOTE: not necessarily time travel)
		time++;
	}

	// We increment time one more than we should
	// as breaking still increments it.
	time--;
	// Calculate time difference
	time = (time - timestamp);
	// Print result
	printf("time %li w/ bus %li: %li\n", time, id, id * time);

	free(nums);
	freeinputfile(input);

	return 0;
}
