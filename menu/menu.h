#ifndef MENU_H_
#define MENU_H_
/**
 * Funkcje oraz zmienne odpowiedzialne za dzialanie interaktywnego menu
 */
#include "common/common.h"

/**
 * Czas oczekiwania na miniecie drgan stykow
 */
#define DEBOUNCE_TIME 200

/**
 * Zdarzenie od przyciskow.
 */
void KeyEvent();
/**
 * Inicjalizacja przycisków
 */
void InitButtons();

#endif
