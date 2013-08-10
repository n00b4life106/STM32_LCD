#include "menu.h"
#include "lcd/hd44780.h"

/**
 * Definicje przyciskow
 */
#define OK_BUTTON GPIO_Pin_2
#define BACK_BUTTON GPIO_Pin_3

/**
 * Flagi wcisniecia przyciskow
 */
static char OK_Button;
static char BACK_Button;
static char TOGGLE_Button;
/**
 * Timery do odmierzania czasu drgania stykow
 */
static int timerKey1;

/**
 * Zdarzenie od przyciskow.
 */
void KeyEvent() {
	// sprawdz czy ktorys przycisk wcisniety
	if (TOGGLE_Button) {
		// jezeli drgania stykow minely
		if(sysTicks-timerKey1>=DEBOUNCE_TIME) {

			STM_EVAL_LEDToggle(LED4);
			LCD_ClearRow(LCD_ROW1);
			// zeruj flage wcisniecia
			TOGGLE_Button=0;
			// wyczyszczenie flag, zeby nie przejsc od razu do kolejnego przerwania
			EXTI_ClearFlag(EXTI_Line0);
			NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
			// ponowne wlaczenie przerwania
			NVIC_EnableIRQ(EXTI0_1_IRQn);
		}
	}
}

/**
 * Obsluga przerwania przycisku TOGGLE
 *
 */
void EXTI0_1_IRQHandler() {

	// przycisk toggle
	if (EXTI_GetFlagStatus(EXTI_Line0)) {
		// wylaczenie dalszych przerwan az mina drgania stykow
		NVIC_DisableIRQ(EXTI0_1_IRQn);
		// ustawienie timera do odliczania czasu drgan
		timerKey1=sysTicks;
		// ustawienie flagi wcisniecia przycisku
		TOGGLE_Button=1;
		// wyczyszczenie flagi przerwania
		EXTI_ClearFlag(EXTI_Line0);
	}
}

/**
 * Inicjalizacja przycisków BACK i OK
 */
//void InitButtons() {
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	/* Enable the BUTTON Clock */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//
//	/* Configure Button pin as input */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Pin = OK_BUTTON|BACK_BUTTON;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* Connect Button EXTI Line to Button GPIO Pin */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);
//
//	/* Configure Button EXTI line */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line2;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//
//	/* Enable and set Button EXTI Interrupt to the lowest priority */
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//
//	NVIC_Init(&NVIC_InitStructure);
//
//}
