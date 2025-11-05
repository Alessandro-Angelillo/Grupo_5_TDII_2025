/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  * Este archivo contiene el punto de entrada principal del programa embebido.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_GPIO.h"  // Driver personalizado para manejo de LEDs y pulsador

// ─────────────────────────────────────────────
// Definición de tiempos para cada secuencia
// ─────────────────────────────────────────────
#define SEQ1_DELAY 1500
#define SEQ2_DELAY 300
#define SEQ3_LED1_DELAY 100
#define SEQ3_LED2_DELAY 300
#define SEQ3_LED3_DELAY 600
#define SEQ4_DELAY 150

// ─────────────────────────────────────────────
// Enumeración de las secuencias disponibles
// ─────────────────────────────────────────────
typedef enum {
  SEQUENCE_1 = 0,
  SEQUENCE_2,
  SEQUENCE_3,
  SEQUENCE_4,
  SEQUENCE_COUNT  // Total de secuencias
} sequence_t;

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);

/**
  * @brief  Punto de entrada principal del programa
  * @retval int
  */
int main(void)
{
  // ─────────────────────────────────────────────
  // Inicialización básica del sistema
  // ─────────────────────────────────────────────
  HAL_Init();               // Inicializa HAL y SysTick
  SystemClock_Config();     // Configura el reloj del sistema
  MX_GPIO_Init();           // Inicializa los pines GPIO
  MX_USART3_UART_Init();    // Inicializa UART3
  MX_USB_OTG_FS_PCD_Init(); // Inicializa USB OTG

  // ─────────────────────────────────────────────
  // Inicialización del sistema de anti-rebote
  // ─────────────────────────────────────────────
  debounceInit_GPIO();

  // ─────────────────────────────────────────────
  // Variables de estado para cada secuencia
  // ─────────────────────────────────────────────
  sequence_t currentSequence = SEQUENCE_1;
  uint32_t tickSeq1 = HAL_GetTick();
  uint32_t tickSeq2 = HAL_GetTick();
  uint32_t tickSeq3[3] = {HAL_GetTick(), HAL_GetTick(), HAL_GetTick()};
  uint32_t tickSeq4 = HAL_GetTick();
  bool_t stateSeq2 = false;
  bool_t stateSeq4 = false;

  // ─────────────────────────────────────────────
  // Bucle principal del programa
  // ─────────────────────────────────────────────
  while (1)
  {
    debounceUpdate_GPIO();  // Actualiza el estado del pulsador con anti-rebote

    // Si se detecta una pulsación válida, se cambia de secuencia
    if (readKey_GPIO()) {
      currentSequence = (currentSequence + 1) % SEQUENCE_COUNT;
    }

    uint32_t now = HAL_GetTick();  // Captura el tiempo actual

    // ─────────────────────────────────────────────
    // Ejecución de la secuencia actual
    // ─────────────────────────────────────────────
    switch (currentSequence)
    {
      // ──────────────── Secuencia 1 ────────────────
      case SEQUENCE_1:
        if (now - tickSeq1 >= SEQ1_DELAY) {
          static uint8_t ledIndex = 0;
          static bool_t ledOn = false;

          if (ledOn) {
            writeLedOff_GPIO(getLedByIndex(ledIndex));
            ledOn = false;
          } else {
            writeLedOn_GPIO(getLedByIndex(ledIndex));
            ledOn = true;
            ledIndex = (ledIndex + 1) % getLedCount();
          }
          tickSeq1 = now;
        }
        break;

      // ──────────────── Secuencia 2 ────────────────
      case SEQUENCE_2:
        if (now - tickSeq2 >= SEQ2_DELAY) {
          stateSeq2 = !stateSeq2;
          for (uint8_t i = 0; i < getLedCount(); i++) {
            if (stateSeq2)
              writeLedOn_GPIO(getLedByIndex(i));
            else
              writeLedOff_GPIO(getLedByIndex(i));
          }
          tickSeq2 = now;
        }
        break;

      // ──────────────── Secuencia 3 ────────────────
      case SEQUENCE_3:
        for (uint8_t i = 0; i < getLedCount(); i++) {
          uint32_t delay = (i == 0) ? SEQ3_LED1_DELAY : (i == 1) ? SEQ3_LED2_DELAY : SEQ3_LED3_DELAY;
          if (now - tickSeq3[i] >= delay) {
            toggleLed_GPIO(getLedByIndex(i));
            tickSeq3[i] = now;
          }
        }
        break;

      // ──────────────── Secuencia 4 ────────────────
      case SEQUENCE_4:
        if (now - tickSeq4 >= SEQ4_DELAY) {
          stateSeq4 = !stateSeq4;
          if (stateSeq4) {
            writeLedOn_GPIO(getLedByIndex(0)); // LD1
            writeLedOff_GPIO(getLedByIndex(1)); // LD2
            writeLedOn_GPIO(getLedByIndex(2)); // LD3
          } else {
            writeLedOff_GPIO(getLedByIndex(0));
            writeLedOn_GPIO(getLedByIndex(1));
            writeLedOff_GPIO(getLedByIndex(2));
          }
          tickSeq4 = now;
        }
        break;

      // ──────────────── Caso por defecto ────────────────
      default:
        break;
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
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
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
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
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
