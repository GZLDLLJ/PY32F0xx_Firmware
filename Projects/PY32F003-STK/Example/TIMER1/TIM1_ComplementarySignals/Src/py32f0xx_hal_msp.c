/**
  ******************************************************************************
  * @file    py32f0xx_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief 初始化全局MSP
  */
void HAL_MspInit(void)
{
}

/**
  * @brief 初始TIM相关MSP
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct = {0};
  /* TIM1时钟使能 */
  __HAL_RCC_TIM1_CLK_ENABLE();                    
  /* GPIOA时钟使能 */
  __HAL_RCC_GPIOA_CLK_ENABLE();                   
  /* GPIOB时钟使能 */
  __HAL_RCC_GPIOB_CLK_ENABLE();                   

  /*初始化PA0/PA3为TIM1_CH1N/TIM1_CH1*/
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF14_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  /* A0-TIM1_CH1N */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF13_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  /* A3-TIM1_CH1 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
