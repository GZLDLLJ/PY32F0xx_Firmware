================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了通过PA6引脚唤醒MCU的功能。下载程序并运行后，LED灯处于常亮状态；按下
用户按键后，LED灯处于常暗状态，且MCU进入STOP模式；拉低PA6引脚后，MCU唤醒，LED灯
处于闪烁状态。

================================================================================
测试环境：
测试用板：PY32F002A_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU；
2. 断开swd连接线并重新上电;
3. 小灯处于常亮状态，按下用户按键，LED灯处于常暗状态，且MCU进入STOP模式；
4. 拉低PA6引脚后，MCU唤醒，LED灯处于闪烁状态。

================================================================================
注意事项：
1，演示此样例功能时需要断开swd连接线并重新上电，因为默认情况下，仿真器会把
DBGMCU_CR.DBG_STOP置位。

================================================================================