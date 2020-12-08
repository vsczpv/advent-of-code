#include <stdio.h>
#include <stdlib.h>
#include "../include/link.h"

// Returns first link of list
inline vlink* __LINKEDLIST_tofirst(vlink* current)
{
	// Go back while there is a link
	while (current != NULL && current->prev != NULL)
		current = current->prev;
	return current;
}

// Returns last link of list
inline vlink* __LINKEDLIST_tolast(vlink* current)
{
	// Go fowards while there is a link
	while (current != NULL && current->next != NULL)
		current = current->next;
	return current;
}

// Returns link + offset, or NULL
inline vlink* __LINKEDLIST_glydenext(vlink* match, int offset)
{
	// Go foward said offset
	for (int i = 0; i < offset; i++)
	{
		match = match->next;
		// If no such link, then this is the end.
		if (!match) break;
	}
	return match;
}

// Returns link - offset, or NULL
inline vlink* __LINKEDLIST_glydeprev(vlink* match, int offset)
{
	// Go backward said offset
	for (int i = 0; i < offset; i++)
	{
		match = match->prev;
		// If no such link, then this is the end.
		if (!match) break;
	}
	return match;
}

// Purges link list out of memory
void purgelink(vlink* entry)
{
	// Takes first link from list
	vlink* current = __LINKEDLIST_tofirst(entry);

	// Frees it while theres still links remaining
	while (current != NULL)
	{
		vlink* purged = current;
		current = current->next;
		free(purged);
	}

	return;
}

// Creates a brand new link
vlink* createlink(void* data)
{
	// Allocates memory for link
	vlink* chain = malloc(sizeof(vlink));

	// Initializes it.
	chain->data = data;
	chain->next = NULL;
	chain->prev = NULL;

	return chain;
}

// Adds a brand new link into an existing link list
void addlink(void* data, vlink* entry, char mode)
{
	// Makes a new link
	vlink* object = createlink(data);

	// Inserts it
	insertlink(object, entry, mode);

	return;
}

// Inserts a link into another link list
void insertlink(vlink* object, vlink* entry, char mode)
{
	// Selects mode
	switch (mode)
	{
		case 0:	// append
		{
			// Takes the last link and joins it at the end
			vlink* current = __LINKEDLIST_tolast(entry);
			object->prev = current;
			current->next = object;
			break;
		}
		case 1:	// prepend
		{
			// Takes the first link and joins it at the start
			vlink* current = __LINKEDLIST_tofirst(entry);
			object->next = current;
			current->prev = object;
			break;
		}
		case 2:	// insert after
		{
			// Just join it after the entry
			object->next = entry->next;
			object->prev = entry;
			entry->next->prev = object;
			entry->next = object;
			break;
		}
		case 3:	// insert before
		{
			// Just join it before the entry
			object->prev = entry->prev;
			object->next = entry;
			entry->prev->next = object;
			entry->prev = object;
			break;
		}
		default:
			// Give error if invalid mode and quit.
			printf("insertlinkedobj: No such mode.\n");
			exit(1);
	}

	return;
}

// Join two link lists back to back
void joinlink(vlink* joiner, vlink* joinee)
{
	// Taking first and last links from lists
	vlink* joiner_last = __LINKEDLIST_tolast(joiner);
	vlink* joinee_first = __LINKEDLIST_tofirst(joinee);

	// Joins them.
	joiner_last->next = joinee_first;
	joinee_first->prev = joiner_last;

	return;
}

// Seeks a link in a list
vlink* seeklink(vlink* entry, char mode, int offset)
{
	vlink* match;

	switch (mode) {
		case 0:	// from first
		{
			match = __LINKEDLIST_glydenext(__LINKEDLIST_tofirst(entry), offset);
			break;
		}
		case 1:	// from last
		{
			match = __LINKEDLIST_glydeprev(__LINKEDLIST_tolast(entry), offset);
			break;
		}
		case 2:	// from entry after
		{
			match = __LINKEDLIST_glydenext(entry, offset);
			break;
		}
		case 3:	// from entry before
		{
			match = __LINKEDLIST_glydeprev(entry, offset);
			break;
		}
		default:
			printf("seeklink: No such mode.\n");
			exit(1);
	}

	return match;
}

// Split a link into two.
vlink* splitlink(vlink* entry, char mode)
{
	// Creates empty new entry.
	vlink* newentry = NULL;
	switch (mode)
	{
		case 0:	// split after
		{
			// Checks if next link exists
			if (entry && entry->next)
			{
				// Splits it and returns entry to second half.
				entry->next->prev = NULL;
				newentry = entry->next;
				entry->next = NULL;
			}
			break;
		}
		case 1:	// split before
		{
			// Checks if previous link exists
			if (entry && entry->prev)
			{
				// Splits it and returns entry to second half.
				entry->prev->next = NULL;
				newentry = entry->prev;
				entry->prev = NULL;
			}
			break;
		}
		default:
			printf("splitlink: No such mode.\n");
			exit(1);
	}

	// If fail, no case is executed and returns is NULL.
	return newentry;
}

// Cut a link out of another link.
vlink** cutlink(vlink* entry, char mode, int amount)
{
	// Later change to some sort of dynamic allocation
	// (will require garbage collection)
	static vlink* newentries[3] = {NULL, NULL, NULL};

	// Pretty sure you've already got the gist of if from previous functions
	// the only difference here is that we check the entries to avoid
	// segmentation faults

	// We also return an array of link*, as different from other functions,
	// it's hard to keep a reference to all possible generated links, due to
	// this function being able divide a link into three.
	// The first is the cutout, second is the first half, and the third is
	// the second half.

	switch (mode)
	{
		case 0: // Cut from start
		{
			entry = __LINKEDLIST_tofirst(entry);
			vlink* lastlink = __LINKEDLIST_glydenext(entry, amount);
			if (lastlink)
			{
				if (entry->prev)
				{
					entry->prev->next = NULL;
					entry->prev = NULL;
				}
				if (lastlink->next)
				{
					lastlink->next->prev = NULL;
					lastlink->next = NULL;
				}
				newentries[0] = entry;
			}
			break;
		}
		case 1:
		{
			entry = __LINKEDLIST_tolast(entry);
			vlink* lastlink = __LINKEDLIST_glydeprev(entry, amount);
			if (lastlink)
			{
				if (entry->next)
				{
					entry->next->prev = NULL;
					entry->next = NULL;
				}
				if (lastlink->prev)
				{
					lastlink->prev->next = NULL;
					lastlink->prev = NULL;
				}
				newentries[0] = entry;
			}
			break;
		}
		case 2:
		{
			vlink* lastlink = __LINKEDLIST_glydenext(entry, amount);
			if (lastlink)
			{
				if (entry->prev)
				{
					newentries[1] = entry->prev;
					entry->prev->next = NULL;
					entry->prev = NULL;
				}
				if (lastlink->next)
				{
					newentries[2] = lastlink->next;
					lastlink->next->prev = NULL;
					lastlink->next = NULL;
				}
				newentries[0] = entry;
			}
			break;
		}
		case 3:
		{
			vlink* lastlink = __LINKEDLIST_glydeprev(entry, amount);
			if (lastlink)
			{
				if (entry->next)
				{
					newentries[2] = entry->next;
					entry->next->prev = NULL;
					entry->next = NULL;
				}
				if (lastlink->prev)
				{
					newentries[1] = lastlink->prev;
					lastlink->prev->next = NULL;
					lastlink->prev = NULL;
				}
				newentries[0] = entry;
			}
			break;
		}
		default:
			printf("cutlink: No such mode.\n");
			exit(1);
	}

	return newentries;
}

int countlink(vlink* entry)
{
	entry = __LINKEDLIST_tofirst(entry);
	int count = 1;

	while (entry != NULL && entry->next != NULL)
	{
		entry = entry->next;
		count++;
	}

	return count;
}
