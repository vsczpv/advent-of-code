#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// How much memory to allocate for nums.ptr
#define NUMS_SIZE sizeof(uint64_t)*4096

// When to output result
#define TARGET_I 2020

// Input
// This time the input is quite simple,
// no need for a file
#define INPUT_AMNT 6
int input[6] = {15, 5, 1, 4, 7, 0};

//#define INPUT_AMNT 3
//int input[3] = {0, 3, 6};	// example 1: 436
//int input[3] = {1, 3, 2};	// example 2: 1
//int input[3] = {2, 1, 3};	// example 3: 10
//int input[3] = {1, 2, 3};	// example 4: 27
//int input[3] = {2, 3, 1};	// example 5: 78
//int input[3] = {3, 2, 1};	// example 6: 438
//int input[3] = {3, 1, 2};	// example 7: 1836

int main(void)
{
	
	// Struct to store numbers
	struct nums_struct { uint64_t* ptr; uint32_t amnt; } nums = { .ptr=malloc(NUMS_SIZE), .amnt=1 };
	for (int i = 0; i < INPUT_AMNT; i++) nums.ptr[i] = input[i];
	
	// Cycle through numbers until TARGET_I
	for (int i = INPUT_AMNT; i < TARGET_I; i++)
	{
		// Get first occurrence of last number
		int64_t j; for (j = i-1; j >= 0; j--) if (nums.ptr[j] == nums.ptr[i-1]) break;
		// Calculate distance, save exception
		uint64_t n = j != -1 ? i-j-1 : 0;
		// Calculate second occurance
		for (j--; j >= 0; j--) if (nums.ptr[j] == nums.ptr[i-1]) break;
		// Calculate difference again, result ends up negated
		nums.ptr[i] = -( n-( j != -1 ? i-j-1 : 0) );
	}	

	printf("%li\n", nums.ptr[TARGET_I-1]);
	free(nums.ptr);
	return 0;
}
