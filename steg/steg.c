#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "bitmap.h"

#define BitSet(arg,posn) ((arg) | (1L << (posn))) 
#define BitClr(arg,posn) ((arg) & ~(1L << (posn))) 
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn))) 
#define BitTst(arg,posn) (((arg) & (1L << (posn))) != 0)

#define BITS_IN_BYTE 8
#define BITS_IN_INT 32
#define DEBUG 1

/* function to return the amount of bytes in a file */

long amountOfBytes(char *str)
{
	long size;
	FILE *fp = fopen(str, "rb");
	{
	   fseek(fp, 0, SEEK_END);
	   size = ftell(fp);
	   fclose(fp);
	   return(size);
	}
}


/* The encode function takes our input data, merges it with the .bmp and outputs as another .bmp  */

void encode(char *argv[])
{
	
	FILE *fbmp = fopen(argv[1], "rb"); // bmp source file to be corrupted
	FILE *fdat = fopen(argv[2], "rb"); // data file to be inserted
    FILE *fout = fopen(argv[3], "wb"); // output file
    BmpData bdat;
	long corruptionFactorMax, dataSize, sourceSize, encodingOffset; 
	int bit, currentSourceByte, currentDataByte, encodingBit;
	
    if (fbmp == NULL || fout == NULL || fdat == NULL) {
        printf("Error: Could not open files.\n");
        exit(0);
    }

    bdat = check_bitmap(fbmp); // get information about our bitmap
	dataSize = amountOfBytes(argv[2]); // get the amount of bytes of our data file
	sourceSize = bdat.numpixelbytes - BITS_IN_INT;
	encodingOffset = bdat.headersize + BITS_IN_INT;
	encodingBit = 0;

	if (DEBUG)
		printf("sourceSize(%li) dataSize(%li) dataBitSize(%li)\n", 
			sourceSize, dataSize, dataSize * BITS_IN_BYTE);

	corruptionFactorMax = ceil((dataSize * BITS_IN_BYTE)/(bdat.numpixelbytes - BITS_IN_INT)) + 1;

	printf("There was a maximum of %li bits corrupted per byte\n", corruptionFactorMax);

	while (bdat.headersize--) { /* Copy header verbatim */
		int c = fgetc(fbmp);
		fputc(c, fout);
	}
	if (DEBUG)
		printf("header fbmp(%li) fout(%li)\n", ftell(fbmp), ftell(fout));

	// encode the size of the input data in the LSBs of the first 32 bytes of pixel data
	for (bit = 0; bit < sizeof(int) * BITS_IN_BYTE; ++bit)
	{ 
		currentSourceByte = fgetc(fbmp);

		// flip bit if different
		if (BitTst(currentSourceByte, 0) != BitTst(dataSize, bit))
			currentSourceByte = BitFlp(currentSourceByte, 0);

		fputc(currentSourceByte, fout);
	}
	if (DEBUG)	   
		printf("input size fbmp(%li) fout(%li)\n", ftell(fbmp), ftell(fout));
	
	// encode the clear text
	while ((currentDataByte = fgetc(fdat)) != EOF)
	{
		for (bit = 0; bit < BITS_IN_BYTE; ++bit) 
		{
			currentSourceByte = fgetc(fbmp);

			// loop around and big using next order bits for encoding if the end of the file is reached.
			if (currentSourceByte == EOF) {
				fseek(fbmp, encodingOffset, SEEK_SET);
				fseek(fout, encodingOffset, SEEK_SET);
				++encodingBit;
				if (DEBUG)
					printf("starting from beginning of file using encodingBit(%i)\n", encodingBit);
			}

			if (BitTst(currentSourceByte, encodingBit) != BitTst(currentDataByte, bit))
				currentSourceByte = BitFlp(currentSourceByte, encodingBit);
			
			fputc(currentSourceByte, fout);
		}
	}
	if (DEBUG)	   
		printf("input data fbmp(%li) fout(%li)\n", ftell(fbmp), ftell(fout));
	
	//copy the remaining bytes into the output if encoding bit is still 0.
	while (encodingBit == 0 && (currentSourceByte = fgetc(fbmp)) != EOF)
		fputc(currentSourceByte, fout);
	
	if (DEBUG)	   
		printf("total fbmp(%li) fout(%li)\n", ftell(fbmp), ftell(fout));
}
/* The checkForExistingFile function checks to see if the output file already exists, and asks if we want to overwrite*/

void checkForExistingFile(char *str)
{	
	int a = 0;
	
	if (access(str, R_OK) == 0)  //check to see if file exists
	{
		printf("Output file %s already exists. Overwrite (y/n)?\n", str);
	
		while (a == 0)
		{
			char x = 0;
			scanf(" %c", &x);
				
			if (x == 'n')
				exit(0);
				
			else if (x == 'y')
				a = 1;
			
			else
				printf("Please enter y or n:\n");
		}
	}	
}


int main(int argc, char* argv[])
{
	if(argc == 4)
	{
		checkForExistingFile(argv[3]);
		encode(argv);
		//printf("passed");
	}
	
	else if(argc == 3)
	{
		checkForExistingFile(argv[2]);
	//	decode(argv);
		printf("passed");
	}
	
	else	
		{
			printf("Usage: (encode or decode, respectively)\n");
			printf("steg <bmpfile> <datafile> <outputfile>\n");
			printf("steg <bmpfile> <outputfile>\n");
			exit(0);
		}
}

