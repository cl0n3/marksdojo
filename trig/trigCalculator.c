#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define PI 3.14159265
#define BUF_SIZE 50
#define NUM_BUF_SIZE 10

float toRadians(float deg)
{
	return deg * PI / 180;
}

float toDegrees(float rad)
{
	return rad * 180 / PI;
}

// This is a function that prints out the help message
void dispHelp(void)
{
	printf("The input format is a single line of letters and numbers comprising\n");
	printf("the following fields:\n");
	printf("\t<types> <start-value> <final-value> <intermediate-rows>\n");
	printf("\n");
	printf("These fields must be input in the order shown above, where <types>\n");
	printf("is a set of letters, and the others are single numbers defining\n");
	printf("the range of computation. The <types> field consists of zero or more\n");
	printf("of the letters from the set <stcdr>, which indicate respectively,\n");
	printf("\n");
	printf("\t(1) The desired trig functions: sin, tan, cos.\n");
	printf("\t(2) Whether to use degrees or radians for input parameters.\n");
	printf("Example input: scr 0 2.3 3\n");
	printf("\n");
}

//This function accepts values and prints out the sin cos tan table
void calculateAndPrint(int doSin, int doCos, int doTan, int useRads, int useDegs, float lower, float upper, int rows)
{
	int totalRows = rows + 2; //we add two because input gives amount of intermediate rows, we want total rows
	float itSize = (upper - lower) / (rows + 1);

	printf("Degrees,Radians");

	if (doSin) printf(",Sin");
	if (doCos) printf(",Cos");
	if (doTan) printf(",Tan");

	printf("\n");

	float row = lower;
	int i;
	for(i = 0; i < totalRows; ++i) 
	{
		printf("%.3f,", (useRads ? toDegrees(row) : row));
		printf("%.3f", (useDegs ? toRadians(row) : row));
		if (doSin) printf(",%.3f", sin(row));
		if (doCos) printf(",%.3f", cos(row));
		if (doTan) printf(",%.3f", tan(row));

		printf("\n");
		row += itSize;
	}
}

int parseInput(int* doSin, int* doCos, int* doTan, int* useRads, int* useDegs, float* operands)
{
	// read the user input
	char buf[BUF_SIZE] = {0};
	printf("Please input request (h-help, q-quit): ");
	fgets(buf, sizeof(buf), stdin);

	// temporary variables needed for parsing
	int illegalInput = 0;
	char num[NUM_BUF_SIZE] = {0};
	int numCnt = 0;
	int operandCnt = 0;
	int i;
	for(i = 0; i < BUF_SIZE; ++i)
	{
		switch(buf[i])
		{
			case 'Q':
			case 'q': return 0;

			case 'H':
			case 'h': dispHelp(); return 2;

			case 'S':
			case 's': *doSin = 1; break;
			case 'T':
			case 't': *doTan = 1; break;
			case 'C':
			case 'c': *doCos = 1; break;

			case 'R':
			case 'r': *useRads = 1; break;
			case 'D':
			case 'd': *useDegs = 1; break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			case '.': num[numCnt++] = buf[i]; break;
			
			case ' ':
			case '\n': if(numCnt) { operands[operandCnt++] = strtod(num, NULL); numCnt = 0; memset(num, 0, sizeof(num)); } break;
			default:
				illegalInput = 1;
		}

		// break out of the lop on newline to avoid buffer overflow.
		if(buf[i] == '\n')
			break;
	}

	// invlaid chars entered
	if (illegalInput ||
		*useDegs && *useRads ||
		(isnan(operands[0]) && isnan(operands[1]) && isnan(operands[2])) ||
		(floor(operands[2]) != operands[2] || operands[2] < 0))
	{
		printf("Error: Illegal input!\n");
		printf("illegal(%i)\n", illegalInput);
		printf("op1(%.3f) op2(%.3f) op3(%.3f)\n", operands[0], operands[1], operands[2]);
		printf("%i\n", floor(operands[2]) != operands[2] || operands[2] < 0);
		return 2;
	}

	return 1;
}

inputChecking()
{	
	int active = 1;
	while(active)
	{
		// variables for user input
		int doSin = 0;
		int doCos = 0;
		int doTan = 0;
		int useRads = 0;
		int useDegs = 0;
		float operands[3] = {NAN};
		// init operands to nan 
		//int i;
		//for (i=0; i<3; ++i) 
		//	operands[i] = NAN;

		int result = parseInput(&doSin, &doCos, &doTan, &useRads, &useDegs, operands);
		if (result == 0)
		{	
			active = 0;
		}
		else if (result == 1)
		{
			// if no operands are set then default to all on. 
			if (!doSin && !doCos && !doTan)
			{
				doSin = 1;
				doCos = 1;
				doTan = 1;
			}

			if (!useDegs && !useRads)
			{
				useDegs = 1;
			}

			// now you can fill complete the calculation and output using the parsed info. 
			// casting is fine here since validation has passed
			calculateAndPrint(doSin, doCos, doTan, useRads, useDegs, operands[0], operands[1], (int)operands[2]);
		}
	}
}

int main(void)
{
	printf("TRIG: the trigonometric calculator\n");
	inputChecking();
}

