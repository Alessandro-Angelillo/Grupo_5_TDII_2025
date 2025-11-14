/*
 * API_GPIO.h
 *
 *  Created on: Aug 23, 2025
 *      Author: aless
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/*   Includes   */
#include <stdint.h>
/*   Exported types   */
typedef uint16_t led_t;


/*   Exported functions prototypes   */
void LED_Blink(GPIO_TypeDef* port, uint16_t pin, uint32_t delay);
void BTN_UpdateDirection(int8_t *direction);

void APP3_UpdateMode(uint8_t *modo, uint32_t debounce_ms);
void APP3_Mode1(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count);
void APP3_Mode2(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count);
void APP3_Mode3(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count, const uint32_t tiempos[]);
void APP3_Mode4(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count);

void APP4_UpdateTimeIndex(uint8_t *tiempo_index, uint8_t tiempos_count, uint32_t debounce_ms);
void APP4_BlinkAll(GPIO_TypeDef* ports[], uint16_t pins[], uint8_t led_count, const uint16_t tiempos[], uint8_t tiempo_index);
void MX_GPIO_Init(void);


#endif /* API_INC_API_GPIO_H_ */
