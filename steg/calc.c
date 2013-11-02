/* This test file is provided to demonstrate how to use check_bitmap(). */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"

int main(int argc, char* argv[])
{
int a, b, c;
	
	a = argv[1];
	b = argv[2];
	c = a*b;
printf("%i\n", c);

}