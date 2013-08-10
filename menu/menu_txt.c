#include "menu_txt.h"

static char* texts[]={
		"Usun czujnik",
		"Dodaj czujnik"
};

char* getText(StringTypeDef s) {
	return texts[s];
}
