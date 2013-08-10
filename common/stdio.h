#ifndef STDIO_H_
#define STDIO_H_

#include "inttypes.h"
/**
 * Minimal std library
 */
#define MINIMAL_STD

char* Itoa (uint8_t i, uint8_t base);


#ifndef MINIMAL_STD
/**
 * Integer to chars representing bits
 */
char* avr_itoba(uint8_t i);
uint8_t Atoi(char*);

uint8_t avr_isalpha (char c);
uint8_t avr_isnum (char c);
#endif


#endif /* STDIO_H_ */
