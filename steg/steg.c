#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "bitmap.h"

#define BitSet(arg,posn) ((arg) | (1L << (posn))) 
#define BitClr(arg,posn) ((arg) & ~(1L << (posn))) 
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn))) 
#define BitTst(arg,posn) ((arg) & (1L << (posn)))

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
	long corruptionFactor, dataSize, tempDataSize; 
	
    if (fbmp == NULL || fout == NULL || fdat == NULL) {
        printf("Error: Could not open files.\n");
        exit(0);
    }

    bdat = check_bitmap(fbmp); // get information about our bitmap

	dataSize = amountOfBytes(argv[2]); // get the amount of bytes of our data file
	
	//temporary stuff to help with debugging:
	
	printf("bytes to store in = %i\n", bdat.numpixelbytes);
	printf("dataByteSize = %li\n", dataSize);
	printf("dataBitSize = %li\n", dataSize*8);
	corruptionFactor = (dataSize * 8)/(bdat.numpixelbytes-32);
	printf("corruption factor = %li\n", corruptionFactor);
	printf("headersize = %u\n", bdat.headersize);
	printf("beginning of pixel data = %u\n", (bdat.headersize + 32));
	



	while (bdat.headersize--) { /* Copy header verbatim */
		int c = fgetc(fbmp);
		fputc(c, fout);
	}
	
	//debugging
	printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
	printf("position of pointer of fout is at:%li\n", ftell(fout));
	


	//insert the number of bytes of the data size into the first 32 bytes of pixel data as a Big Endian binary number 
	
	tempDataSize = dataSize;
	while (tempDataSize != 0) 
	{ 
		int c = fgetc(fbmp);
		if (fmod(tempDataSize, 2)) // if result > 0 we need a 1

			if (BitTst(c,0) == 1)
				fputc(c, fout);
			else
				fputc(BitSet(c,0), fout);

		else // result is an even number, we need a 0

			if (BitTst(c,0) == 0)
				fputc(c, fout);
			else
				fputc(BitClr(c,0), fout);

			tempDataSize = floor(tempDataSize/2);						
	}
	
	//debugging
	printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
	printf("position of pointer of fout is at:%li\n", ftell(fout));
	
	fseek( fbmp, 86, SEEK_SET );
	fseek( fout, 86, SEEK_SET );

	//debugging
	printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
	printf("position of pointer of fout is at:%li\n", ftell(fout));

		
	// get a character of our data to be encoded. Iterate through each bit of each byte and place it in outgoing file.
	int i, j, index = 0;
	for ( i = 0; i < dataSize; i++ )
	{
					int c = fgetc(fbmp);
					
		for ( j = 0; j < 7; j++ ) // a loop to iterate through each bit of byte d (input data)
		{
			//debugging
			printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
			printf("position of pointer of fout is at:%li\n", ftell(fout));		
			
			int d = fgetc(fdat);

			
			if (BitTst(c,index) == BitTst(d,j)){
				fputc(c, fout);
			}

			if ((BitTst(c,index) == 1) && (BitTst(d,j) == 0)){
				fputc(BitClr(c,0), fout);
			}
			
			if ((BitTst(c,index) == 0) && (BitTst(d,j) == 1)){
				fputc(BitSet(c,0), fout);
			}
				
			if (c == EOF){
				index += 1;
				fseek( fbmp, (86), SEEK_SET );
				fseek( fdat, (86), SEEK_SET );
			}
		}
	}


	//debugging
	printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
	printf("position of pointer of fout is at:%li\n", ftell(fout));
	
	//after inserting our data, copy the leftover bytes into the output.

    while (bdat.numpixelbytes--) { //copy the rest of the pixels
        int c = fgetc(fbmp);
        assert(c != EOF);
        fputc(c, fout); 
	}
		   
	printf("position of pointer of fbmp is at:%li\n", ftell(fbmp));
	printf("position of pointer of fout is at:%li\n", ftell(fout));
    
	
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

