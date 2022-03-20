#include <stdio.h>
#include <stdlib.h>

int main() {
	int k;
	unsigned char *c;   // '*' means a pointer, name C
                        // unsigned char : is a character...
	double d = 6.0;     // d is a floating number
	// read double by uchar pointer (1 byte)
	c = (unsigned char*) &d;    // '&' is a pointer and means 'the memory located'
	for(k=sizeof(d)-1;k>=0;k--) {
		printf("%02X", c[k]);   // c[k] is the value at c with k shift
		if(k % 4 == 0) printf(" ");
	}
	printf("\n");

//--------------------------------------------------------------------------

	unsigned int  *n;   // n is an unit pointer (4 bytes)
	double v[2] = {6.0, 12e-4}; // 12e-4 means 12 * 10^-4
    // v is a pointer
    // and v[0] = 6.0, v[1] = 12e-4

	// read double by uint pointer (4 bytes)
	n = (unsigned int *) v;
	for(k=sizeof(v)/sizeof(unsigned int)-1;k>=0;k--) {
		printf("%08X ", *(n+k));
	}
	printf("\n");
	
	return 0;
}