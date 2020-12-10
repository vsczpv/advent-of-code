#include "../../shared-code.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// I guess this won't play very well on 32-bit systems...
// Well so much for code portability
#define BIGGEST 0x7fffffffffffffff

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// ncount holds how many lines/numbers there are
	int ncount = countlines(index);
	// ordnums will hold the numbers in smallest to biggest order
	// nums will hold the unordered numbers
	long int* ordnums =  (long int*) malloc(sizeof(long int)*ncount);
	long int* nums = (long int*) malloc(sizeof(long int)*ncount);
	// iteration stuff
	int len = strlen(index);
	int k = 0;
	int i = 0;
	// Cycle through input
	while (i < len)
	{
		// Get num
		long int num = strtol(&index[i], NULL, 10);
		// Advance to next number
		i = skipspace(1, i, index); i++;
		// Put num into nums
		nums[k] = num; k++;
	}

	k = 0;
	// Cycle through ordnums
	while (k < ncount)
	{
		// Set num to BIGGEST
		// This ensures that the very first value will always
		// be replaced by something from nums
		long int num = BIGGEST;
		// si holds the index of the smallest number
		int si;
		// Cycle through nums
		for (i = 0; i < ncount; i++)
			// If num is bigger than nums[i]
			if (num > nums[i])
			{
				// Set that as smallest number
				num = nums[i];
				// Remember its index
				si = i;
			}

		// Store smallest num into ordnums, increment index
		ordnums[k] = num; k++;
		// Turn said number into BIGGEST in nums, so that
		// it will not be picked again
		nums[si] = BIGGEST;
	}

	// onediff and threediff holds the amount of one/three differences
	// in ordnums. This has to start at 1 because the "outlet" and our "device"
	// are not accounted inside ordnums, but they do count towards the diffs
	int onediff = 1;
	int threediff = 1;
	// cycle through ordnums and increment onediff and threediff respectively
	for (i = 1; i < ncount; i++)
		if ((ordnums[i] - ordnums[i-1]) == 1) onediff++;
		else if ((ordnums[i] - ordnums[i-1]) == 3) threediff++;

	//for (int j = 1; j < ncount; j++) printf("%li +%li\n", ordnums[j], ordnums[j] - ordnums[j-1]);
	// Print result
	printf("%li\n", (long int) onediff * threediff);

	// Free stuff
	free(nums);
	free(ordnums);
	freeinputfile(input);

	return 0;
}
