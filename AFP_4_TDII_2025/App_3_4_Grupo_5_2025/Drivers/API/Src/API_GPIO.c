/*
 * API_GPIO.c
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */
#include "API_GPIO.h"

// Pines de LEDs
#define LED1_PIN    GPIO_PIN_0   // PB0
#define LED2_PIN    GPIO_PIN_7   // PB7
#define LED3_PIN    GPIO_PIN_14  // PB14
#define LED_PORT    GPIOB

// Pulsador
#define BUTTON_PIN  GPIO_PIN_0
#define BUTTON_PORT GPIOA

void GPIO_Init_Custom(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Habilitar relojes
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configurar LEDs como salida
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configurar pulsador como entrada
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}

void GPIO_ToggleLEDs(void) {
    HAL_GPIO_TogglePin(LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);
}

bool GPIO_ReadButton(void) {
    return (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET);
}
