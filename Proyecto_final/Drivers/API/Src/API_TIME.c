#include "API_TIME.h"
#include "stm32f4xx_hal.h"
/* Funcion no bloqueante verifica si paso el tiempo
 determinado
 */
bool delayRead(tick_t * delay, tick_t duration)
{
    if ((HAL_GetTick() - *delay) >= duration)
    {
        *delay = HAL_GetTick();
        return true;
    }
    return false;
}
