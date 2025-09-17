/*
 * API_Delay.h
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */

#ifndef API_DELAY_H
#define API_DELAY_H

#include <stdint.h>
#include <stdbool.h>

/* Estructura para manejar retardos no bloqueantes */
typedef struct {
    uint32_t start_time;
    uint32_t duration;
    bool running;
} delay_t;

/* Inicializa un delay */
void delayInit(delay_t *delay, uint32_t duration);

/* Devuelve true si el delay ya terminó */
bool delayRead(delay_t *delay);

/* Reinicia un delay */
void delayWrite(delay_t *delay, uint32_t duration);

/* Devuelve el tiempo actual en ms (wrapper de HAL_GetTick) */
uint32_t millis(void);

#endif
