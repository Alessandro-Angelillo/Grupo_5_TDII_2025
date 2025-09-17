/*
 * API_GPIO.h
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */

#ifndef API_GPIO_H
#define API_GPIO_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// Inicialización de LEDs y botón
void GPIO_Init_Custom(void);

// Función para alternar los 3 LEDs a la vez
void GPIO_ToggleLEDs(void);

// Función para leer el botón (true si está presionado)
bool GPIO_ReadButton(void);

#endif
