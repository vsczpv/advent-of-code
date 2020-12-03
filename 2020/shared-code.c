#include "shared-code.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

inputFile* getinputfile(char* path)
{
	// Allocate input structure
	inputFile* input = malloc(sizeof(inputFile));

	// Open file
	input->fd = open("input.txt", O_RDONLY);

	// Get file stat (to know its size)
	fstat(input->fd, &(input->st));

	// Mmap file into memory
	input->buffer = (char*) mmap(NULL, input->st.st_size, PROT_READ, MAP_PRIVATE, input->fd, 0);

	// Return input file
	return input;
}

void freeinputfile(inputFile* input)
{
	// Get rid of the buffer
	munmap(input->buffer, input->st.st_size);
	// Close the file
	close(input->fd);
	// Free the structure
	free(input);

	return;
}
