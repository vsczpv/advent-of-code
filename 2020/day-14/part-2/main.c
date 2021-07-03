#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "shared-code.h"
#include "linkmacro.h"
#include "link.h"

// Sanity Check
// Uncomment for a progress prompt
// The input should finish at ~590
//#define PROGRESS_PROMPT

// Mask states
#define MASK_ZERO '0'
#define MASK_ONE '1'
#define MASK_IGN 'X'

// Bit-width
#define BIT_WIDTH (uint64_t) 36

// All-mask
#define ALL_MASK (uint64_t) 0xfffffffff

// Memory list
vlink* mem = NULL;

// Memory entry
typedef struct mementry
{
	char addr[36];
	uint64_t val;
} mementry;

// Mask
char mask[BIT_WIDTH] = { MASK_ZERO };

// Decode buffer
// bool checked is used internally by memstore
int decodeamnt = 0;
struct decodebuff_struct { char buff[36]; bool checked; }* decodebuff = NULL;

// Update decode buffer
// We need a buffer to apply the mask into,
// and it changes size everytime a new mask
// is loaded.
// It grows by 2^n, n = number of X'es.
static inline void updatebuff(void)
{
	
	int x = 1;
	for (int i = 0; i < BIT_WIDTH; i++) if (mask[i] == MASK_IGN) x *= 2;
	
	// Potential memory leak
	// Only realloc if x > decodeamnt
	// Shouldn't cause problems
	if (x > decodeamnt) decodebuff = (struct decodebuff_struct*) realloc(decodebuff, sizeof(struct decodebuff_struct)*x);
	decodeamnt = x;
	
	return;
}

// Memory store
static inline void memstore(uint64_t val, uint64_t addr)
{
	// Bitmask to fill in the X'es
	int x = decodeamnt >> 1;
	// Decode mask & apply into addr
	for (int i = 0; i < BIT_WIDTH; i++)
	{
		switch (mask[i])
		{
			// Decode addr into bits and store it
			case MASK_ZERO:
				for (int j = 0; j < decodeamnt; j++) decodebuff[j].buff[i] = addr & ((uint64_t) 1 << (BIT_WIDTH-i-1)) ? MASK_ONE : MASK_ZERO;
				break;
			// Store MASK_ONE
			case MASK_ONE:
				for (int j = 0; j < decodeamnt; j++) decodebuff[j].buff[i] = MASK_ONE;
				break;
			// Store a binary pattern
			// It's here where we fill in the X'es
			// X was set to the amount of decoded addresses >> 1
			// then, it's anded with j, essestially creating this pattern:
			// 000		100
			// 001		101
			// 010		110
			// 011		111
			// Said pattern is then applied to the X'es.
			case MASK_IGN:
				for (int j = 0; j < decodeamnt; j++) decodebuff[j].buff[i] = j & x ? MASK_ONE : MASK_ZERO;
				x >>= 1;
				break;
		}
	}

	// Reset decodebuff[i].checked
	for (int i = 0; i < decodeamnt; i++) decodebuff[i].checked = false;
	
	// Cycle through mem
	vlink* entry = mem;
	while (entry)
	{
		mementry* me = (mementry*) entry->data;
		// If entry already exists then just replace it's value
		for (int i = 0; i < decodeamnt; i++) if (!strncmp(me->addr, decodebuff[i].buff, BIT_WIDTH))
		{
			decodebuff[i].checked = true;
			me->val = val;
		}
		entry = entry->next;
	}
	
	// Create the remaining entries
	for (int i = 0; i < decodeamnt; i++) if (!decodebuff[i].checked) 
	{
		mementry* me = (mementry*) malloc(sizeof(mementry));
		strncpy(me->addr, decodebuff[i].buff, BIT_WIDTH);
		me->val = val;
		addlink(me, mem, LINK_APPEND);
	}

	return;
}

int main(void)
{
	
	// Initializing mem
	mem = createlink(&(mementry) {"placeholder", 0});
	
	// Getting input file
	inputFile* input = getinputfile("input.txt");
	char* ptr = input->buffer;
	
	#ifdef PROGRESS_PROMPT
	int debug = 0;
	#endif
	
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
		if ( !strcmp(strsep(&ptr, "="), "mask ") ) { strncpy(mask, &strsep(&ptr, "\n")[1], BIT_WIDTH); updatebuff(); }
		else if (ptr) memstore(strtol(strsep(&ptr, "\n"), NULL, 10), strtol(&strchr(oldptr, '[')[1], NULL, 10)); 
		#ifdef PROGRESS_PROMPT	
		printf("Progress: %i\n", debug++);
		#endif
	}
	
	// Print sum
	uint64_t sum = 0;
	vlink* entry = mem;
	while (entry) sum += ((mementry*) entry->data)->val, entry = entry->next;
	printf("%li\n", sum);
	
	// Get rid of "placeholder"
	// One could get rid of this by mallocing
	// the thing instead of statically creating
	// it like I did.
	vlink* old = mem;
	mem = splitlink(old, LINK_SPLITAFT);
	purgelink(old);

	// Free entries
	{
		vlink* entry = mem;
		while (entry) { free(entry->data); entry = entry->next; }
	}
	
	// Free everything else
	purgelink(mem);
	free(decodebuff);
	freeinputfile(input);
	return 0;
}
