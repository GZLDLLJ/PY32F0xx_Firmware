/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef        TimHandle1, TimHandle3;
TIM_SlaveConfigTypeDef   sSlaveConfig;
TIM_MasterConfigTypeDef  sMasterConfig;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick */
  HAL_Init();
  
  /* System clock configuration */
  APP_SystemClockConfig(); 

  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  TimHandle1.Instance = TIM1;                                           /* Select TIM1 */
  TimHandle1.Init.Period            = 800 - 1;                          /* Auto-reload value */
  TimHandle1.Init.Prescaler         = 100 - 1;                          /* Prescaler of 100-1 */
  TimHandle1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           /* No clock division */
  TimHandle1.Init.CounterMode       = TIM_COUNTERMODE_UP;               /* Up counting */
  TimHandle1.Init.RepetitionCounter = 1 - 1;                            /* No repetition counting */
  TimHandle1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* Auto-reload register not buffered */
  /* Initialize TIM1 */
  if (HAL_TIM_Base_Init(&TimHandle1) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  TimHandle3.Instance = TIM3;                                               /* Select TIM3 */
  TimHandle3.Init.Prescaler = 100 - 1;                                      /* Prescaler of 100-1 */
  TimHandle3.Init.CounterMode = TIM_COUNTERMODE_UP;                         /* Up counting */
  TimHandle3.Init.Period = 1 - 1;                                           /* Auto-reload value */
  TimHandle3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                   /* No clock division */
  TimHandle3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;       /* Auto-reload register not buffered */
  /* Initialize TIM3 */
  if (HAL_TIM_Base_Init(&TimHandle3) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;                 /* Master clock generates TRGO signal on update event */
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;         /* Master/slave mode disabled */
  /* Configure TIM3 as master mode */
  HAL_TIMEx_MasterConfigSynchronization(&TimHandle3, &sMasterConfig);

  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_EXTERNAL1;             /* Slave mode selected as trigger mode */
  sSlaveConfig.InputTrigger     = TIM_TS_ITR2;                         /* Trigger source for TIM1 is TIM3 */
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;     /* External trigger polarity is non-inverted */
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;           /* External trigger is not prescaled */
  sSlaveConfig.TriggerFilter    = 0;                                   /* No trigger input filter */
  /* Configure TIM1 as slave mode */
  if (HAL_TIM_SlaveConfigSynchro(&TimHandle1, &sSlaveConfig) != HAL_OK) 
  {
    APP_ErrorHandler();
  }
  
  /* Start TIM1 counting and enable interrupt */
  if (HAL_TIM_Base_Start_IT(&TimHandle1) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Start TIM3 counting */
  if (HAL_TIM_Base_Start(&TimHandle3) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim：TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Toggle LED */
  BSP_LED_Toggle(LED_GREEN);
}

/**
  * @brief   System clock configuration function
  * @param   None
  * @retval  None
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI \
                                   | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;  /* Configure HSE, HSI, LSI, LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                             /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                             /* HSI no division */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;                     /* HSI calibration frequency of 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                            /* Disable HSE */
  /* RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz; */                                  /* HSE frequency range of 16~32MHz */
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                                            /* Disable LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                                            /* Disable LSE */
  /* RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM; */                             /* Default LSE drive capability */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                                        /* Disable PLL */
  /* RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI; */                           /* Select HSI as PLL source */
  /* Configure oscillators */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;/* Configure SYSCLK, HCLK, and PCLK */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                                        /* Configure the system clock as HSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                            /* AHB clock without division */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                                             /* APB clock without division */
  /* Configure clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief   This function is executed in case of error occurrence.
  * @param   None
  * @retval  None
  */
void APP_ErrorHandler(void)
{
  /* Infinite loop */
  while (1)
  {
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
