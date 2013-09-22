//degrees to radians : multiply by pi divide by 180
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#define PI 3.14159265


int main(void)
{
	printf("TRIG: the trigonometric calculator\n");
	inputChecking();
}


//THIS IS A FUNCTION TO CHECK THE VALUES ENTERED BY THE USER

inputChecking()
{
//int n = 0;
//while ( n != 1){
	char buf[500] = {0}, trigInputValues[5] = {0}, lowerTrigValues[5] = {0}, trigFuncs[] = {'s','t','c','d','r','h','q',' '}, low[5]={0}, up[5]={0}, *lowPtr, *upPtr;///insert information here
	double lowerUpper[2] = {0};
	int degrees = 0, radians = 0, i, rows = 0;

	printf("Please input request (h-help, q-quit): ");

	fgets(buf, sizeof(buf), stdin);
	sscanf(buf, "%c%c%c%c%c %s %s %i", &trigInputValues[0], &trigInputValues[1], &trigInputValues[2], &trigInputValues[3], &trigInputValues[4], &low[0], &up[0], &rows);

	// Incase of UPPERCASE input, convert the input to lowercase.
	for(i=0; i<sizeof(trigInputValues); i++){
		lowerTrigValues[i] = tolower(trigInputValues[i]);
	}

	if ( lowerTrigValues[0] == 'h' ){ //display help message
		dispHelp();
	}

	if ( lowerTrigValues[0] == 'q' ){ //display goodbye message and quit
		printf("goodbye");
		exit(0);
	}

	// Checking that only Degrees or Radians have been selected (not both).
	for (i=0; i<sizeof(lowerTrigValues); i++){
		if ( lowerTrigValues[i] == 'd')
			degrees = 1;
		if ( lowerTrigValues[i] == 'r')
			radians = 1;
	}

	if ( degrees == 1 && radians == 1 ){
		printf("Error: Illegal input!\n");
		inputChecking();
	}

	if ( degrees == 0 && radians == 0 ){ //If neither are specified, default = radians.
		degrees = 1;
	}

	// Checking that invalid letters have not been entered into the <types> field.
	for (i=0; i<sizeof(lowerTrigValues)/sizeof(lowerTrigValues[0]); i++){
		if ( memchr(trigFuncs, lowerTrigValues[i], sizeof(trigFuncs)) == NULL ){
			printf("Error: Illegal input!\n");
			inputChecking();
		}
		break;
	}


	// Checking only numerical values are entered after the <types> field using strtod()
	// to convert from a string to floats. If it doesn't work then the inputs are not valid.
	lowerUpper[0] = strtod(low, NULL);
 	lowerUpper[1] = strtod(up, NULL);

	if ( lowerUpper[0] == 0 && low != 0 ){
		printf("Error: Illegal input!\n");
		inputChecking();
	}

	if ( lowerUpper[1] == 0 && up != 0 ){
		printf("Error: Illegal input!\n");
		inputChecking();
	}

	// Checking that only a positive integer/numerical value has been entered for the number of intermediate rows.
	if ( isdigit(rows) != 0 || rows < 0 ){
		printf("Error: Illegal input!\n");
		inputChecking();
	}

	// If our inputs have made it past all our error checks, enter into the calculate and print function.
	calculateAndPrint(lowerUpper, lowerTrigValues, rows);

/*
		// ##REMOVE## Loop to print out and check each value of an array
		for (i=0; i<sizeof(lowerCaseValues); i++)
		{printf("%i=%c\n", i, lowerCaseValues[i]);
		}

		// ##REMOVE## print out and check values being recorded
		printf("lower=%f\n", lowerUpper[0]);
		printf("upper=%f\n", lowerUpper[1]);
		printf("rows=%i\n", rows);
*/

}


//This function accepts values and prints out the sin cos tan table
void calculateAndPrint(float lowerUpper[], float lowerTrigValues[], int rows)
{
float i, itSize = 0;
int doSin, doCos, doTan, doRad, doDeg;

//we add 2 to rows because input gives amount of intermediate rows, we want total rows
itSize = (lowerUpper[1]-lowerUpper[0])/(rows+2);

/*
printf("lower=%f\n", lowerUpper[0]);
printf("upper=%f\n", lowerUpper[1]);
printf("rows=%i\n", rows);
printf("it size=%f\n", itSize);
*/

for(i=lowerUpper[0]; i<lowerUpper[1]; i=i+itSize){

	printf("%7.3f,", sin(i));
	printf("%7.3f,", cos(i));
	printf("%7.3f\n",tan(i));
	}
inputChecking();
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
	inputChecking();
}
