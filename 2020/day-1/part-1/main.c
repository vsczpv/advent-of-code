#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

// Struct for return values.
typedef struct summulres
{
	long int num1;
	long int num2;
	long int mul;
} summulres;

// Sum and multiply, takes the number to look for and a struct pointer
void summul(char* input, long int match, summulres* res)
{

	// Set mul to zero to indicate failure in case no number is found.
	res->mul = 0;

	// Create pointer for num1 and num2 in string
	// We also initialize numstr1 here
	char* numstr1 = input;
	char* numstr2;

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
			// Sum them
			long int sum = num1 + num2;
			// Is is the match?
			if (sum == match)
			{
				// Populate results, multiply them and return.
				res->num1 = num1;
				res->num2 = num2;
				res->mul = num1 * num2;
				return;
			};
		}
	}

	// If no match was found, we'll return with res->mul == 0.
	// We are looking for a nonzero number, so 0 should be fine for a error value.
	return;
}

int main(int argc, char* argv[])
{

	// Getting file as memory mapped string
	int fd = open("input.txt", O_RDONLY);

	struct stat st;
	fstat(fd, &st);

	char* input = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	// Create result structure and get the result for 2020.
	summulres res = {};
	summul(input, 2020, &res);

	// Check for errors
	if (res.mul == 0) return 1;

	// Print result
	printf("Result is: %li * %li = %li\n", res.num1, res.num2, res.mul);

	// Unmap the string
	munmap(input, st.st_size);

	// Quit.
	return 0;
}
