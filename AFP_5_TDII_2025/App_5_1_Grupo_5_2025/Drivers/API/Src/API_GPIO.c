/**
 * @file API_GPIO.c
 * @brief Driver para manejo de LEDs con HAL
 * @date 2 Nov 2025
 * @author Javier
 */

#include "API_GPIO.h"

#include "main.h"

// SECCIÓN 1: VECTOR DE LEDS Y FUNCIONES DE CONTROL

led_t leds[] = {LD1_Pin, LD2_Pin, LD3_Pin};  // Vector de LEDs


void writeLedOn_GPIO(led_t led) {
    HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_SET);
}

void writeLedOff_GPIO(led_t led) {
    HAL_GPIO_WritePin(GPIOB, led, GPIO_PIN_RESET);
}

void toggleLed_GPIO(led_t led) {
    HAL_GPIO_TogglePin(GPIOB, led);
}

uint8_t getLedCount(void) {
    return sizeof(leds) / sizeof(led_t);
}

led_t getLedByIndex(uint8_t index) {
    return leds[index];
}

// SECCIÓN 2: FUNCIONES DE ANTI-REBOTE (DEBOUNCE)

#define DEBOUNCE_DELAY 40  // Tiempo mínimo para considerar pulsación válida

static button_state_t buttonState = BUTTON_RELEASED;
static uint32_t lastDebounceTime = 0;

void debounceInit_GPIO(void) {
  buttonState = BUTTON_RELEASED;
  lastDebounceTime = HAL_GetTick();
}

void debounceUpdate_GPIO(void) {
  static GPIO_PinState lastRead = GPIO_PIN_RESET;
  GPIO_PinState currentRead = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);
  uint32_t now = HAL_GetTick();

  if (currentRead != lastRead) {
    lastDebounceTime = now;
    lastRead = currentRead;
  }

  if ((now - lastDebounceTime) >= DEBOUNCE_DELAY) {
    buttonState = (currentRead == GPIO_PIN_SET) ? BUTTON_PRESSED : BUTTON_RELEASED;
  }
}

button_state_t readButtonState_GPIO(void) {
  return buttonState;
}


// SECCIÓN 3: INICIALIZACIÓN DE GPIO (STM32CubeMX)

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}


// SECCIÓN 4: FUNCIONES DE ERROR Y ASSERT (HAL)

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

