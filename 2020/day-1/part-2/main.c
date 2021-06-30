#include "shared-code.h"
#include <stdlib.h>
#include <stdio.h>

// Struct for return values.
typedef struct summulres
{
	long int num1;
	long int num2;
	long int num3;
	long int mul;
} summulres;

// Sum and multiply, takes the number to look for and a struct pointer
void summul(char* input, long int match, summulres* res)
{

	// Set mul to zero to indicate failure in case no number is found.
	res->mul = 0;

	// Create pointer for num1, num2 and num3 in string
	// We also initialize numstr1 here
	char* numstr1 = input;
	char* numstr2;
	char* numstr3;

	// Loop until end
	while (numstr1[1] != '\0')
	{
		// Get num1 and advance pointer
		long int num1 = strtol(numstr1, &numstr1, 10);
		// Set numstr2 to start of string
		numstr2 = input;
		// Loop until end
		while (numstr2[1] != '\0')
		{
			// Get num2 and advance pointer
			long int num2 = strtol(numstr2, &numstr2, 10);
			// Skip if comparing same number
			if (numstr1 == numstr2) continue;
			// Set numstr3 to start of string
			numstr3 = input;
			// Loop until end
			while (numstr3[1] != '\0')
			{
				// Gen num3 and advance pointer
				long int num3 = strtol(numstr3, &numstr3, 10);
				// Skip if comparing same as num1 and num2
				if (numstr3 == numstr1 || numstr3 == numstr2) continue;
				// Get the sum
				long int sum = num1 + num2 + num3;
				// Is it the match?
				if (sum == match)
				{
					// Return the values and multiply it.
					res->num1 = num1;
					res->num2 = num2;
					res->num3 = num3;
					res->mul = num1 * num2 * num3;
				}
			}
		}
	}

	// If no match was found, we'll return with res->mul == 0.
	// We are looking for a nonzero number, so 0 should be fine for a error value.
	return;
}

int main(int argc, char* argv[])
{

	// Getting input file
	inputFile* input = getinputfile("input.txt");

	// Create result structure and get the result for 2020.
	summulres res = {};
	summul(input->buffer, 2020, &res);

	// Check for errors
	if (res.mul == 0) return 1;

	// Print result
	printf("Result is: %li * %li * %li = %li\n", res.num1, res.num2, res.num3, res.mul);

	// Unmap the input file
	freeinputfile(input);

	// Quit.
	return 0;
}
