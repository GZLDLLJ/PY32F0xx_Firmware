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
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_CompInit(void);

/**
  * @brief  应用程序入口函数.
  * @param  无
  * @retval int
  */
int main(void)
{
  /* 开SYSCFG和PWR时钟 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  /* 配置系统时钟 */
  APP_SystemClockConfig();

  /* 初始化LED */
  BSP_LED_Init(LED_GREEN);

  /* 比较器初始化 */
  APP_CompInit();

  while (1)
  {
    if(LL_COMP_ReadOutputLevel(COMP2))
    {
      BSP_LED_On(LED_GREEN);
    }
    else
    {
      BSP_LED_Off(LED_GREEN);
    }
  }
}

/**
  * @brief  比较器初始化函数
  * @param  无
  * @retval 无
  */
static void APP_CompInit(void)
{
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**比较器1的GPIO配置
  PA1   ------> 比较器1的Plus端输入
  */
  LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);
  
  LL_GPIO_SetPinPull(GPIOA,LL_GPIO_PIN_1,LL_GPIO_PULL_NO);

  /* 使能比较器1时钟*/
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_COMP1);

  /* 使能比较器2时钟 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_COMP2);
 
  /* 输入Minus端选择VREFINT */
  LL_COMP_SetInputMinus(COMP2,LL_COMP_INPUT_MINUS_VREFINT);
  
  /* 使能Scaler */
  LL_COMP_EnableScaler(COMP2);
  
  /* 迟滞关闭 */
  LL_COMP_SetInputHysteresis(COMP2,LL_COMP_HYSTERESIS_DISABLE);
  
  /* 极性不反向 */
  LL_COMP_SetOutputPolarity(COMP2,LL_COMP_OUTPUTPOL_NONINVERTED);
  
  /* 功耗模式快速 */
  LL_COMP_SetPowerMode(COMP2, LL_COMP_POWERMODE_HIGHSPEED);

  /* 窗口模式使能选择COMP1的Plus端输入 */
  LL_COMP_SetCommonWindowMode(__LL_COMP_COMMON_INSTANCE(COMP2), LL_COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON);

  /* 输入Plus端选择PA1 */
  LL_COMP_SetInputPlus(COMP1,LL_COMP_INPUT_PLUS_IO3);

  __IO uint32_t wait_loop_index = 0;
  wait_loop_index = (LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (1000000 * 2)));
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }

  /* 使能比较器2 */
  LL_COMP_Enable(COMP2);
  wait_loop_index = ((LL_COMP_DELAY_STARTUP_US / 10UL) * (SystemCoreClock / (100000UL * 2UL)));
  while(wait_loop_index != 0UL)
  {
    wait_loop_index--;
  }
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */
static void APP_SystemClockConfig(void)
{
  /* 使能HSI */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* 设置 AHB 分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置HSISYS作为系统时钟源 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /* 设置 APB1 分频*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(8000000);

  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(8000000);
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
