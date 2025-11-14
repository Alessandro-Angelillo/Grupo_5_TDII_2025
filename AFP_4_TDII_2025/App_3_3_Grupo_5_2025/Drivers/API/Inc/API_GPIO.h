/*
 * API_GPIO.h
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */

#ifndef API_INC_API_GPIO_H
#define API_INC_API_GPIO_H

#include "stdint.h" // Para usar uint16_t
#include "stdbool.h" // Para usar bool

// Define el tipo para los pines de los LEDs
typedef uint16_t led_t;

// Define el tipo para el estado del botón
typedef bool buttonStatus_t;

// Prototipos de las funciones del driver
void MX_GPIO_Init(void);
void writeLedOn_GPIO(led_t LDx);
void writeLedOff_GPIO(led_t LDx);
void toggleLed_GPIO(led_t LDx);
buttonStatus_t readButton_GPIO(void);

#endif /* API_INC_API_GPIO_H */
