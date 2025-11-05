/**
 * @file API_GPIO.h
 * @brief Header del driver GPIO para LEDs
 * @date 2 Nov 2025
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

#include "stdint.h"

// SECCIÓN 1: DEFINICIONES PARA MANEJO DE LEDS

typedef uint16_t led_t;

void MX_GPIO_Init(void);
void writeLedOn_GPIO(led_t led);
void writeLedOff_GPIO(led_t led);
void toggleLed_GPIO(led_t led);
uint8_t getLedCount(void);
led_t getLedByIndex(uint8_t index);


// SECCIÓN 2: DEFINICIONES PARA ANTI-REBOTE (BOTÓN)

// Estados posibles del botón
typedef enum {
  BUTTON_RELEASED = 0,
  BUTTON_PRESSED
} button_state_t;

// Inicializa el sistema anti-rebote
void debounceInit_GPIO(void);

// Actualiza el estado del botón (llamar periódicamente)
void debounceUpdate_GPIO(void);

// Devuelve el estado estable del botón
button_state_t readButtonState_GPIO(void);
#endif
