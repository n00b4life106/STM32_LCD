#include "common.h"

/**
 * System time
 */
volatile int sysTicks;
/**
 * SysTick period in us
 */
static int period;

/**
 * freq- SysTick frequency - should be between 1 (1Hz,1s) and 10^5 (100kHz,10us)
 */
void SysTickInit(int freq) {

	// check value
	if (freq<1)
		freq=1;
	if (freq>100000)
		freq=100000;

	period=1000000/freq; // period in us
	SysTick_Config(F_CPU/freq); // ticks every 1/freq seconds
}
/**
 * Delay function
 */
void _delay_ms(int count) {

	//period greater than 1ms
	if (period>1000)
		return;

	int ticks=sysTicks; // get current number of ticks
	count=count*1000/period; // if period is smaller we have to wait more ticks

	while (sysTicks-ticks<count); // delay

}
/**
 * Counting the time
 */
void SysTick_Handler() {
	sysTicks++;
}
/**
 * Toggle bit function to make it similar to the ones used in the STM library
 */
void GPIO_ToggleBit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin) {
	GPIO_WriteBit(GPIOx,GPIO_Pin,1-GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin));
}
