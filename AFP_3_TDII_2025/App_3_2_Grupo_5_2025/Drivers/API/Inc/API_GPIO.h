/*
 * API_GPIO.h
 *
 *  Created on: Sep 16, 2025
 *      Author: EVE
 */
#ifndef API_GPIO_H_
#define API_GPIO_H_

#include "main.h"      // macros LD1_Pin, LD1_GPIO_Port, USER_Btn_Pin, etc.
#include <stdint.h>
#include <stdbool.h>

#define LED_COUNT 3

typedef enum {
    LED1 = 0,
    LED2,
    LED3
} led_t;

typedef bool buttonStatus_t;

/* Inicialización del módulo GPIO (antes estaba en main.c) */
void MX_GPIO_Init(void);

/* Escribe un LED (true = ON, false = OFF) */
void gpioWrite(led_t led, bool state);

/* Lee el estado del pulsador PA0 (devuelve true si está en SET) */
buttonStatus_t readButton(void);

#endif /* API_GPIO_H_ */
