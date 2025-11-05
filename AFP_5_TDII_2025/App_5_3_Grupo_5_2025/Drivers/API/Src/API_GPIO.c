#include "API_GPIO.h"
#include "main.h"

// ─────────────────────────────────────────────
// Tiempo mínimo para considerar estable el estado del pulsador (anti-rebote)
// ─────────────────────────────────────────────
#define DEBOUNCE_DELAY 40  // en milisegundos

// ─────────────────────────────────────────────
// Arreglo con los LEDs disponibles (LD1, LD2, LD3)
// ─────────────────────────────────────────────
led_t leds[] = {LD1_Pin, LD2_Pin, LD3_Pin};

// ─────────────────────────────────────────────
// Variables internas para el sistema de anti-rebote
// ─────────────────────────────────────────────
static button_state_t buttonState = BUTTON_RELEASED;       // Estado actual del botón
static button_state_t lastStableState = BUTTON_RELEASED;   // Último estado estable
static uint32_t lastDebounceTime = 0;                      // Tiempo de última transición
static bool_t keyPressedFlag = false;                      // Flag para detectar pulsación válida

// ─────────────────────────────────────────────
// Enciende el LED especificado
// ─────────────────────────────────────────────
void writeLedOn_GPIO(led_t led) {
  HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_SET);
}

// ─────────────────────────────────────────────
// Apaga el LED especificado
// ─────────────────────────────────────────────
void writeLedOff_GPIO(led_t led) {
  HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_RESET);
}

// ─────────────────────────────────────────────
// Cambia el estado del LED (ON ↔ OFF)
// ─────────────────────────────────────────────
void toggleLed_GPIO(led_t led) {
  HAL_GPIO_TogglePin(GPIOB, led);
}

// ─────────────────────────────────────────────
// Devuelve la cantidad de LEDs definidos
// ─────────────────────────────────────────────
uint8_t getLedCount(void) {
  return sizeof(leds) / sizeof(led_t);
}

// ─────────────────────────────────────────────
// Devuelve el LED correspondiente al índice dado
// ─────────────────────────────────────────────
led_t getLedByIndex(uint8_t index) {
  return leds[index];
}

// ─────────────────────────────────────────────
// Inicializa el sistema de anti-rebote
// ─────────────────────────────────────────────
void debounceInit_GPIO(void) {
  buttonState = BUTTON_RELEASED;
  lastStableState = BUTTON_RELEASED;
  lastDebounceTime = HAL_GetTick();
  keyPressedFlag = false;
}

// ─────────────────────────────────────────────
// Actualiza el estado del pulsador con lógica de anti-rebote
// ─────────────────────────────────────────────
void debounceUpdate_GPIO(void) {
  GPIO_PinState currentRead = HAL_GPIO_ReadPin(EXT_BTN_GPIO_Port, EXT_BTN_Pin);  // Lectura actual del pin
  uint32_t now = HAL_GetTick();  // Tiempo actual

  static GPIO_PinState lastRead = GPIO_PIN_SET;  // Última lectura del pin

  // Si el estado cambió, reiniciar temporizador de rebote
  if (currentRead != lastRead) {
    lastDebounceTime = now;
    lastRead = currentRead;
  }

  // Si el estado se mantuvo estable por suficiente tiempo
  if ((now - lastDebounceTime) >= DEBOUNCE_DELAY) {
    button_state_t newState = (currentRead == GPIO_PIN_RESET) ? BUTTON_PRESSED : BUTTON_RELEASED;

    // Si el nuevo estado es diferente al último estado estable
    if (newState != lastStableState) {
      if (newState == BUTTON_PRESSED) {
        keyPressedFlag = true;  // Se detectó una pulsación válida
      }
      lastStableState = newState;  // Actualizar estado estable
    }

    buttonState = newState;  // Actualizar estado actual
  }
}

// ─────────────────────────────────────────────
// Devuelve el estado actual del pulsador (presionado o no)
// ─────────────────────────────────────────────
button_state_t readButtonState_GPIO(void) {
  return buttonState;
}

// ─────────────────────────────────────────────
// Devuelve true si se detectó una pulsación válida
// (una sola vez por pulsación)
// ─────────────────────────────────────────────
bool_t readKey_GPIO(void) {
  if (keyPressedFlag) {
    keyPressedFlag = false;  // Limpiar flag para próxima detección
    return true;
  }
  return false;
}

// ─────────────────────────────────────────────
// Inicializa todos los pines GPIO utilizados
// ─────────────────────────────────────────────
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Habilitación de los relojes para todos los puertos GPIO utilizados
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Inicialización de LEDs: LD1, LD2, LD3 en estado apagado
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  // Configuración de los pines de salida para los LEDs
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configuración del pin del pulsador externo (PA5) como entrada con pull-up
  GPIO_InitStruct.Pin = EXT_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(EXT_BTN_GPIO_Port, &GPIO_InitStruct);
}

