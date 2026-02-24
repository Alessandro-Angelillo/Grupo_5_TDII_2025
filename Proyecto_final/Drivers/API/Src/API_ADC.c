#include "API_ADC.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

float readTemperature(void)
{
    uint32_t adcValue; // guarda el valor digital del ADC
    float voltage; // guarda el voltaje convertido
    float temperature; // guarda la temperatura final

    HAL_ADC_Start(&hadc1); // inicia la convercion ADC
    HAL_ADC_PollForConversion(&hadc1, 10); // espera la conversion max 10ms
    adcValue = HAL_ADC_GetValue(&hadc1); // lee el valor digital
    HAL_ADC_Stop(&hadc1); //detiene ADC

    voltage = (adcValue * 3.3f) / 4095.0f;
    temperature = voltage * 100.0f;   // LM35 entrega 10mv por grado

    return temperature;
}


