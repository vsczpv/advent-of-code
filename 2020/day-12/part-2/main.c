#include "shared-code.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Macros for looks
#define FLIGHT_N 0
#define FLIGHT_S 1
#define FLIGHT_E 2
#define FLIGHT_W 3
#define FLIGHT_L 4
#define FLIGHT_R 5
#define FLIGHT_F 6

// Plane structure
// Holds plane and waypoint position
typedef struct planeS
{
	double px;
	double py;
	double wx;
	double wy;
} planeS;

// Move waypoint north
void inst_n(planeS* plane, long int num)
{
	plane->wy -= (double) num;
	return;
}

// Move waypoint south
void inst_s(planeS* plane, long int num)
{
	plane->wy += (double) num;
	return;
}

// Move waypoint east
void inst_e(planeS* plane, long int num)
{
	plane->wx += (double) num;
	return;
}

// Move waypoint west
void inst_w(planeS* plane, long int num)
{
	plane->wx -= (double) num;
	return;
}

// Rotate left
void inst_l(planeS* plane, long int num)
{
	// Turn num into radians
	double nangle = ((double) (num) * M_PI / 180.f);
	// Get angle of between waypoint from plane
	double wangle = atan2(plane->wy-plane->py, plane->wx-plane->px);
	// Get the distance between the waypoint and plane
	double distance = sqrt(pow(plane->wx-plane->px, 2) + pow(plane->wy-plane->py, 2));
	// Rotate the waypoint
	wangle -= nangle;
	// Apply rotation
	plane->wy = sin(wangle)*distance + plane->py;
	plane->wx = cos(wangle)*distance + plane->px;
	return;
}

// Rotate right
void inst_r(planeS* plane, long int num)
{
	// Turn num into radians
	double nangle = ((double) (num) * M_PI / 180.f);
	// Get angle of between waypoint from plane
	double wangle = atan2(plane->wy-plane->py, plane->wx-plane->px);
	// Get the distance between the waypoint and plane
	double distance = sqrt(pow(plane->wx-plane->px, 2) + pow(plane->wy-plane->py, 2));
	// Rotate the waypoint
	wangle += nangle;
	// Apply rotation
	plane->wy = sin(wangle)*distance + plane->py;
	plane->wx = cos(wangle)*distance + plane->px;
	return;
}

// Go to waypoint
void inst_f(planeS* plane, long int num)
{
	// Get difference
	double vx = plane->wx - plane->px;
	double vy = plane->wy - plane->py;
	// Do it for num times
	for (int i = 0; i < num; i++)
	{
		// Fly that plane
		plane->px += vx;
		plane->py += vy;
	}
	// Rellocate waypoint
	plane->wx = plane->px + vx;
	plane->wy = plane->py + vy;

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

	// Populate plane
	// waypoint starts at 10, -1
	planeS plane = { 0.f, 0.f, 10.f, -1.f };

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
			case 'N': // Move waypoint north
				flight[k].inst = FLIGHT_N;
				break;
			case 'S': // Move waypoint south
				flight[k].inst = FLIGHT_S;
				break;
			case 'E': // Move waypoint  east
				flight[k].inst = FLIGHT_E;
				break;
			case 'W': // Move waypoint  west
				flight[k].inst = FLIGHT_W;
				break;
			case 'L': // Rotate waypoint left
				flight[k].inst = FLIGHT_L;
				break;
			case 'R': // Rotate waypoint right
				flight[k].inst = FLIGHT_R;
				break;
			case 'F': // Go to waypoint
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
		inst[flight[k].inst](&plane, flight[k].num);;

	// Print result
	printf("%f\n", fabs(plane.px) + fabs(plane.py));

	free(flight);
	freeinputfile(input);

	return 0;
}
