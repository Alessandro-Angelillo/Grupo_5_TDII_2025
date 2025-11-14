/*
 * API_Delay.c
 *
 *  Created on: Sep 16, 2025
 *      Author: EVE
 */
#include "API_delay.h"
#include "stm32f4xx_hal.h"

void delayInit(delay_t *delay, uint32_t duration) {
    delay->startTime = HAL_GetTick();
    delay->duration = duration;
    delay->running = true;
}

bool delayRead(delay_t *delay) {
    if (!delay->running) {
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    }

    if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
        delay->running = false;
        return true;
    }

    return false;
}

void delayWrite(delay_t *delay, uint32_t duration) {
    delay->duration = duration;
}

