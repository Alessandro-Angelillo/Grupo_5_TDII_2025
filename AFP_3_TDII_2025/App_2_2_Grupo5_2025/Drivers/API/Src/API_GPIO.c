/*
 * API_GPIO.c
 *
 *  Created on: Aug 23, 2025
 *      Author: Ivan
 */

/**   includes   **/
#include "main.h"
#include "API_GPIO.h"
/*   Defines   */

/*   Declaration of variables   */
//Valores esperados para led_t LDx:
//led_t LDx;

/*   function definitions   */

/**
  * @brief  Enciende y apaga un LED con un retardo definido.
  * @param  port: Puerto GPIO al que pertenece el pin del LED
  * @param  pin: Pin del LED dentro del puerto
  * @param  delay: Tiempo en milisegundos que el LED permanecerá encendido y luego apagado.
  * @retval None
  */
void LED_Blink(GPIO_TypeDef* port, uint16_t pin, uint32_t delay)
{
    // Encender LED
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    HAL_Delay(delay);

    // Apagar LED
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    HAL_Delay(delay);
}

/**
  * @brief  Lee el estado del botón y cambia la dirección si detecta un flanco ascendente.
  * @param  direction: puntero a la variable dirección (1 = adelante, -1 = atrás).
  * @retval None
  */
void BTN_UpdateDirection(int8_t *direction, uint32_t debounce_ms)
{
    static GPIO_PinState last_button_state = GPIO_PIN_RESET; // estado anterior
    GPIO_PinState button_state;

    // Leer estado actual del botón
    button_state = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);

    // Detectar flanco ascendente (suelto -> presionado)
    if (button_state == GPIO_PIN_SET && last_button_state == GPIO_PIN_RESET)
    {
        *direction = -*direction;  // invertir dirección
        HAL_Delay(debounce_ms);             // anti-rebote básico
    }

    // Actualizar estado anterior
    last_button_state = button_state;
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

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


