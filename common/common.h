/*
 *    Created on: 08-08-2013
 *        Author: Michal Ksiezopolski
 * Last modified: 08-08-2013
 */

/**
 * Some common functions and variables
 */

#ifndef COMMON_H_
#define COMMON_H_

/**
 * Add other library headers here
 */
#include "stm32f0_discovery.h"

/**
 * Clock frequency for discovery board
 */
#define F_CPU 48000000

/**
 * Common macros
 */
#define PORT(x) xPORT(x)
#define xPORT(x) (GPIO##x)

#define CLK(x) xCLK(x)
#define xCLK(x) (RCC_AHBPeriph_GPIO##x)

/**
 * System time
 */
extern volatile int sysTicks;
/**
 * freq- SysTick frequency - should be between 1 (1Hz,1s) and 10^5 (100kHz,10us)
 */
void SysTickInit(int freq);
/**
 * Delay function
 */
void _delay_ms(int count);
/**
 * Toggle bit function to make it similar to the ones used in the STM library
 */
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif
