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

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{
  /* 初始化所有外设，Flash接口，SysTick */
  HAL_Init();

  /* 初始化LED */
  BSP_LED_Init(LED_GREEN);

  TimHandle1.Instance = TIM1;                                           /* 选择TIM1 */
  TimHandle1.Init.Period            = 8000 - 1;                         /* 自动重装载值 */
  TimHandle1.Init.Prescaler         = 100 - 1;                          /* 预分频为1000-1 */
  TimHandle1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           /* 时钟不分频 */
  TimHandle1.Init.CounterMode       = TIM_COUNTERMODE_UP;               /* 向上计数 */
  TimHandle1.Init.RepetitionCounter = 1 - 1;                            /* 不重复计数 */
  TimHandle1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* 自动重装载寄存器没有缓冲 */
  if (HAL_TIM_Base_Init(&TimHandle1) != HAL_OK)                         /* TIM1初始化 */
  {
    APP_ErrorHandler();
  }

  TimHandle3.Instance = TIM3;                                               /* 选择TIM3 */
  TimHandle3.Init.Prescaler = 1000 - 1;                                     /* 预分频为1000-1 */
  TimHandle3.Init.CounterMode = TIM_COUNTERMODE_UP;                         /* 向上计数 */
  TimHandle3.Init.Period = 8000 - 1;                                        /* 自动重装载值 */
  TimHandle3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                   /* 时钟不分频 */
  TimHandle3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;       /* 自动重装载寄存器没有缓冲 */
  if (HAL_TIM_OC_Init(&TimHandle3) != HAL_OK)                               /* OC输出初始化 */
  {
    APP_ErrorHandler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;                 /* 主时钟更新事件产生TRGO信号 */
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;         /* 主从模式关闭 */
  HAL_TIMEx_MasterConfigSynchronization(&TimHandle3, &sMasterConfig);       /* TIM3配置为主模式 */

  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_TRIGGER;               /* 从模式选择为触发模式 */
  sSlaveConfig.InputTrigger     = TIM_TS_ITR2;                         /* TIM1的触发选择为TIM3 */
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;     /* 外部触发极性不反向 */
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;           /* 外部触发不分频 */
  sSlaveConfig.TriggerFilter    = 0;                                   /* 不滤波 */
  if (HAL_TIM_SlaveConfigSynchro(&TimHandle1, &sSlaveConfig) != HAL_OK) /* TIM1配置为从模式 */
  {
    APP_ErrorHandler();
  }
  __HAL_TIM_ENABLE_IT(&TimHandle1, TIM_IT_UPDATE);                      /* 使能TIM1更新中断 */
  if (HAL_TIM_OC_Start(&TimHandle3, TIM_CHANNEL_1) != HAL_OK)               /* TIM3的OC输出启动 */
  {
    APP_ErrorHandler();
  }

  while (1)
  {
  }
}

/**
  * @brief  TIM更新中断回调函数
  * @param  htim：TIM句柄
  * @retval 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  BSP_LED_Toggle(LED_GREEN);

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
