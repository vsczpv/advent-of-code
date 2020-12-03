// Shared code between days
// Contains things like loading input files, etc...

#ifndef SHARED_CODE_H_
#define SHARED_CODE_H_

#include <sys/stat.h>

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

#endif // SHARED_CODE_H_
