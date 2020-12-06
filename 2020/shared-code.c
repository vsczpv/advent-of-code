#include "shared-code.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

inputFile* getinputfile(char* path)
{
	// Allocate input structure
	inputFile* input = malloc(sizeof(inputFile));

	// Open file
	input->fd = open(path, O_RDONLY);

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

char *strnstr(const char *haystack, const char *needle, size_t len)
{
        int i;
        size_t needle_len;

        if (0 == (needle_len = strnlen(needle, len)))
                return (char *)haystack;

        for (i=0; i<=(int)(len-needle_len); i++)
        {
                if ((haystack[0] == needle[0]) &&
                        (0 == strncmp(haystack, needle, needle_len)))
                        return (char *)haystack;

                haystack++;
        }
        return NULL;
}
