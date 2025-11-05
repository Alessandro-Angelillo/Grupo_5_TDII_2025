/**
 * @file API_GPIO.c
 * @brief Driver para manejo de LEDs y pulsador externo con anti-rebote
 * @date 3 Nov 2025
 * @author EVE
 */

#include "API_GPIO.h"  // Header del driver GPIO
#include "main.h"      // Definiciones globales del proyecto

#define DEBOUNCE_DELAY 40  // Tiempo mínimo en ms para considerar una pulsación válida

// ─────────────────────────────────────────────
// SECCIÓN: LEDs
// ─────────────────────────────────────────────

// Vector con los pines de los LEDs definidos en el proyecto
led_t leds[] = {LD1_Pin, LD2_Pin, LD3_Pin};

// Enciende el LED especificado
void writeLedOn_GPIO(led_t led) {
  HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_SET);
}

// Apaga el LED especificado
void writeLedOff_GPIO(led_t led) {
  HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_RESET);
}

// Cambia el estado del LED (ON ↔ OFF)
void toggleLed_GPIO(led_t led) {
  HAL_GPIO_TogglePin(GPIOB, led);
}

// Devuelve la cantidad total de LEDs definidos
uint8_t getLedCount(void) {
  return sizeof(leds) / sizeof(led_t);
}

// Devuelve el pin del LED según su índice
led_t getLedByIndex(uint8_t index) {
  return leds[index];
}

// ─────────────────────────────────────────────
// SECCIÓN: Anti-rebote (debounce)
// ─────────────────────────────────────────────

// Variables internas para controlar el estado del pulsador
static button_state_t buttonState = BUTTON_RELEASED;
static button_state_t lastStableState = BUTTON_RELEASED;
static uint32_t lastDebounceTime = 0;
static bool_t keyPressedFlag = false;

// Inicializa el sistema anti-rebote
void debounceInit_GPIO(void) {
  buttonState = BUTTON_RELEASED;
  lastStableState = BUTTON_RELEASED;
  lastDebounceTime = HAL_GetTick();
  keyPressedFlag = false;
}

// Actualiza el estado del pulsador aplicando lógica anti-rebote
void debounceUpdate_GPIO(void) {
  static GPIO_PinState lastRead = GPIO_PIN_RESET;
  GPIO_PinState currentRead = HAL_GPIO_ReadPin(EXT_BTN_GPIO_Port, EXT_BTN_Pin);
  uint32_t now = HAL_GetTick();

  // Si el estado leído cambió, reinicia el temporizador
  if (currentRead != lastRead) {
    lastDebounceTime = now;
    lastRead = currentRead;
  }

  // Si pasó suficiente tiempo, considera el nuevo estado como válido
  if ((now - lastDebounceTime) >= DEBOUNCE_DELAY) {
    button_state_t newState = (currentRead == GPIO_PIN_SET) ? BUTTON_PRESSED : BUTTON_RELEASED;

    // Si el estado cambió, actualiza y marca pulsación si fue PRESSED
    if (newState != lastStableState) {
      if (newState == BUTTON_PRESSED) {
        keyPressedFlag = true;
      }
      lastStableState = newState;
    }

    buttonState = newState;
  }
}

// Devuelve el estado actual del botón (PRESSED o RELEASED)
button_state_t readButtonState_GPIO(void) {
  return buttonState;
}

// Devuelve true si se detectó una pulsación válida (una sola vez)
bool_t readKey_GPIO(void) {
  if (keyPressedFlag) {
    keyPressedFlag = false;
    return true;
  }
  return false;
}

// ─────────────────────────────────────────────
// SECCIÓN: Inicialización de GPIOs
// ─────────────────────────────────────────────

// Inicializa los pines GPIO usados por LEDs y pulsador externo
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Habilita los relojes de los puertos GPIO necesarios
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Establece el estado inicial de los LEDs en apagado
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD2_Pin|LD3_Pin, GPIO_PIN_RESET);

  // Configura los pines de los LEDs como salida digital
  GPIO_InitStruct.Pin = LD1_Pin|LD2_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configura PA5 como entrada digital para el pulsador externo
  GPIO_InitStruct.Pin = EXT_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // o GPIO_NOPULL según tu circuito
  HAL_GPIO_Init(EXT_BTN_GPIO_Port, &GPIO_InitStruct);
}

// ─────────────────────────────────────────────
// SECCIÓN: Manejo de errores
// ─────────────────────────────────────────────

// Esta función se define solo si USE_FULL_ASSERT está habilitado
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  // Tu código opcional para depuración
  // Ejemplo: imprimir por UART el archivo y línea del error
}
#endif /* USE_FULL_ASSERT */

