
#include "stdio.h"
#include "string.h"

/**
 * Global variable for holding string results.
 * Will take 9 bytes in SRAM - enough for one byte when
 * using base 2 - 8 bits + '/0'
 */
static char string[9];

char* Itoa(uint8_t val, uint8_t base) {

	// if value sent is zero, out task is done
	if (val==0) {
		string[0]='0';
		string[1]='\0';
		return string;
	}

	uint8_t i=0;
	uint8_t digit;
	// we take max eight digits for binary data
	while (val>0 && i<8) {
		digit=val%base;
		val=val/base;

		// Adjust for hex
		if (digit>9) {
			digit+='A'-'9'+1;
		}
		string[i]='0'+digit;
		i++;
	}
	string[i]='\0';

	// reverse the string
	return Strrev(string);
}
