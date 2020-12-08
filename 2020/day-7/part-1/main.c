#include "../../shared-code.h"
#include "../../lib/link/include/linkmacro.h"
#include "../../lib/link/include/link.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// WARNING:
// this thing is *slow*
// it takes about 17 minutes to cruch the input file in a pentium G3220
// all thanks to a inneficient recursive search approach
// oh, and it's also single threaded
//
// have fun

// Shallow bag structure
// This structure holds info for bags at the start
// of a rule:
// light red bags contain 1 bright white bag, 2 muted yellow bags.
// ^^^^^^^^^^^^^^
// it also holds a vlink reference to Deep bags
typedef struct sbag
{
	char* type;
	vlink* holds;
} sbag;

// Deep bag structure
// This structure holds info for bags inside other bags
// light red bags contain 1 bright white bag, 2 muted yellow bags.
//                        ^^^^^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^^^^
// Holds both type and amount
typedef struct dbag
{
	char* type;
	int amnt;
} dbag;

// Skip spaces (and also newlines)
int skipspace(int spaces, int i, char* str)
{
	int j = i;
	int count = 0;
	while (count < spaces)
	{
		if (str[j] == '\n') break;
		if (str[j] == ' ') count++; j++;
	}
	return j;
}

// removedummylink removes the "NULL" entry
// at the start of every link
// said null entry is needed as one cannot
// just create a link inside a loop
// but creating one outside a loop means you
// cant also asign it a value
vlink* removedummylink(vlink* entry)
{
	vlink* temp = entry;
	entry = splitlink(entry, LINK_SPLITAFT);
	purgelink(temp);
	return entry;
}

// Recursive function to search for shiny bags
int countshinybags(vlink* entry, bool recursion)
{
	// Result variable
	// In non recursive mode it holds how many
	// shiny bag options it found
	// In recursive mode it acts as a boolean
	// To indicate it found a shiny bag
	int res = 0;

	// Check if recursion or not
	if (recursion)
	{
		// Get current bag type
		sbag* nbag = (sbag*) (entry->data);
		// Cycle through all sub bags
		for (int i = 0; i <= countlink(nbag->holds); i++)
		{
			// Get bag type
			dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, i)->data);
			// Skip no bags, increment if shiny found...
			if (strcmp(nnbag->type, "other bags") == 0) continue;
			if (strcmp(nnbag->type, "shiny gold") == 0) { res++; break; }
			else {
				// Not found, search recursively
				// This works the same as the one below
				for (int j = 0; j <= countlink(entry); j++)
				{
					sbag* lbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, j)->data);
					if (strcmp(lbag->type, nnbag->type) == 0)
						if (countshinybags(seeklink(entry, LINK_FROMFIRST, j), true))
							{ res++; j = countlink(nbag->holds)+1; break; }
				}
			}
		}
		return res;
	} else {
		// Cycle through all bag types
		for (int i = 0; i <= countlink(entry); i++)
		{
			// Get the bag type
			sbag* nbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, i)->data);
			// We don't check our own bag
			if (strcmp(nbag->type, "shiny gold") == 0) continue;

			// Cycle through all sub bags
			for (int j = 0; j <= countlink(nbag->holds); j++)
			{
				// Get the bag type
				dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, j)->data);
				// "other bags" is the equivalent of no bags
				if (strcmp(nnbag->type, "other bags") == 0) continue;
				// Found a shiny bag, increment res and break
				if (strcmp(nnbag->type, "shiny gold") == 0) { res++; break; }
				// Didn't find, search recursively
				else {
					// Cycle through all types of bag
					for (int k = 0; k <= countlink(entry); k++)
					{
						// Get bag type
						// We need to have this third middle man variable
						// because nnbag is a dbag structure, while the
						// bag types (nbag) are sbag.
						sbag* lbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, k)->data);
						// If we found the type we are looking for then do recursion
						if (strcmp(lbag->type, nnbag->type) == 0)
							if (countshinybags(seeklink(entry, LINK_FROMFIRST, k), true))
								// If we find it, increment res, set j to max and break both for loops.
								{ res++; j = countlink(nbag->holds)+1; break; }
					}
				}
			}
			// Print current progress for sanity reasons
			printf("%i %s\n", res, nbag->type);
		}
		// Return result
		return res;
	}
}

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// bagtypes is a linked list that hold all types of bags
	int i = 0;
	int len = strlen(index);
	vlink* bagtypes = createlink(NULL);
	// Cycle through input
	while (i < len)
	{
		// Get end index of bag type
		int j = skipspace(2, i, index);
		// Allocate a sbag structure
		sbag* nbag = (sbag*) malloc(sizeof(sbag));
		// Allocate string space for bag type in nbag
		nbag->type = (char*) malloc(sizeof(char)*(j-i) + 1);
		// Copy bag type into nbag
		strncpy(nbag->type, &index[i], j-i-1);
		// Go to start of first inside bag
		j = skipspace(2, j, index); i = j;
		// Create linked list for inside bags
		nbag->holds = createlink(NULL);
		while (index[i] != '\n')
		{
			// Get amount of bags
			int amnt = strtol(&index[i], NULL, 10);
			// Go to bag type index, advance indexes...
			j = skipspace(1, i, index); i = j;
			j = skipspace(2, i, index);
			// Allocate a dbag structure
			dbag* nnbag = (dbag*) malloc(sizeof(dbag));
			// Allocate string space for bag type in nnbag
			nnbag->type = (char*) malloc(sizeof(char)*(j-i) + 1);
			// Put amount of bags in nnbag
			nnbag->amnt = amnt;
			// Copy bag type into nnbag
			strncpy(nnbag->type, &index[i], j-i-1);
			// Add nnbag into nbag->holds
			addlink(nnbag, nbag->holds, LINK_APPEND);
			// Advance index to next bag/end of line
			i = skipspace(1, j, index);
		}
		// Remove dummy "NULL" link
		nbag->holds = removedummylink(nbag->holds);
		// Add nbag to bag types
		addlink(nbag, bagtypes, LINK_APPEND);
		// Increment index to next line
		i++;
	}
	// Remove dummy "NULL" link
	bagtypes = removedummylink(bagtypes);

	// Count shiny bag possibilities and print result
	int res = countshinybags(bagtypes, false);
	printf("%i\n", res);

	// Free everything correctly
	for (int j = 0; j <= countlink(bagtypes); j++)
	{
		sbag* nbag = (sbag*) (seeklink(bagtypes, LINK_FROMFIRST, j)->data);
		for (int k = 0; k <= countlink(nbag->holds); k++)
		{
			dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, k)->data);
			free(nnbag);
		}
		purgelink(nbag->holds);
		free(nbag);
	}

	// Free even more stuff
	purgelink(bagtypes);
	freeinputfile(input);

	return 0;
}
