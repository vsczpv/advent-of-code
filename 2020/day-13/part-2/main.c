#include "../../shared-code.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Again, i struggled with this one
// I think my old brute force approach could actually solve it, but i
// ran into a problem that, i didn't knew if it was broken (only worked with example input)
// or if it was just taking too long to calculate the result
// I think it was the first alternative, as the actual answer wasn't that high (480ish trillion)
// and my code seemed to go through that

// Anyway, i used an approch that i had to multiply the input numbers (complicated story, go to link below)
// https://www.reddit.com/r/adventofcode/comments/kcb3bb/2020_day_13_part_2_can_anyone_tell_my_why_this/

// Bus structure
// Holds bus id and it's distance from last bus id (to skip x'es)
typedef struct bus
{
	long int num;
	int offset;
} bus;

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Get size of file
	int len = strlen(index);
	// Get end of file
	char* endindex = index + len;
	// Skip timestamp value
	strtol(index, &index, 10); index++;

	// Count how many buses there are
	// This starts at 1, as the count is ',' based.
	int ncount = 1;
	int k = 0;
	while (index[k] != '\0')
		// Decrement ncount if x found, as we don't count them
		if (index[k] == 'x') { k++; ncount--; continue; }
		else if (index[k] == ',') { k++; ncount++; }
		else k++;

	// Allocate space for numbers
	k = 0;
	bus* nums = (bus*) malloc(sizeof(bus)*ncount);
	// boffset holds distance between last and current number
	// we need to keep track of this because we do not store the x'es
	int boffset = 0;
	// Get numbers
	while (index < endindex)
	{
		boffset++;
		// Ignore 'x'
		if (index[0] == 'x') index += 2;
		else {
			// Populate number and advance indexes
			nums[k].num = strtol(index, &index, 10);
			nums[k].offset = boffset;
			index++; k++; boffset = 0;
		}
	}

	// Time holds current time
	// Speed holds the speed we increment time
	// (it changes every time we found a streak of buses)
	bool done = false;
	long int time = 0;
	long int speed = 1;
	// Loop while not done
	while (!done)
	{
		// Set timeoffset to time
		long int timeofst = time;

		// Cycle through buses
		for (int i = 0; i < ncount; i++)
		{
			// Check if bus in on schedule, etc...
			if ((timeofst % nums[i].num) != 0) break;
			// Increment timeofst based of next num
			// Doing it like this will probably cause a segfault...
			timeofst += nums[i+1].offset;
			// If we got all buses then break
			if (i == ncount-1) done = true;

			// If we got more than one bus we multiply them togheter
			// to get the new speed, so we can skip useles timestamps
			if (i > 0)
			{
				long int mul = nums[0].num;
				for (int j = 1; j <= i; j++)
				{
					mul *= nums[j].num;
				}
				speed = mul;
			}
		}

		// Advance time
		// (NOTE: not necessarily time travel)
		time += speed;
	}

	// Subtract time from speed, as we incremented it on loop break aswell
	time -= speed;
	// Print result
	printf("time %li\n", time);

	free(nums);
	freeinputfile(input);

	return 0;
}
