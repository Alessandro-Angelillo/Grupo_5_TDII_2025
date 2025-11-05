#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

#include "stdint.h"           // Tipos de datos estándar (uint8_t, uint16_t, etc.)
#include "stm32f4xx_hal.h"    // Biblioteca HAL para STM32F4

// ─────────────────────────────────────────────
// SECCIÓN: Abstracción de LEDs
// ─────────────────────────────────────────────

// Define el tipo led_t como un entero de 16 bits (representa el pin del LED)
typedef uint16_t led_t;

// Inicializa los pines GPIO (LEDs y pulsador externo)
void MX_GPIO_Init(void);

// Enciende el LED especificado
void writeLedOn_GPIO(led_t led);

// Apaga el LED especificado
void writeLedOff_GPIO(led_t led);

// Cambia el estado del LED (ON ↔ OFF)
void toggleLed_GPIO(led_t led);

// Devuelve la cantidad total de LEDs definidos
uint8_t getLedCount(void);

// Devuelve el pin del LED según su índice
led_t getLedByIndex(uint8_t index);

// ─────────────────────────────────────────────
// SECCIÓN: Definición del pulsador externo
// ─────────────────────────────────────────────

// Define el pin físico del pulsador externo (PA5)
#define EXT_BTN_Pin GPIO_PIN_5
#define EXT_BTN_GPIO_Port GPIOA

// ─────────────────────────────────────────────
// SECCIÓN: Anti-rebote
// ─────────────────────────────────────────────

// Define tipo booleano personalizado
typedef uint8_t bool_t;
#define true 1
#define false 0

// Estados posibles del pulsador
typedef enum {
  BUTTON_RELEASED = 0,  // No presionado
  BUTTON_PRESSED        // Presionado
} button_state_t;

// Inicializa el sistema anti-rebote
void debounceInit_GPIO(void);

// Actualiza el estado del pulsador aplicando lógica anti-rebote
void debounceUpdate_GPIO(void);

// Devuelve el estado actual del botón (PRESSED o RELEASED)
button_state_t readButtonState_GPIO(void);

// Devuelve true si se detectó una pulsación válida (una sola vez)
bool_t readKey_GPIO(void);

#endif /* API_INC_API_GPIO_H_ */
