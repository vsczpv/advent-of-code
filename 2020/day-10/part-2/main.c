#include "../../shared-code.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// cheating goes brrrr

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
	// Cycle through input to get numbers
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
	// Cycle through ordnums to sort them
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

	// ### START OF RANT ###
	// So yeah, I've kinda hit a wall on this one
	// I went to the subreddit and looked up a couple solutions to at least
	// try gasping what the h* I'm supposed todo, and I still dont understand it very well.
	// From what I got, this has something todo with a so called "tribonacci sequence",
	// and I have no idea whats that (something todo with adding numbers togheter I think).
	// The suggestion I got was one that I'm supposed to "count paths" to get all combinations
	// which atleast makes some sense when it comes to get combinations, also seems similar to
	// that tribonacci thing
	// https://www.reddit.com/r/adventofcode/comments/kacdbl/2020_day_10c_part_2_no_clue_how_to_begin/
	// So yeah, i didn't manage to fully solve this puzzle, as i didn't really understand by myself
	// the theory behind it, but the implementation is completely mine, so i'll take a half win.
	// ### END OF RANT ###

	// Now we'll use nums as the array to hold the paths
	// So we clean it.
	for (int j = 0; j < ncount; j++) nums[j] = 0;
	// We also set the first numbers <= 3 to 1 as they always have a path from zero
	for (int j = 0; j < 3; j++) nums[j] = ordnums[j] <= 3 ? 1 : 0;

	// Cycle through first num
	for (i = 0; i < ncount; i++)
	{
		// Cycle through second num
		// It is the first num + 1, up to 4 more numbers, as 5 is the maximum
		// amount of consecutive +1/+3 paths (i think). We also stop if we reached
		// the end to avoid a segfault/garbage data
		for (k = i + 1; k < (i + 4 > ncount ? ncount : i + 4); k++)
		{
			// If said number is reachable we add the current paths to this one
			if (ordnums[k] - ordnums[i] <= 3) nums[k] += nums[i];
		}
	}

	// Print the result (the very last path)
	printf("%li\n", nums[ncount-1]);

	// Free stuff
	free(nums);
	free(ordnums);
	freeinputfile(input);

	return 0;
}
