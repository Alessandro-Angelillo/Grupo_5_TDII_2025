/*
 * API_delay.h
 *
 *  Created on: Sep 16, 2025
 *      Author: EVE
 */
#ifndef API_DELAY_H
#define API_DELAY_H

#include <stdbool.h>
#include "main.h"

typedef struct {
    uint32_t startTime;
    uint32_t duration;
    bool running;
} delay_t;

void delayInit(delay_t *delay, uint32_t duration);
bool delayRead(delay_t *delay);
void delayWrite(delay_t *delay, uint32_t duration);

#endif /* API_DELAY_H */
