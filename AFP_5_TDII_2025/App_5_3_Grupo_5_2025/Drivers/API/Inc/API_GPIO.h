#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

// ─────────────────────────────────────────────
// Inclusión de tipos estándar y HAL de STM32
// ─────────────────────────────────────────────
#include "stdint.h"
#include "stm32f4xx_hal.h"

// ─────────────────────────────────────────────
// Definición de tipo para representar un LED
// ─────────────────────────────────────────────
typedef uint16_t led_t;  // Cada LED se identifica por su número de pin

// ─────────────────────────────────────────────
// Prototipos de funciones para manejo de LEDs
// ─────────────────────────────────────────────
void MX_GPIO_Init(void);                   // Inicializa todos los pines GPIO
void writeLedOn_GPIO(led_t led);          // Enciende el LED especificado
void writeLedOff_GPIO(led_t led);         // Apaga el LED especificado
void toggleLed_GPIO(led_t led);           // Cambia el estado del LED (ON/OFF)
uint8_t getLedCount(void);                // Devuelve la cantidad de LEDs disponibles
led_t getLedByIndex(uint8_t index);       // Devuelve el LED según su índice

// ─────────────────────────────────────────────
// Definición del pin del pulsador externo
// ─────────────────────────────────────────────
#define EXT_BTN_Pin GPIO_PIN_5             // Pin físico del pulsador (PA5)
#define EXT_BTN_GPIO_Port GPIOA            // Puerto GPIO correspondiente

// ─────────────────────────────────────────────
// Definición de tipo booleano personalizado
// ─────────────────────────────────────────────
typedef uint8_t bool_t;                    // Tipo booleano para compatibilidad
#define true 1
#define false 0

// ─────────────────────────────────────────────
// Enumeración de estados del pulsador
// ─────────────────────────────────────────────
typedef enum {
  BUTTON_RELEASED = 0,                    // Estado: pulsador no presionado
  BUTTON_PRESSED                          // Estado: pulsador presionado
} button_state_t;

// ─────────────────────────────────────────────
// Prototipos de funciones para anti-rebote
// ─────────────────────────────────────────────
void debounceInit_GPIO(void);             // Inicializa variables internas del anti-rebote
void debounceUpdate_GPIO(void);          // Actualiza el estado del pulsador con lógica de rebote
button_state_t readButtonState_GPIO(void); // Devuelve el estado actual del pulsador
bool_t readKey_GPIO(void);               // Devuelve true si se detectó una pulsación válida

#endif /* API_INC_API_GPIO_H_ */
