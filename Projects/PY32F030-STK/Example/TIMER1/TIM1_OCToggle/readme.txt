================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了TIM1比较模式下的OC翻转输出功能，使能CH1(PA08),CH2(PA09),CH3(PA10),
CH4(PA11)四个通道的输出功能，并且当计数器TIMx_CNT与TIMx_CCRx匹配时输出信号翻转
，频率为400KHz。

Function descriptions:
This sample demonstrates the output compare flip-flop functionality in TIM1 
compare mode. It enables the output function for CH1 (PA08), CH2 (PA09), CH3 
(PA10), and CH4 (PA11) channels. When the counter TIMx_CNT matches TIMx_CCRx, 
the output signal will toggle at a frequency of 400KHz.
================================================================================
测试环境：
测试用板：PY32F030_STK
MDK版本： 5.28
IAR版本： 9.20
GCC版本： GNU Arm Embedded Toolchain 10.3-2021.10

Test environment:
Test board: PY32F030_STK
MDK Version: 5.28
IAR Version: 9.20
GCC Version: GNU Arm Embedded Toolchain 10.3-2021.10
================================================================================
使用步骤：
1. 编译下载并运行程序；
2. 用示波器监测PA08/PA09/PA10/PA11引脚，可观察到与程序中配置相匹配的翻转波形。

Example execution steps:
1.Compile, download, and run the program.
2.Use an oscilloscope to monitor the PA08, PA09, PA10, and PA11 pins. You 
will observe OC waveforms matching the configuration in the program.
================================================================================
注意事项：
时钟主频为32M

Notes:
The system frequency is 32MHz.
================================================================================