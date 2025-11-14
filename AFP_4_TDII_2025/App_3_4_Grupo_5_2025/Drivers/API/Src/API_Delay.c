/*
 * API_Delay.c
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */

#include "API_Delay.h"

/* Inicializa el delay con un tiempo en ms */
void delayInit(delay_t *delay, uint32_t duration) {
    delay->startTime = 0;
    delay->duration = duration;
    delay->running = 0;
}

/* Retorna 1 si el tiempo se cumplió, 0 si todavía no */
uint8_t delayRead(delay_t *delay) {
    uint32_t now = HAL_GetTick();

    if (!delay->running) {
        delay->startTime = now;
        delay->running = 1;
        return 0;
    }

    if ((now - delay->startTime) >= delay->duration) {
        delay->running = 0;  // Se reinicia automáticamente
        return 1;
    }

    return 0;
}

/* Cambia el tiempo del delay */
void delayWrite(delay_t *delay, uint32_t duration) {
    delay->duration = duration;
}

