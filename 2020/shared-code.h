// Shared code between days
// Contains things like loading input files, libraries, etc...

#ifndef SHARED_CODE_H_
#define SHARED_CODE_H_

#include <sys/stat.h>
#include <stddef.h>

// Structure that holds input file info
typedef struct inputFile
{
	char* buffer;
	int fd;
	struct stat st;
} inputFile;

// Load and mmap input file into char array
inputFile* getinputfile(char* path);

// Dispense of file correctly
void freeinputfile(inputFile* input);

// strnstr implementation from stackoverflow
// glibc doesn't have this by default.
// https://stackoverflow.com/questions/23999797/implementing-strnstr
char *strnstr(const char *haystack, const char *needle, size_t len);

#endif // SHARED_CODE_H_
