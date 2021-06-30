#include <stdio.h>
#include "../include/link.h"
#include "../include/linkutil.h"

void printintlink(vlink* entry)
{
	vlink* current = __LINKEDLIST_tofirst(entry);

	while (1)
	{
		printf("%li", (long int) current->data);
		current = current->next;
		if (current != NULL)
			printf(", ");
		else
			break;
	}

	printf("\n");
	return;
}

void printstrlink(vlink* entry)
{
	vlink* current = __LINKEDLIST_tofirst(entry);

	while (current != NULL)
	{
		printf("%s\n", (char*) current->data);
		current = current->next;
	}

	return;
}
