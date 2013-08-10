#include "string.h"

static char string[10];

uint8_t Strlen(const char* s) {
	uint8_t i=0;
	while (*s++) {
		i++;
	}
	return i;
}
void Strcpy(char* src,char* dest, uint8_t len) {
	register uint8_t i=0;

	for(i=0;i<len;i++)
		dest[i]=src[i];

	dest[i]=0;
}

char* Strrev(char* s) {

	uint8_t imax=Strlen(s);
	// maximum length of string is 10 bytes
	if (imax>10) {
		imax=10;
	}

	uint8_t i,j=0;
	// i is unsigned so it carries over to 0xff
	for (i=imax-1;i!=0xff;i--) {
		string[j]=s[i];
		j++;
	}
	string[imax]='\0';

	return string;
}

uint8_t Strncmp(const char* s1, const char* s2, uint8_t n)
{
    while(n--)
        if(*s1++!=*s2++)
            return *(unsigned char*)(s1 - 1) - *(unsigned char*)(s2 - 1);
    return 0;
}
