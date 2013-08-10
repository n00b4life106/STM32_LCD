
#ifndef STRING_H_
#define STRING_H_

#include "common.h"

uint8_t Strlen(const char* s);
char* Strrev(char* s);
uint8_t Strncmp(const char *, const char *, uint8_t);
void Strcpy(char* src,char* dest, uint8_t len);

#endif /* STRING_H_ */
