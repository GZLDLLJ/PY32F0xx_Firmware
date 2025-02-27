================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了通过USART轮询收发数据的功能，MCU复位后会向PC端发送"UART Test",
PC端发送12个字符，MCU会反馈同样的12个字符给PC端。

Function descriptions:
This sample demonstrates the functionality of data transmission and reception 
using USART in polling mode. After resetting the MCU, it will send the string 
"UART Test" to the PC. When the PC sends 12 characters, the MCU will send back 
the same 12 characters to the PC.
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
1. 编译并下载程序到MCU；
2. 通过USB转TTL模块连接PC与STK板,STK板与USB转TTL模块的连线方式如下；
STK板        USB转TTL模块
PA02(TX) --> RX
PA03(RX) --> TX
GND      --> GND
3.PC端打开串口调试助手，正确连接上通讯COM口，波特率设置为9600，1位
停止位，8位数据位，无奇偶校验。
4. 复位MCU并重新运行，PC端收到字符串：UART Test；
5. PC端发送12个字符，MCU会反馈同样的12个字符给PC端。

Example execution steps:
1.Compile and download the program to the MCU.
2.Connect the PC and the STK board using a USB to TTL module. The connection 
is as follows:
STK board USB to TTL module
PA02(TX) --> RX
PA03(RX) --> TX
GND --> GND
3.Open a serial terminal on the PC and correctly connect to the designated 
COM port. Set the baud rate to 9600, stop bits to 1, data bits to 8, and 
parity to none.
4.Reset the MCU and run the program again. The PC will receive the string 
"UART Test".
5.Send 12 characters from the PC. The MCU will send back the same 12 
characters to the PC.
================================================================================
注意事项：

Notes:
================================================================================