/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  * @date
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
#define Vcc_Power     3.3l                                            /* VCC电源电压,根据实际情况修改 */
#define TScal1        (float)((HAL_ADC_TSCAL1) * 3.3 / Vcc_Power)     /* 85摄氏度校准值对应电压*/
#define TScal2        (float)((HAL_ADC_TSCAL2) * 3.3 / Vcc_Power)     /* 30摄氏度校准值对应电压 */
#define TStem1        30l                                             /* 30摄氏度*/
#define TStem2        85l                                             /* 85摄氏度 */
#define Temp_k        ((float)(TStem2-TStem1)/(float)(TScal2-TScal1)) /* 温度计算 */
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef             AdcHandle;
ADC_ChannelConfTypeDef        sConfig;
volatile int16_t              aADCxConvertedData;
int16_t                       aTEMPERATURE;
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Error_Handler(void);
void APP_ADCConfig(void);

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{
  /* 初始化所有外设，Flash接口，SysTick */
  HAL_Init();

  /* 初始化调试串口(printf使用) */
  BSP_USART_Config();

  /* ADC配置 */  
  APP_ADCConfig();

  /* 无限循环 */
  while (1)
  {
    HAL_Delay(500);                                                  
    if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)                         /* 启动ADC并使能ADC中断 */
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  ADC配置函数
  * @param  无
  * @retval 无
  */
void APP_ADCConfig(void)
{
  __HAL_RCC_ADC_FORCE_RESET();
  __HAL_RCC_ADC_RELEASE_RESET();
  __HAL_RCC_ADC_CLK_ENABLE();                                                     /* 使能ADC时钟 */

  AdcHandle.Instance = ADC1;
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)                          /* ADC校准 */
  {
    Error_Handler();
  }
  AdcHandle.Instance                   = ADC1;                                    /* ADC*/
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;                /* 模拟ADC时钟源为PCLK 4分频*/
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;                      /* 转换分辨率12bit*/
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;                     /* 数据右对齐 */
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;              /* 扫描序列方向：向上(从通道0到通道11)*/
  AdcHandle.Init.EOCSelection          = ADC_EOC_SEQ_CONV;                        /* 转换结束标志*/
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;                                  /* 等待转换模式开启 */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                                 /* 单次转换模式 */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                                 /* 不使能非连续模式 */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                      /* 软件触发 */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;           /* 触发边沿无 */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* 当过载发生时，覆盖上一个值*/
  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_239CYCLES_5;              /* 通道采样时间为239.5ADC时钟周期 */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)                                         /* ADC初始化*/
  {
    Error_Handler();
  }

  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;                                 /* 设置是否采样 */
  sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;                                  /* 设置采样通道 */
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                      /* 配置ADC通道 */
  {
    Error_Handler();
  }

}

/**
  * @brief  ADC转换完成执行函数,打印当前测得得温度值
  * @param  无
  * @retval 无
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  aADCxConvertedData = HAL_ADC_GetValue(hadc);
  aTEMPERATURE =(int16_t)(Temp_k * aADCxConvertedData - Temp_k * TScal1 + TStem1);
  printf("Temperature = %d \r\n", aTEMPERATURE);

}

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void Error_Handler(void)
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

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
