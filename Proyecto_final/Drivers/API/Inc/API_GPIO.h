#ifndef API_GPIO_H_ // proteccion contra doble inclucion
#define API_GPIO_H_

#include <stdint.h> // libreria estandar
#include <stdbool.h>

/* Inicialización */
void MX_GPIO_Init(void);

/* Actuadores */
void heaterOn(void); // enciende la resistencia
void heaterOff(void); // apaga la resistencia

void fanOn(void);  // enciende el vendilador
void fanOff(void); // apaga el ventilador

void motorOn(void); // enciende el motor dw giro
void motorOff(void); // apaga el motor de giro

#endif
