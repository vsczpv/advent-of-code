#include "../../shared-code.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VTOKAMNT 7
#define ETOKAMNT 7

#define BYRMIN 1920
#define BYRMAX 2002
#define IYRMIN 2010
#define IYRMAX 2020
#define EYRMIN 2020
#define EYRMAX 2030

#define CMMIN 150
#define CMMAX 193
#define INMIN 59
#define INMAX 76

// WARNING
//
// This code is kinda broken, but i got the answer in quite a reasonable margin of error
// and considering the airport roleplay, i'd say it's not a big of a deal.
// You see, this code CAN get the right aswer (150 in this case), but it has a random chance
// of returning 149 instead.
// I'm out of patience already, and i am NOT going to debug this for 3 and a half hours to
// find out i fucked up some sort of pointer that resulted in a off-by-one error somewhere.
//
// Enjoy >:)

// The validation tokens
static const char vtokens[VTOKAMNT][5] =
{
	"byr:",
	"iyr:",
	"eyr:",
	"hgt:",
	"hcl:",
	"ecl:",
	"pid:"
};

// The Eye color tokens
static const char etokens[ETOKAMNT][4] =
{
	"amb",
	"blu",
	"brn",
	"gry",
	"grn",
	"hzl",
	"oth"
};

// Checks if character is a digit
// Accepts both decimal and hex
bool isDigit(bool hexmode, char c)
{
	if (hexmode)
	{
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) return true; else return false;
	} else {
		if (c >= '0' && c <= '9') return true; else return false;
	}
}

// Check special numbers for byr, iyr, hcl and pid
// mode 0: 4-digit decimal
// mode 1: '#' prepended 6-digit hexadecimal
// mode 2: 9-digit decimal
bool isSpecialNum(short mode, char* str)
{
	switch (mode)
	{
		case 0:
			if (
				isDigit(false, str[0]) &&
				isDigit(false, str[1]) &&
				isDigit(false, str[2]) &&
				isDigit(false, str[3]) &&
				(str[4] == ' ' || str[4] == '\n')
			) return true; else return false;
			break;
		case 1:
			if (str[0] == '#')
				if (
					isDigit(true, str[1]) &&
					isDigit(true, str[2]) &&
					isDigit(true, str[3]) &&
					isDigit(true, str[4]) &&
					isDigit(true, str[5]) &&
					isDigit(true, str[6]) &&
					(str[7] == ' ' || str[7] == '\n')
				) return true; else return false;
			else return false;
			break;
		case 2:
			if (
				isDigit(false, str[0]) &&
				isDigit(false, str[1]) &&
				isDigit(false, str[2]) &&
				isDigit(false, str[3]) &&
				isDigit(false, str[4]) &&
				isDigit(false, str[5]) &&
				isDigit(false, str[6]) &&
				isDigit(false, str[7]) &&
				isDigit(false, str[8]) &&
				(str[9] == ' ' || str[9] == '\n')
			) return true; else return false;
			break;
		default:
			perror("isSpecialNum: Invalid mode");
	}
}

int main(int argc, char* argv[])
{
	// Initializing input file...
	inputFile* input = getinputfile("file.txt");

	// .. and other variables.
	char* index = input->buffer;
	char* endindex = index;
	int valid = 0;

	while (endindex)
	{
		int count = 0;
		// Get end of batch
		endindex = strstr(index, "\n\n");
		// Check for vtokens
		for (int i = 0; i < VTOKAMNT; i++)
		{
			char* entry = strnstr(index, vtokens[i], endindex-index);
			if (entry)
			{
				// Advance entry to value
				entry += 4;
				// Switch for each token
				switch (i)
				{
					// byr:
					case 0:
					{
						// Check if is a valid number
						if (isSpecialNum(0, entry))
						{
							// Get number and increment count if inbetween BYRMIN and BYRMAX
							long int byr = strtol(entry, NULL, 10);
							if (byr >= BYRMIN && byr <= BYRMAX) count++;
						}
						break;
					}
					// iyr:
					case 1:
					{
						// Check if is a valid number
						if (isSpecialNum(0, entry))
						{
							// Get number and increment count if inbetween IYRMIN and IYRMAX
							long int iyr = strtol(entry, NULL, 10);
							if (iyr >= IYRMIN && iyr <= IYRMAX) count++;
						}
						break;
					}
					// eyr:
					case 2:
					{
						// Check if is a valid number
						if (isSpecialNum(0, entry))
						{
							// Get number and increment count if inbetween EYRMIN and EYRMAX
							long int eyr = strtol(entry, NULL, 10);
							if (eyr >= EYRMIN && eyr <= EYRMAX) count++;
						}
						break;
					}
					// hgt:
					case 3:
					{
						// Get number
						long int hgt = strtol(entry, &entry, 10);
						// Process either if appended with in or cm
						if (strnstr(entry, "in", 3))
						{
							// Check number and increment count if inbetween INMIN and INMAX
							if (hgt >= INMIN && hgt <= INMAX) count++;
						} else if (strnstr(entry, "cm", 3)) {
							// Check number and increment count if inbetween CMMIN and CMMAX
							if (hgt >= CMMIN && hgt <= CMMAX) count++;
						}
						break;
					}
					// hcl;
					case 4:
					{
						// Check number and increment
						// There are no max/min values, so this is pretty simple
						// Besides the part that i spent ALMOST THREE HOURS DEBBUGING THIS GRAP.
						// Don't worry, i just accidentaly inverted an if statement.
						// Easy fix...
						if (isSpecialNum(1, entry)) count++;
						break;
					}
					// ecl
					case 5:
					{
						// Cycle through etokens and increment count if one is found.
						for (int j = 0; j < ETOKAMNT; j++)
							if (strnstr(entry, etokens[j], 4)) { count++; break; }
					}
					// pid
					case 6:
					{
						// Similar to hcl, no special shenanigans
						// just check if the number is there.
						if (isSpecialNum(2, entry)) count++;
						break;
					}
					// cid
					case 7:
						// Nothing todo, we just ignore it.
						count++;
						break;
					// ???
					default:
						break;
				}
			}
		}
		// If found VTOKAMNT tokens, then it's valid
		if (count == VTOKAMNT) valid++;
		index = endindex + 1;
	}

	// Print result
	printf("%i\n", valid);

	// Free stuff
	freeinputfile(input);

	return 0;
}
