================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例使用外接1KHz高精度的时钟来校准内部HSI。

================================================================================
测试环境：
测试用板：PY32F003_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行。
2. PA4接1KHz高精度时钟。
3. 按下按键，LED亮，开始校准HSI。
4. LED闪烁表明校准完成，MCO2(PA1)输出3MHz波形。
================================================================================
注意事项：
PA4输入1Khz高精度时钟。
================================================================================