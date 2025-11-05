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
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* INCLUDES */

#include "main.h"
#include "API_GPIO.h"  // Driver personalizado para manejo de LEDs y pulsador externo

// Definiciones de tiempo en milisegundos
/* DEFINICIONES DE TIEMPO Y ESTADOS */

#define LED_ON_TIME 200  // Duración del encendido de cada LED
#define LED_OFF_TIME 200  // Duración del apagado de cada LED

// Estados posibles del LED
typedef enum {
  LED_STATE_ON,
  LED_STATE_OFF
} led_state_t;

// VARIABLES DE CONTROL DE LA SECUENCIA
// Se comentan porque se definen dentro de main()
// static uint32_t lastTick = 0;
// static uint8_t currentLed = 0;

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
UART_HandleTypeDef huart3;             // UART3 (no utilizada en esta aplicación)
PCD_HandleTypeDef hpcd_USB_OTG_FS;     // USB OTG FS (no utilizado en esta aplicación)

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);             // Configura el reloj del sistema
// static void MX_GPIO_Init(void);         // Comentado porque se usa el del driver
static void MX_USART3_UART_Init(void);     // Inicializa UART3
static void MX_USB_OTG_FS_PCD_Init(void);  // Inicializa USB OTG FS
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Punto de entrada principal de la aplicación
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();              // Inicializa HAL y el sistema de interrupciones

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  SystemClock_Config();    // Configura el reloj del sistema

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  // Inicializa periféricos configurados
  MX_GPIO_Init();          // Inicializa GPIOs (LEDs y pulsador externo)
  MX_USART3_UART_Init();   // UART (no usada en esta app)
  MX_USB_OTG_FS_PCD_Init();// USB (no usado en esta app)

  /* USER CODE BEGIN 2 */

  debounceInit_GPIO();     // Inicializa lógica anti-rebote del pulsador

  // Variables locales para controlar la secuencia de LEDs
  uint32_t lastTick = HAL_GetTick();  // Marca de tiempo inicial
  uint8_t currentLed = 0;             // LED actual encendido
  int8_t direction = 1;               // Dirección de la secuencia (1 o -1)
  led_state_t ledState = LED_STATE_ON;

  writeLedOn_GPIO(getLedByIndex(currentLed)); // Enciende el primer LED

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    debounceUpdate_GPIO();  // Actualiza estado del pulsador con anti-rebote

    if (readKey_GPIO()) {
      direction *= -1;      // Invierte la dirección de la secuencia
    }

    uint32_t now = HAL_GetTick();  // Obtiene el tiempo actual

    // Lógica no bloqueante para alternar LEDs
    if (ledState == LED_STATE_ON && (now - lastTick >= LED_ON_TIME)) {
      writeLedOff_GPIO(getLedByIndex(currentLed)); // Apaga LED actual
      lastTick = now;
      ledState = LED_STATE_OFF;
    }
    else if (ledState == LED_STATE_OFF && (now - lastTick >= LED_OFF_TIME)) {
      currentLed = (currentLed + direction + getLedCount()) % getLedCount(); // Calcula siguiente LED
      writeLedOn_GPIO(getLedByIndex(currentLed)); // Enciende siguiente LED
      lastTick = now;
      ledState = LED_STATE_ON;
    }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief Configura el reloj del sistema
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
    Error_Handler();  // Entra en modo error si falla la configuración
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();  // Entra en modo error si falla la configuración
  }
}

/**
  * @brief Inicializa UART3 (no utilizada en esta aplicación)
  * @param None
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
    Error_Handler();
  }
}

/**
  * @brief Inicializa USB OTG FS (no utilizado en esta aplicación)
  * @param None
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
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval*/
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
