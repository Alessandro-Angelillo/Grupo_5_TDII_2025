/*
 * API_GPIO.h
 *
 *  Created on: Aug 23, 2025
 *      Author: Ivan
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_

/*   Includes   */
#include <stdint.h>
/*   Exported types   */
typedef uint16_t led_t;


/*   Exported functions prototypes   */
void LED_Blink(GPIO_TypeDef* port, uint16_t pin, uint32_t delay);
void MX_GPIO_Init(void);


#endif /* API_INC_API_GPIO_H_ */
