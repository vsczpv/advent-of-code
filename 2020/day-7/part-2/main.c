#include "shared-code.h"
#include "linkmacro.h"
#include "link.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// WARNING:
// The better, more descriptive comments are in the part-1 solution
// I didn't want to rewrite the whole thing again y'know
// (and no, i couldn't just copy-paste them)

// Part-2
// Surprisingly enough, the performance problem is gone
// This thing cheeses through the input file in seconds
// If compared with the part-1 counterpart though...
// I don't care! i'll keep the part-1 non-optimized
// as is just fun to watch that thing struggle to parse the input
// (there is also the fact that i have no idea WHAT made this fast...)
// Edit: I now know what the problem is (thanks to u/adrian17 & u/ClimberSeb)
// but i'm to lazy to fix it lol

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

// Count how many bags are inside our shiny shiny bag
// For better explanation cross-refrence this function
// with it's part-1 counterpart.
// I had focus on highlighting the differences between them
// in this commentary, not explaining if from scratch
long int countshinybags(vlink* entry, bool recursion, long int factor)
{
	// Result variable
	// In non recursive mode it holds how many
	// shiny bag options it found
	// In recursive mode it acts as a boolean
	// To indicate it found a shiny bag
	long int res = 0;
	int selfamnt = 1;

	// Check if recursion or not
	if (recursion)
	{
		// Get bag type
		sbag* nbag = (sbag*) (entry->data);
		// Cycle through sub bags
		for (int i = 0; i < countlink(nbag->holds); i++)
		{
			// Get bag type
			dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, i)->data);
			// If this is the end of the line return 1 * factor
			// Factor is the amount of this instace of bags
			// So if we have "13 dark red bags", we have a factor of 13.
			if (strcmp(nnbag->type, "other bags") == 0) {
				//printf("end: %li %li %s\n", res, factor, nbag->type);
				return 1 * factor;
			} else {
				// Cycle through bag types
				for (int j = 0; j < countlink(entry); j++)
				{
					// Convert nnbag from dbag to sbag just like in part-1
					sbag* lbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, j)->data);
					if (strcmp(lbag->type, nnbag->type) != 0) continue;
					// Add the bag total from next recursion to the current one
					// we also have to put in acount that, plus the next recursion's bags
					// this one also puts weight into the count, more specifically it puts
					// one more bag, the bags that holds the other bags, we add that in
					// form of selfamnt. After that we also multiply the factor.
					res += (countshinybags(seeklink(entry, LINK_FROMFIRST, j), true, nnbag->amnt) + selfamnt) * factor;
					// We only want to count this bag once, so we zero out selfamnt
					selfamnt = 0;
					//printf("branch: %li %li %s\n", res, factor, nbag->type);
				}
			}
		}
		// Return current result
		return res;
	} else {
		// Search and get our shiny bag
		sbag* nbag;
		for (int i = 0; i < countlink(entry); i++)
		{
			nbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, i)->data);
			if (strcmp(nbag->type, "shiny gold") == 0) break;
		}
		// Cycle through bags inside this one
		for (int i = 0; i < countlink(nbag->holds); i++)
		{
			// Get the bag
			dbag* nnbag = (dbag*) (seeklink(nbag->holds, LINK_FROMFIRST, i)->data);
			// Cycle through bag types
			for (int j = 0; j < countlink(entry); j++)
			{
				// Convert from dbag to sbag... check stuff...
				sbag* lbag = (sbag*) (seeklink(entry, LINK_FROMFIRST, j)->data);
				if (strcmp(lbag->type, nnbag->type) == 0)
					// Add count from recursion to the first bag
					res += countshinybags(seeklink(entry, LINK_FROMFIRST, j), true, nnbag->amnt);
			}
		}
		// Return final count
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

	// Count bags inside shiny bag and print result
	long int res = countshinybags(bagtypes, false, 1);
	printf("%li\n", res);

	// Free everything correctly
	for (int j = 0; j < countlink(bagtypes); j++)
	{
		sbag* nbag = (sbag*) (seeklink(bagtypes, LINK_FROMFIRST, j)->data);
		for (int k = 0; k < countlink(nbag->holds); k++)
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
