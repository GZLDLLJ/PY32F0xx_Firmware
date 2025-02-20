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
RTC_HandleTypeDef RtcHandle;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_RtcAlarmConfig(void);
static void APP_RtcTimeShow(void);

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{
  /* 复位所有外设，初始化flash接口和systick.*/
  HAL_Init();
  
  /* 系统时钟配置 */
  APP_SystemClockConfig();
  
  /* UART初始化 */
  DEBUG_USART_Config();
  
  /* RTC初始化 */
  RtcHandle.Instance = RTC;                        /* 选择RTC */
  RtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND; /* RTC一秒时基自动计算 */
  RtcHandle.Init.OutPut = RTC_OUTPUTSOURCE_NONE;   /* TAMPER引脚无输出 */
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* RTC时钟配置 */
  APP_RtcAlarmConfig();
  
  while (1)
  {
  }
}

/**
  * @brief  秒中断回调函数
  * @param  hrtc：RTC句柄
  * @retval 无
  */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
  printf("RTC_IT_SEC\r\n");
  APP_RtcTimeShow();
}

/**
  * @brief  闹钟中断回调函数
  * @param  hrtc：RTC句柄
  * @retval 无
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  printf("RTC_IT_ALARM\r\n");
}

/**
  * @brief  RTC闹钟配置
  * @param  无
  * @retval 无
  */
static void APP_RtcAlarmConfig(void)
{
  RTC_DateTypeDef  sdatestructure = {0};
  RTC_TimeTypeDef  stimestructure = {0};
  RTC_AlarmTypeDef salarmstructure = {0};

  /* 设置日期: 2021/5/21 星期五 */
  sdatestructure.Year = 0x21;
  sdatestructure.Month = 0x05;
  sdatestructure.Date = 0x21;
  sdatestructure.WeekDay = RTC_WEEKDAY_FRIDAY;
  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* 设置时间: 12:13:00 */
  stimestructure.Hours = 0x12;
  stimestructure.Minutes = 0x13;
  stimestructure.Seconds = 0x00;
  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /*设置RTC闹钟，时间到12:13:35产生中断*/
  salarmstructure.AlarmTime.Hours = 0x12;
  salarmstructure.AlarmTime.Minutes = 0x13;
  salarmstructure.AlarmTime.Seconds = 0x35;
  if (HAL_RTC_SetAlarm_IT(&RtcHandle, &salarmstructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  显示RTC当前时间
  * @param  无
  * @retval 无
  */
static void APP_RtcTimeShow(void)
{
  RTC_DateTypeDef sdatestructureget = {0};
  RTC_TimeTypeDef stimestructureget = {0};
  
  /* 获取RTC当前时间*/
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  
  /* 获取RTC当前日期 */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  
  /* 显示时间格式为 : hh:mm:ss */
  printf("%02d:%02d:%02d\r\n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI; /* Select oscillators HSE,HSI,LSI */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI not divided */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;  /* Configure HSI clock as 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* Disable HSE */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Disable LSI */

  /* Configure oscillators */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Select clock types HCLK, SYSCLK, PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; /* Select HSI as system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB  clock not divided */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB  clock not divided */
  /* Configure clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void APP_ErrorHandler(void)
{
  /* 无限循环 */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
