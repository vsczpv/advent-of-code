#include "../../shared-code.h"
#include <stdio.h>

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");

	freeinputfile(input);

	return 0;
}
