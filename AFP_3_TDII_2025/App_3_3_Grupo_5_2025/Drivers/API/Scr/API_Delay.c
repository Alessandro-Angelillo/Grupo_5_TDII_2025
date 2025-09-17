/*
 * API_Delay.c
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */

#include "API_Delay.h"
#include "stm32f4xx_hal.h"  // Ajustar según tu MCU

void delayInit(delay_t *delay, uint32_t duration){
    delay->start_time = millis();
    delay->duration = duration;
    delay->running = true;
}

bool delayRead(delay_t *delay){
    if(!delay->running) return true;
    if((millis() - delay->start_time) >= delay->duration){
        delay->running = false;
        return true;
    }
    return false;
}

void delayWrite(delay_t *delay, uint32_t duration){
    delay->duration = duration;
    delay->start_time = millis();
    delay->running = true;
}

uint32_t millis(void){
    return HAL_GetTick();
}
