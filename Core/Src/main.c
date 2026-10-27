/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Conexiones al TURTLE:
 *   Motor A: ENA = PAT1, IN1 = PAT3, IN2 = PAT4
 *   Motor B: ENB = PAT2, IN3 = PAT5, IN4 = PAT6
 *
 * dir:  1 = adelante,  -1 = atrás,  0 = detenido
 */
static void motorA(int8_t dir)
{
  HAL_GPIO_WritePin(GPIOA, PAT3_Pin, (dir > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);  // IN1
  HAL_GPIO_WritePin(GPIOA, PAT4_Pin, (dir < 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);  // IN2
  HAL_GPIO_WritePin(GPIOA, PAT1_Pin, (dir != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET); // ENA
}

static void motorB(int8_t dir)
{
  HAL_GPIO_WritePin(GPIOA, PAT5_Pin, (dir > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);  // IN3
  HAL_GPIO_WritePin(GPIOA, PAT6_Pin, (dir < 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);  // IN4
  HAL_GPIO_WritePin(GPIOA, PAT2_Pin, (dir != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET); // ENB
}

static void detener(void)
{
  motorA(0);
  motorB(0);
}

// Secuencia 1: avanzar en línea recta
static void avanzar(uint32_t ms)
{
  motorA(1);
  motorB(1);
  HAL_Delay(ms);
  detener();
}

// Secuencia 2: girar en el lugar hacia un lado
static void girar_izquierda(uint32_t ms)
{
  motorA(-1);
  motorB(1);
  HAL_Delay(ms);
  detener();
}

// Secuencia 3: retroceder en línea recta
static void retroceder(uint32_t ms)
{
  motorA(-1);
  motorB(-1);
  HAL_Delay(ms);
  detener();
}

// Secuencia 4: girar en el lugar hacia el otro lado
static void girar_derecha(uint32_t ms)
{
  motorA(1);
  motorB(-1);
  HAL_Delay(ms);
  detener();
}

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
  /* USER CODE BEGIN 2 */

  // Las 4 secuencias, una tras otra (se ejecutan una sola vez)
  avanzar(4000);            // 1. Avanzar 4 s
  HAL_Delay(500);

  girar_izquierda(3000);    // 2. Girar en el lugar hacia un lado 3 s
  HAL_Delay(500);

  retroceder(4000);         // 3. Retroceder 4 s
  HAL_Delay(500);

  girar_derecha(3000);      // 4. Girar en el lugar hacia el otro lado 3 s

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PAT1_Pin|PAT2_Pin|PAT3_Pin|PAT4_Pin
                          |PAT5_Pin|PAT6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PAT1_Pin PAT2_Pin PAT3_Pin PAT4_Pin
                           PAT5_Pin PAT6_Pin */
  GPIO_InitStruct.Pin = PAT1_Pin|PAT2_Pin|PAT3_Pin|PAT4_Pin
                          |PAT5_Pin|PAT6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

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