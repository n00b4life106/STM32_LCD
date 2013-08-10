/*
 *    Created on: 08-08-2013
 *        Author: Michal Ksiezopolski
 * Last modified: 08-08-2013
 */
#include "hd44780.h"
#include "common/string.h"

static	void 	LCD_Write(uint8_t data);
static 	uint8_t LCD_Read();
static 	void 	LCD_DataOut();
static 	void 	LCD_DataIn();
static 	void 	LCD_SendData(uint8_t data);
static	void 	LCD_SendCommand(uint8_t command);
static 	uint8_t LCD_ReadFlag();
static	void 	LCD_SendTextToDevice(LCD_RowTypeDef LCD_Row);

/**
 * Data displayed on the LCD
 */
typedef struct {
	char* row1;			// pointer to data in row
	uint8_t modified1; 	// has data been modified
	uint8_t chars1; 	// number of chars in string
	char* row2;
	uint8_t modified2;
	uint8_t chars2;
} LCD_DataStr;

static LCD_DataStr LCD_Data;



/**
 * Timer for update function.
 */
int LCD_UpdateTimer;

/**
 * Update the LCD
 */
void LCD_Update() {

	if (sysTicks-LCD_UpdateTimer>=LCD_UPDATE_INTERVAL) {
		LCD_UpdateTimer=sysTicks;
		if (LCD_Data.modified1) {
			LCD_SendTextToDevice(LCD_ROW1);
			LCD_Data.modified1=Bit_RESET;
		}
		else if (LCD_Data.modified2) {
			LCD_SendTextToDevice(LCD_ROW2);
			LCD_Data.modified2=Bit_RESET;
		}
	}
}
/**
 * Initialize the display.
 */
void LCD_Init() {
	// Initialize SysTick
	SysTickInit(1000);

	// Wait 50 ms for voltage to settle.
	_delay_ms(50);
	LCD_DataOut();
	RCC_AHBPeriphClockCmd(CLK(LCD_DATA_PORT),ENABLE);
	RCC_AHBPeriphClockCmd(CLK(LCD_CTRL_PORT),ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=(LCD_RS|LCD_RW|LCD_E);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(PORT(LCD_CTRL_PORT),&GPIO_InitStructure);

	//clear all control signals initially
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_RW|LCD_RS|LCD_E);

	//initialization in 4-bit interface
	LCD_Write(0b0011);
	_delay_ms(5);

	LCD_Write(0b0011);
	_delay_ms(1);

	LCD_Write(0b0011);
	_delay_ms(1);

	LCD_Write(0b0010);
	_delay_ms(1);

	// 2 row display
	LCD_SendCommand(LCD_CMD_FUNCTION|LCD_2_ROWS_BIT);
	LCD_SendCommand(LCD_CMD_DISPLAY_ON_OFF|LCD_DISPLAY_ON_BIT|LCD_CURSOR_ON_BIT|LCD_BLINK_ON_BIT);
	LCD_Clear();

}
/**
 * Go to the beginning of the display.
 *
 */
void LCD_Home() {
	LCD_SendCommand(LCD_CMD_HOME);
}
/**
 * Position the LCD at a given memory location.
 */
void LCD_Position(uint8_t position) {
	LCD_SendCommand(LCD_CMD_SET_DDRAM | (position & 0x7f));
}
/**
 * Shifts the LCD display one position to the left
 */
void LCD_Shift() {
	LCD_SendCommand(LCD_CMD_CUR_DISP_SHIFT|LCD_SHIFT_DISPLAY_BIT);
}
/**
 * Clear the display.
 */
void LCD_Clear() {
	LCD_SendCommand(LCD_CMD_CLEAR_DISPLAY);
}
/**
 * Clears a row of the display
 */
void LCD_ClearRow(LCD_RowTypeDef LCD_Row) {
	// print empty string - will be padded with spaces
	LCD_Puts("",LCD_Row);
}
/**
 * Print a character.
 */
void LCD_Putc(char c) {
	LCD_SendData(c);
}
/**
 * Print a string on LCD.
 */
void LCD_Puts(char* s, LCD_RowTypeDef LCD_Row) {

	int len=Strlen(s);
	// check if string isn't too long
	if (len>LCD_CHAR_PER_LINE) {
		len=LCD_CHAR_PER_LINE;
	}

	if (LCD_Row==LCD_ROW1) {
		LCD_Data.row1=s;
		LCD_Data.modified1=Bit_SET;
		LCD_Data.chars1=len;
	}
	else {
		LCD_Data.row2=s;
		LCD_Data.modified2=Bit_SET;
		LCD_Data.chars2=len;
	}
}
/**
 * Send new text to the LCD
 */
static void LCD_SendTextToDevice(LCD_RowTypeDef LCD_Row) {

	LCD_Position(LCD_Row);
	char* s;
	uint8_t chars=0;
	if (LCD_Row==LCD_ROW1) {
		s=LCD_Data.row1;
		chars=LCD_Data.chars1;
	}
	else {
		s=LCD_Data.row2;
		chars=LCD_Data.chars2;
	}

	uint8_t i;
	for (i=0;i<LCD_CHAR_PER_LINE;i++) {
		if (i<chars)
			LCD_Putc(s[i]);
		else {
			LCD_Putc(' '); // fill in rest of line with blanks
		}

	}
}
/**
 * Send data to LCD
 */
static void LCD_SendData(uint8_t data) {

	// rs high and rw low for writing data
	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_RS);
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_RW);

	LCD_DataOut();

	// write higher 4 bits first
	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_E);
	LCD_Write(data>>4);
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_E);

	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_E);
	LCD_Write(data);
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_E);

	// wait until the command gets processed
	while (LCD_ReadFlag());

}
/**
 * Send a command to the LCD.
 */
 static void LCD_SendCommand(uint8_t command) {
	// rs low and rw low for writing command
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_RW|LCD_RS);
	LCD_DataOut();

	// write higher 4 bits first
	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_E);
	LCD_Write(command>>4);
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_E);

	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_E);
	LCD_Write(command);
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_E);

	// wait until the command gets processed
	while (LCD_ReadFlag());


}
/**
 * Read the busy flag.
 */
static uint8_t LCD_ReadFlag() {
	LCD_DataIn();
	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_RS);
	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_RW);

	uint8_t result=0;
	result=(LCD_Read()<<4);
	result|=LCD_Read();
	return result & LCD_BUSY_FLAG;

}
/**
 * Set data lines as output.
 */
static void LCD_DataOut() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=(LCD_D4|LCD_D5|LCD_D6|LCD_D7);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init( PORT(LCD_DATA_PORT) ,&GPIO_InitStructure);
}
/**
 * Set data lines as input with pull up.
 */
static void LCD_DataIn() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=(LCD_D4|LCD_D5|LCD_D6|LCD_D7);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init( PORT(LCD_DATA_PORT) , &GPIO_InitStructure);
}
/**
 * This functions sets data on the data lines when writing.
 */
static void LCD_Write(uint8_t data) {

	if (data & (1<<3))
		GPIO_SetBits(PORT(LCD_DATA_PORT),LCD_D7);
	else
		GPIO_ResetBits(PORT(LCD_DATA_PORT),LCD_D7);
	if (data & (1<<2))
		GPIO_SetBits(PORT(LCD_DATA_PORT),LCD_D6);
	else
		GPIO_ResetBits(PORT(LCD_DATA_PORT),LCD_D6);
	if (data & (1<<1))
		GPIO_SetBits(PORT(LCD_DATA_PORT),LCD_D5);
	else
		GPIO_ResetBits(PORT(LCD_DATA_PORT),LCD_D5);
	if (data & (1<<0))
		GPIO_SetBits(PORT(LCD_DATA_PORT),LCD_D4);
	else
		GPIO_ResetBits(PORT(LCD_DATA_PORT),LCD_D4);

}
/**
 * Reads the data lines
 */
static uint8_t LCD_Read() {
	uint8_t result=0;

	GPIO_SetBits(PORT(LCD_CTRL_PORT),LCD_E);

	if(GPIO_ReadInputDataBit(PORT(LCD_DATA_PORT),LCD_D7))
		result|=(1<<3);
	if(GPIO_ReadInputDataBit(PORT(LCD_DATA_PORT),LCD_D6))
		result|=(1<<2);
	if(GPIO_ReadInputDataBit(PORT(LCD_DATA_PORT),LCD_D5))
		result|=(1<<1);
	if(GPIO_ReadInputDataBit(PORT(LCD_DATA_PORT),LCD_D4))
		result|=(1<<0);

	GPIO_ResetBits(PORT(LCD_CTRL_PORT),LCD_E);
	return result;
}

