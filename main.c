/*
 *    Created on: 08-08-2013
 *        Author: Michal Ksiezopolski
 * Last modified: 08-08-2013
 */

/**
 * LCD driver test project
 */

#include "common/common.h"
#include "lcd/hd44780.h"
#include "menu/menu.h"

int timer;
void SendText();
char* tekst[]= {"Litwo ojczyzno mo",
				"ty jestes"};
/**
 *
 */
int main(void) {

	// Initialize SysTick for counting time - done in LCD_Init
	// SysTickInit(1000);

	// Initialize diodes
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	// Initialize button
	STM_EVAL_PBInit(BUTTON_USER,BUTTON_MODE_EXTI);

	//initialize LCD
	LCD_Init();

    while(1) {

    	SendText();
    	LCD_Update();
    	KeyEvent();
    }
}
/**
 * Switches sent text every 1s
 */
void SendText() {
	static uint8_t i;

	if (sysTicks-timer>=1000) {
		timer=sysTicks;
		LCD_Puts(tekst[i],LCD_ROW1);
		STM_EVAL_LEDToggle(LED3);
		i=!i;
	}

}
