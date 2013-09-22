#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define PI 3.14159265
#define BUF_SIZE 50
#define NUM_BUF_SIZE 10

// the delimiter for the program output 
char delim = ',';
// the decimal format for the program output
char* format = NULL;
// the optional file name to write program output
char* file = NULL;
FILE* outputFile = NULL;

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
void calculateAndPrint(FILE* stream, int doSin, int doCos, int doTan, int useRads, int useDegs, float lower, float upper, int rows)
{
	int totalRows = rows + 2; //we add two because input gives amount of intermediate rows, we want total rows
	float itSize = (upper - lower) / (rows + 1);

	fprintf(stream, "Degrees%cRadians", delim);

	if (doSin) fprintf(stream, "%cSin", delim);
	if (doCos) fprintf(stream, "%cCos", delim);
	if (doTan) fprintf(stream, "%cTan", delim);

	fprintf(stream, "\n");

	float row = lower;
	int i;
	for(i = 0; i < totalRows; ++i) 
	{
		fprintf(stream, format, (useRads ? toDegrees(row) : row));
		fputc(delim, stream);
		fprintf(stream, format, (useDegs ? toRadians(row) : row));
		if (doSin) { fputc(delim, stream); fprintf(stream, format, sin(row)); }
		if (doCos) { fputc(delim, stream); fprintf(stream, format, cos(row)); }
		if (doTan) { fputc(delim, stream); fprintf(stream, format, tan(row)); }

		fprintf(stream, "\n");
		row += itSize;
	}
}

int parseDelim()
{
	char buf[BUF_SIZE] = {0};
	printf("\nPlease enter delimiter ['%c']: ", delim);
	fgets(buf, sizeof(buf), stdin);
	delim = buf[0];
}

int parseFormat()
{
	char buf[BUF_SIZE] = {0};
	printf("\nPlease enter output format ['%s']: ", format);
	fgets(buf, sizeof(buf), stdin);
	// find the \n 
	char* nlp = strchr(buf,'\n');
	int newSize = nlp-buf;
	free(format);
	format = (char*) malloc (newSize);
	strncpy(format, buf, newSize);
}

int parseFile()
{
	char buf[BUF_SIZE] = {0};
	printf("\nPlease enter output file ['%s']: ", file ? file : "");
	fgets(buf, sizeof(buf), stdin);
	// find the \n 
	char* nlp = strchr(buf,'\n');
	int newSize = nlp-buf;
	if (file)
		free(file);
	file = (char*) malloc (newSize);
	strncpy(file, buf, newSize);

	// close the old and open the new file
	if (outputFile)
	{
		fclose(outputFile);
	}

	outputFile = fopen(file, "w");
}

int parseInput(char* buf, int* doSin, int* doCos, int* doTan, int* useRads, int* useDegs, float* operands)
{
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

			case 'F':
			case 'f': parseFormat(); return 2;
			case 'Z':
			case 'z': parseDelim(); return 2; 
			case 'O':
			case 'o': parseFile(); return 2;

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
			case '\n': if(numCnt) { operands[operandCnt++] = strtod(num, NULL); numCnt = 0; memset(num, 0, NUM_BUF_SIZE); } break;
			default:
				illegalInput = 1;
		}

		// break out of the lop on newline to avoid buffer overflow.
		if(buf[i] == '\n')
			break;
	}

	// invlaid chars entered
	if (illegalInput ||
		(*useDegs && *useRads) ||
		(isnan(operands[0]) || isnan(operands[1]) || isnan(operands[2])) ||
		(floor(operands[2]) != operands[2] || operands[2] < 0))
	{
		printf("Error: Illegal input!\n");
		//printf("illegal(%i)\n", illegalInput);
		//printf("op1(%.3f) op2(%.3f) op3(%.3f)\n", operands[0], operands[1], operands[2]);
		//printf("%i\n", floor(operands[2]) != operands[2] || operands[2] < 0);
		return 2;
	}

	return 1;
}

int evaluateBuffer(char* buf)
{
	// variables for user input
	int doSin = 0;
	int doCos = 0;
	int doTan = 0;
	int useRads = 0;
	int useDegs = 0;
	float operands[3] = {NAN,NAN,NAN};

	int result = parseInput(buf, &doSin, &doCos, &doTan, &useRads, &useDegs, operands);
	if (result == 0)
	{	
		return 0;
	}
	else if (result == 1)
	{
		// set up defaults
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

		calculateAndPrint(stdout, doSin, doCos, doTan, useRads, useDegs, operands[0], operands[1], (int)operands[2]);
		if (file)
			calculateAndPrint(outputFile, doSin, doCos, doTan, useRads, useDegs, operands[0], operands[1], (int)operands[2]);
	}

	return 1;
}

void inputChecking()
{
	while(1)
	{
		// read the user input
		char buf[BUF_SIZE] = {0};
		printf("\nPlease input request (h-help, q-quit): ");
		fgets(buf, sizeof(buf), stdin);

		if (!evaluateBuffer(buf))
			break;		
	}
}

int main(int argc, char **argv)
{
	printf("TRIG: the trigonometric calculator\n");

	format = (char*) malloc(5);
	strcpy(format, "%.3f");
	
	if (argc > 1)
	{
		// use the arg buffer if args detected
		char buf[BUF_SIZE] = {0};
		int arg;
		// first arg is the name of the program.
		for (arg = 1; arg < argc; ++arg)
		{
			strcat(buf, argv[arg]);
			strcat(buf, arg == argc - 1 ? "\n" : " ");
		}
		printf("'%s'\n", buf);
		evaluateBuffer(buf);
	}
	else
	{
		inputChecking();	
	}

	free(format);
	if (file)
		free(file);
	if (outputFile)
		fclose(outputFile);
}

