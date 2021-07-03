#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "shared-code.h"

//#include <assert.h>

// At one moment I ran into overflow problems due to that strage 36-bit width
// Solution? Just slap uint64_t like a madman!

// Mask states
#define MASK_ZERO '0'
#define MASK_ONE '1'
#define MASK_IGN 'X'

// Bit-width
#define BIT_WIDTH (uint64_t) 36

// All-mask
#define ALL_MASK (uint64_t) 0xfffffffff

// Although a 36-bit wide address space is specified,
// values never get stored that high, so we'll just 
// staticly allocate a couple kilobytes.
#define MEM_AMNT 1024 * 100
uint64_t mem[MEM_AMNT] = { 0 };

// Mask
char mask[36] = { MASK_ZERO };

// Memory store
static inline void memstore(uint64_t val, uint64_t addr)
{
	
	// Had to check if my statment about memory size was true
	//assert(addr < MEM_AMNT);
	
	// Apply mask on val and store it
	for (uint64_t i = 0; i < BIT_WIDTH; i++) switch (mask[i])
	{
		case MASK_ZERO:
			val &= ALL_MASK - ((uint64_t) 1 << (BIT_WIDTH-i-1));
			break;
		case MASK_ONE:
			val |= ((uint64_t) 1 << (BIT_WIDTH-i-1));
			break;
		case MASK_IGN:
			break;
	}
	mem[addr] = val;

	return;
}

int main(void)
{
	
	// Getting input file
	inputFile* input = getinputfile("input.txt");
	char* ptr = input->buffer;
	
	while (ptr)
	{
		
		// Backup of ptr for use with mem[X] operations
		char* oldptr = ptr;
		// Get string before "=" and check if "mask "
		// Note the space character there
		// If so, get value after "=" and set the mask
		// by strcpying it.
		// We, here too, compensate for the leading 
		// space character with &v[1].
		// If not, we store the value to memory
		// We get both address and value with strtoll
		if ( !strcmp(strsep(&ptr, "="), "mask ") ) strncpy(mask, &strsep(&ptr, "\n")[1], BIT_WIDTH);
		else if (ptr) memstore(strtol(strsep(&ptr, "\n"), NULL, 10), strtol(&strchr(oldptr, '[')[1], NULL, 10)); 
	}
	
	// Print sum
	uint64_t sum = 0;
	for (uint64_t i = 0; i < MEM_AMNT; i++) sum += mem[i];
	printf("%li\n", sum);
	
	freeinputfile(input);
	return 0;
}
