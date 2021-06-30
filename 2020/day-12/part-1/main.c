#include "shared-code.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define FLIGHT_N 0
#define FLIGHT_S 1
#define FLIGHT_E 2
#define FLIGHT_W 3
#define FLIGHT_L 4
#define FLIGHT_R 5
#define FLIGHT_F 6

// Plane structure
// Holds position and angle in radians
typedef struct planeS
{
	double px;
	double py;
	double pa;
} planeS;

// Go north
void inst_n(planeS* plane, long int num)
{
	plane->py -= (double) num;
	return;
}

// Go south
void inst_s(planeS* plane, long int num)
{
	plane->py += (double) num;
	return;
}

// Go east
void inst_e(planeS* plane, long int num)
{
	plane->px += (double) num;
	return;
}

// Go west
void inst_w(planeS* plane, long int num)
{
	plane->px -= (double) num;
	return;
}

// Turn left
void inst_l(planeS* plane, long int num)
{
	plane->pa += ((double) (num) * M_PI / 180.f);
	return;
}

// Turn right
void inst_r(planeS* plane, long int num)
{
	plane->pa -= ((double) (num) * M_PI / 180.f);
	return;
}

// Go forward
void inst_f(planeS* plane, long int num)
{
	plane->px += sin(plane->pa) * (double) num;
	plane->py += cos(plane->pa) * (double) num;
	return;
}

// Function pointer array that holds instructions
static void (*inst[7])(planeS*, long int) = {inst_n, inst_s, inst_e, inst_w, inst_l, inst_r, inst_f};

// Flightpath strucuture
// holds intruction and its value
typedef struct flightpath
{
	short inst;
	long int num;
} flightpath;

int main(int argc, char* argv[])
{

	inputFile* input = getinputfile("input.txt");
	char* index = input->buffer;

	// Plane, angle is initialized to east
	planeS plane = { 0.f, 0.f, M_PI/2.f };

	// Count amount of lines and allocate space for the flightpath
	// based on line amount
	int lcount = countlines(index);
	flightpath* flight = (flightpath*) malloc(sizeof(flightpath)*lcount);

	int len = strlen(index);
	int i = 0;
	int k = 0;
	// Cycle through input file
	while (i < len)
	{
		// Get instruction and value, advance to next line
		char c = index[i]; i++;
		long int num = strtol(&index[i], NULL, 10);
		i = skipspace(1, i, index); i++;

		// Switch case for instruction
		// It populates flight[k] with the respective function pointer index
		switch (c)
		{
			case 'N': // Go north
				flight[k].inst = FLIGHT_N;
				break;
			case 'S': // Go south
				flight[k].inst = FLIGHT_S;
				break;
			case 'E': // Go east
				flight[k].inst = FLIGHT_E;
				break;
			case 'W': // Go west
				flight[k].inst = FLIGHT_W;
				break;
			case 'L': // Turn left
				flight[k].inst = FLIGHT_L;
				break;
			case 'R': // Turn right
				flight[k].inst = FLIGHT_R;
				break;
			case 'F': // Go forward
				flight[k].inst = FLIGHT_F;
				break;
			default:
				perror("error: invalid instruction\n");
		}

		// Populate its number and advance index
		flight[k].num = num; k++;
	}

	// Cycle through flighpath and execute the instructions
	for (k = 0; k < lcount; k++)
		inst[flight[k].inst](&plane, flight[k].num);

	// Print result
	printf("%f\n", fabs(plane.px) + fabs(plane.py));

	free(flight);
	freeinputfile(input);

	return 0;
}
