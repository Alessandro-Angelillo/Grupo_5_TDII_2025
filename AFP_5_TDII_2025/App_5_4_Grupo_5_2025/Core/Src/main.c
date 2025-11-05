/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * Este archivo contiene el punto de entrada principal del programa embebido.
  * Implementa parpadeo de LEDs con cambio de frecuencia mediante pulsador.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_GPIO.h"  // Driver personalizado para manejo de LEDs y pulsador

// ─────────────────────────────────────────────
// Definición de tiempos de parpadeo en milisegundos
// ─────────────────────────────────────────────
#define TIME1 100
#define TIME2 250
#define TIME3 700
#define TIME4 2000

// ─────────────────────────────────────────────
// Enumeración de frecuencias disponibles
// ─────────────────────────────────────────────
typedef enum {
  FREQ_1 = 0,
  FREQ_2,
  FREQ_3,
  FREQ_4,
  FREQ_COUNT  // Total de frecuencias
} blink_freq_t;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;  // UART para comunicación serie
PCD_HandleTypeDef hpcd_USB_OTG_FS;  // USB OTG

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);  // Configuración del reloj del sistema
//static void MX_GPIO_Init(void);  // Comentado, se usa desde API_GPIO
static void MX_USART3_UART_Init(void);  // Inicialización de UART3
static void MX_USB_OTG_FS_PCD_Init(void);  // Inicialización de USB OTG

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Punto de entrada principal del programa
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  // ─────────────────────────────────────────────
  // Inicialización básica del sistema
  // ─────────────────────────────────────────────
  HAL_Init();               // Inicializa HAL y SysTick
  SystemClock_Config();     // Configura el reloj del sistema
  MX_GPIO_Init();           // Inicializa los pines GPIO
  MX_USART3_UART_Init();    // Inicializa UART3
  MX_USB_OTG_FS_PCD_Init(); // Inicializa USB OTG

  /* USER CODE BEGIN 2 */

  debounceInit_GPIO();  // Inicializa el sistema de anti-rebote

  // Variables de estado para frecuencia y temporización
  blink_freq_t currentFreq = FREQ_1;
  uint32_t tickBlink = HAL_GetTick();  // Marca de tiempo para parpadeo

  // bool_t ledsOn = false;  // Comentado, no se usa en esta versión

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    debounceUpdate_GPIO();  // Actualiza el estado del pulsador con anti-rebote

    // Si se detecta una pulsación válida, se cambia de frecuencia
    if (readKey_GPIO()) {
      currentFreq = (currentFreq + 1) % FREQ_COUNT;
    }

    uint32_t now = HAL_GetTick();  // Captura el tiempo actual
    uint32_t delay = 0;            // Variable para almacenar el tiempo de parpadeo

    // Selección del tiempo según la frecuencia actual
    switch (currentFreq) {
      case FREQ_1: delay = TIME1; break;
      case FREQ_2: delay = TIME2; break;
      case FREQ_3: delay = TIME3; break;
      case FREQ_4: delay = TIME4; break;
      default: delay = TIME1; break;
    }

    // Si ha pasado el tiempo correspondiente, se alternan los LEDs
    if (now - tickBlink >= delay) {
      for (uint8_t i = 0; i < getLedCount(); i++) {
        toggleLed_GPIO(getLedByIndex(i));  // Cambia el estado de cada LED
      }
      tickBlink = now;  // Actualiza la marca de tiempo
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief Configuración del reloj del sistema
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();  // Manejo de error si falla la configuración
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();  // Manejo de error si falla la configuración
  }
}

/**
  * @brief Inicialización de UART3
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();  // Manejo de error si falla la inicialización
  }
}

/**
  * @brief Inicialización de USB OTG FS
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();  // Manejo de error si falla la inicialización
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
