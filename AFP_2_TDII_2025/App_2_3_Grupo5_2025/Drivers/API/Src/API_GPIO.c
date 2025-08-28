/*
 * API_GPIO.c
 *
 *  Created on: Aug 23, 2025
 *      Author: aless
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
  * @param  port: Puerto GPIO al que pertenece el pin del LED (ej: LD1_GPIO_Port).
  * @param  pin: Pin del LED dentro del puerto (ej: LD1_Pin).
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
void BTN_UpdateDirection(int8_t *direction)
{
    static GPIO_PinState last_button_state = GPIO_PIN_RESET; // estado anterior
    GPIO_PinState button_state;

    // Leer estado actual del botón
    button_state = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);

    // Detectar flanco ascendente (suelto -> presionado)
    if (button_state == GPIO_PIN_SET && last_button_state == GPIO_PIN_RESET)
    {
        *direction = -*direction;  // invertir dirección
        HAL_Delay(50);             // anti-rebote básico
    }

    // Actualizar estado anterior
    last_button_state = button_state;
}

/**
  * @brief  Lee el botón USER y cambia el modo en flanco ascendente (1..4, con wrap).
  * @param  modo: puntero a la variable de modo (1..4).
  * @param  debounce_ms: tiempo de antirrebote en milisegundos.
  * @retval None
  */
void APP3_UpdateMode(uint8_t *modo, uint32_t debounce_ms)
{
    static GPIO_PinState last_button_state = GPIO_PIN_RESET;
    GPIO_PinState button_state = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);

    if (button_state == GPIO_PIN_SET && last_button_state == GPIO_PIN_RESET)
    {
        (*modo)++;
        if (*modo > 4) *modo = 1;
        HAL_Delay(debounce_ms); // anti-rebote simple
    }

    last_button_state = button_state;
}

/**
  * @brief  Modo 1: secuencia tipo App 1.1 con 150 ms.
  * @param  ports: arreglo de puertos de LEDs.
  * @param  pins: arreglo de pines de LEDs.
  * @param  led_count: cantidad de LEDs.
  * @retval None
  */
void APP3_Mode1(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count)
{
    static uint8_t  i = 0;
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_time >= 150)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], GPIO_PIN_RESET); // apagar anterior
        i = (i + 1) % led_count;
        HAL_GPIO_WritePin(ports[i], pins[i], GPIO_PIN_SET);   // encender siguiente
        last_time = current_time;
    }
}

/**
  * @brief  Modo 2: parpadeo simultáneo de todos cada 300 ms.
  * @param  ports: arreglo de puertos de LEDs.
  * @param  pins: arreglo de pines de LEDs.
  * @param  led_count: cantidad de LEDs.
  * @retval None
  */
void APP3_Mode2(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count)
{
    static uint8_t  estado = 0;
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_time >= 300)
    {
        estado = !estado;
        for (uint8_t i = 0; i < led_count; i++)
        {
            HAL_GPIO_WritePin(ports[i], pins[i], estado ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        last_time = current_time;
    }
}

/**
  * @brief  Modo 3: parpadeo independiente por LED (100, 300, 600 ms).
  * @param  ports: arreglo de puertos de LEDs.
  * @param  pins: arreglo de pines de LEDs.
  * @param  led_count: cantidad de LEDs.
  * @retval None
  */
void APP3_Mode3(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count, const uint32_t tiempos[])
{
    static uint32_t last_toggle_time[3] = {0, 0, 0};
    static uint8_t  led_state[3]        = {0, 0, 0};
    uint32_t current_time = HAL_GetTick();

    for (uint8_t i = 0; i < led_count && i < 3; i++)
    {
        if (current_time - last_toggle_time[i] >= tiempos[i])
        {
            led_state[i] = !led_state[i];
            HAL_GPIO_WritePin(ports[i], pins[i], led_state[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
            last_toggle_time[i] = current_time;
        }
    }
}

/**
  * @brief  Modo 4: LD1 y LD3 sincronizados, LD2 inverso, cada 150 ms.
  * @param  ports: arreglo de puertos de LEDs.
  * @param  pins: arreglo de pines de LEDs.
  * @param  led_count: cantidad de LEDs (se esperan al menos 3).
  * @retval None
  */
void APP3_Mode4(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count)
{
    static uint8_t  estado = 0;
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();

    if (led_count < 3) return; // seguridad mínima

    if (current_time - last_time >= 150)
    {
        estado = !estado;

        // LD1 y LD3
        HAL_GPIO_WritePin(ports[0], pins[0], estado ? GPIO_PIN_SET : GPIO_PIN_RESET); // LD1
        HAL_GPIO_WritePin(ports[2], pins[2], estado ? GPIO_PIN_SET : GPIO_PIN_RESET); // LD3

        // LD2 inverso
        HAL_GPIO_WritePin(ports[1], pins[1], estado ? GPIO_PIN_RESET : GPIO_PIN_SET); // LD2

        last_time = current_time;
    }
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


