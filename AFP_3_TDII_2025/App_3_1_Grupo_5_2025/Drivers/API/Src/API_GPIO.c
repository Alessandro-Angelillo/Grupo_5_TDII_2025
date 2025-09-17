/*
 * API_GPIO.c
 *
 *  Created on: Sep 16, 2025
 *      Author:
 *      stm32f413
 */
#include "API_GPIO.h"

/* Arreglo con los LEDs onboard */
LED leds[NUM_LEDS] = {
    {GPIOB, GPIO_PIN_0},   // LED1 - Verde
    {GPIOB, GPIO_PIN_7},   // LED2 - Azul
    {GPIOB, GPIO_PIN_14}   // LED3 - Rojo
};

/* Implementación de MX_GPIO_Init (copiada desde tu main.c) */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);
}

/* Funciones de control de LEDs */
void GPIO_LED_On(uint8_t index)
{
    if (index < NUM_LEDS) {
        HAL_GPIO_WritePin(leds[index].puerto, leds[index].pin, GPIO_PIN_SET);
    }
}

void GPIO_LED_Off(uint8_t index)
{
    if (index < NUM_LEDS) {
        HAL_GPIO_WritePin(leds[index].puerto, leds[index].pin, GPIO_PIN_RESET);
    }
}
