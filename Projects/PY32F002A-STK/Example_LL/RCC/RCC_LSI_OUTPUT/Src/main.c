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
#include "py32f002xx_ll_Start_Kit.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void APP_SystemClockConfig(void);
static void APP_ConfigGPIO(void);
  
/**
  * @brief  应用程序入口函数.
  * @param  无
  * @retval int
  */
int main(void)
{
  /* 配置系统时钟 */
  APP_SystemClockConfig();

  /*引脚输出初始化*/
  APP_ConfigGPIO();
  
  /*MCO输出时钟及分频初始化*/
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK,LL_RCC_MCO1_DIV_1);

  while (1)
  {

  }
}

/**
  * @brief  配置PA01为MCO复用功能
  * @param  无
  * @retval 无
  */
static void APP_ConfigGPIO(void)
{
  /* GPIOA时钟使能*/
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  /* 配置PA01为复用模式，并复用为MCO输出引脚*/
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* 选择1号引脚*/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1; 
  /* 配置为复用模式*/
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE; 
  /* 选择复用为AF15功能*/
  GPIO_InitStruct.Alternate = LL_GPIO_AF15_MCO;     
  /* 输出速度选择*/
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;   
  /* 输出模式选择*/
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  /* 无上下拉*/
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                
  
  /* 初始化GPIOA*/
  LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
    
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */
void APP_SystemClockConfig(void)
{
 /* LSI使能及初始化 */
  LL_RCC_LSI_Enable();
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }
  
  /* 设置AHB分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  
  /* 配置LSI为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_LSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_LSI)
  {
  }
  
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  
  /*设置APB1分频及初始化*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32768);
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
