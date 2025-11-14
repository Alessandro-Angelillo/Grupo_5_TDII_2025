/*
 * API_Delay.h
 *
 *  Created on: Sep 17, 2025
 *      Author: EVE
 */
#ifndef API_DELAY_H
#define API_DELAY_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

/* Estructura para manejar retardos no bloqueantes */
typedef struct {
    uint32_t startTime;     // Tiempo inicial (ms)
    uint32_t duration;      // Duración configurada
    uint8_t running;        // Estado: 0 = detenido, 1 = corriendo
} delay_t;

/* Prototipos de funciones */
void delayInit(delay_t *delay, uint32_t duration);
uint8_t delayRead(delay_t *delay);
void delayWrite(delay_t *delay, uint32_t duration);

#endif /* API_DELAY_H */
