/*
 * API_GPIO.h
 *
 *  Created on: Sep 16, 2025
 *      Author: EVE
 */
#ifndef API_GPIO_H
#define API_GPIO_H

#include <stdint.h>
#include "main.h"

typedef struct {
    GPIO_TypeDef* puerto;
    uint16_t pin;
} LED;

/* Cantidad de LEDs y delay (usamos tus defines) */
#define NUM_LEDS 3
#define DELAY_MS 200

/* Inicialización de GPIO */
void MX_GPIO_Init(void);

/* Funciones de la API para LEDs */
void GPIO_LED_On(uint8_t index);
void GPIO_LED_Off(uint8_t index);

#endif // API_GPIO_H

