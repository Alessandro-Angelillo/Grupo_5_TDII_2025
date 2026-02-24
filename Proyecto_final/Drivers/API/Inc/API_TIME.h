#ifndef API_TIME_H_
#define API_TIME_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;

bool delayRead(tick_t * delay, tick_t duration);

#endif
