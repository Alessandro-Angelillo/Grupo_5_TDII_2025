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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_GPIO.h" // Incluimos nuestro driver GPIO
//#include <stdio.h> // Para usar printf (si quieres depurar por UART)
#include "API_Delay.h"


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
UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
GPIO_TypeDef* led_ports[] = {LD1_GPIO_Port, LD2_GPIO_Port, LD3_GPIO_Port};
uint16_t led_pins[] = {LD1_Pin, LD2_Pin, LD3_Pin};
const uint8_t led_count = 3;

uint8_t secuencia = 1;
GPIO_PinState button_state, last_button_state = GPIO_PIN_RESET;

// Delays no bloqueantes
delay_t led_delay;       // Secuencia 1
delay_t seq2_delay;      // Secuencia 2
delay_t seq3_delay[3];   // Secuencia 3
delay_t seq4_delay;      // Secuencia 4

// Variables de estado
uint8_t led_index = 0;   // Secuencia 1
bool seq2_state = false; // Secuencia 2
bool seq4_state = false; // Secuencia 4
bool seq3_state[3] = {false, false, false}; // Secuencia 3
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
      delayInit(&led_delay, 150);
      delayInit(&seq2_delay, 300);
      delayInit(&seq3_delay[0], 100);
      delayInit(&seq3_delay[1], 300);
      delayInit(&seq3_delay[2], 600);
      delayInit(&seq4_delay, 150);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

      // === Leer pulsador ===
      button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
      if(button_state == GPIO_PIN_SET && last_button_state == GPIO_PIN_RESET){
          secuencia++;
          if(secuencia > 4) secuencia = 1;
          delayWrite(&led_delay, 200); // anti-rebote
      }
      last_button_state = button_state;

      // === Ejecutar secuencia ===
      switch(secuencia)
      {
          case 1: // Secuencia 1: alternancia 150 ms
              if(delayRead(&led_delay)){
                  for(uint8_t i=0; i<led_count; i++)
                      HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(led_ports[led_index], led_pins[led_index], GPIO_PIN_SET);
                  led_index++;
                  if(led_index >= led_count) led_index = 0;
                  delayWrite(&led_delay, 150);
              }
              break;

          case 2: // Secuencia 2: todos juntos 300 ms
              if(delayRead(&seq2_delay)){
                  seq2_state = !seq2_state;
                  for(uint8_t i=0; i<led_count; i++)
                      HAL_GPIO_WritePin(led_ports[i], led_pins[i], seq2_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                  delayWrite(&seq2_delay, 300);
              }
              break;

          case 3: // Secuencia 3: independiente 100/300/600 ms
              for(uint8_t i=0; i<led_count; i++){
                  if(delayRead(&seq3_delay[i])){
                      seq3_state[i] = !seq3_state[i];
                      HAL_GPIO_WritePin(led_ports[i], led_pins[i], seq3_state[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
                      delayWrite(&seq3_delay[i], (i==0)?100:(i==1)?300:600);
                  }
              }
              break;

          case 4: // Secuencia 4: LD1+LD3 juntos, LD2 inverso 150 ms
              if(delayRead(&seq4_delay)){
                  seq4_state = !seq4_state;
                  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, seq4_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, seq4_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, seq4_state ? GPIO_PIN_RESET : GPIO_PIN_SET);
                  delayWrite(&seq4_delay, 150);
              }
              break;
      }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
