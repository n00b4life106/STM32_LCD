/*
 *    Created on: 08-08-2013
 *        Author: Michal Ksiezopolski
 * Last modified: 08-08-2013
 */
#ifndef HD44780_H_
#define HD44780_H_

#include "common/common.h"
/**
 * Pins and ports for LCD
 */
#define LCD_DATA_PORT A
#define LCD_CTRL_PORT B

#define LCD_RS 	GPIO_Pin_13
#define LCD_RW 	GPIO_Pin_14
#define LCD_E 	GPIO_Pin_15

#define LCD_D4 	GPIO_Pin_8
#define LCD_D5 	GPIO_Pin_9
#define LCD_D6 	GPIO_Pin_10
#define LCD_D7 	GPIO_Pin_11

/**
 * Structure for initializing the LCD module
 * @todo implement LCD_InitTypeDef
 */
typedef struct {
	uint8_t show_cursor;
	uint8_t blink_on;

} LCD_InitTypeDef;

/**
 * LCD commands HD44780
 */
#define LCD_CMD_CLEAR_DISPLAY 			0x01

#define LCD_CMD_HOME 					0x02
/**
 * During write cursor (LCD_SHIFT_DISPLAY_EM_BIT=0) or display (LCD_SHIFT_DISPLAY_EM_BIT=1)
 * shifts to right (LCD_INCREMENT_BIT=1) or left.
 */
#define LCD_CMD_ENTRY_MODE 				0x04
	#define LCD_INCREMENT_BIT 			0x02
	#define LCD_SHIFT_DISPLAY_EM_BIT	0x01

#define LCD_CMD_DISPLAY_ON_OFF 			0x08
	#define LCD_DISPLAY_ON_BIT			0x04
	#define LCD_CURSOR_ON_BIT 			0x02
	#define LCD_BLINK_ON_BIT			0x01
/**
 * Moving cursor (LCD_SHIFT_DISPLAY_BIT=0) or display (LCD_SHIFT_DISPLAY_BIT=1)
 * left or right (LCD_SHIFT_RIGHT_BIT=1).
 * Shifting display does not change current DDRAM address.
 */
#define LCD_CMD_CUR_DISP_SHIFT 			0x10
	#define LCD_SHIFT_RIGHT_BIT 		0x04
	#define LCD_SHIFT_DISPLAY_BIT 		0x08

#define LCD_CMD_FUNCTION				0x20
	#define LCD_2_ROWS_BIT	 			0x08
	#define LCD_FORMAT_5x10_BIT 		0x04
	#define LCD_8_BIT	 				0x10

#define LCD_CMD_SET_CGRAM 				0x40

#define LCD_CMD_SET_DDRAM 				0x80
/**
 * Row addresses
 */
#define LCD_ROW1 						0x00
#define LCD_ROW2 						0x40
#define LCD_CHAR_PER_LINE 				28 // number of characters in one line
#define LCD_BUSY_FLAG 					(1<<7)


void LCD_Init();
void LCD_Home();
void LCD_Position(uint8_t position);
void LCD_Clear();
void LCD_ClearRow(uint8_t row);
void LCD_Putc(char c);
void LCD_Puts(char* s);

void LCD_SendCommand(uint8_t command);


#endif
