#ifndef MENU_TXT_H_
#define MENU_TXT_H_
/**
 * String constants
 */

typedef enum {
	tMainMenuDelete,
	tMainMenuAdd,
} StringTypeDef;

char* getText(StringTypeDef s);

#endif
