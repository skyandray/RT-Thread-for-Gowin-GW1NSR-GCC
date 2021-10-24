# Porting RT-Thread for Gowin GW1NSR-4C Soc GCC version

Hello everyone, this project based on RT-THREAD NANO 3.1.5 and GOWIN GW1NSR-4C Soc chip. The compilation environment is GCC. In this code, two user threads are running and two leds are controlled. The crystal oscillator on my development board is 27M, so the main frequency of my MCU is 78M. If your main frequency is 80M, you need to modify the __SYSTEM_CLOCK macro.

Finsh is also ported to this project, using UART0, the parameter is 115200 8n1.

An openocd 0.11 script is also provided for you to debug with the onboard FTDI chip.

Thanks!


# RT-Thread 移值到 高云 GW1NSR-4C 平台 GCC 版本

大家好，这个项目基于RT-Thread 3.1.5 和 高云 GW1NSR-4C 芯片。编译环境是GCC。
在代码里，有两个用户线程分别控制两个GPIO的两个LED。我板子上的晶振是27M，所以我的MCU主频是78M。如果你的主频是80M，需要修改代码里的__SYSTEM_CLOCK宏为80000000。

Finsh 的支持也被移植过来了，使用UART0，参数为：115200 8n1

我这里也提供了一个openocd 0.11的调试用脚本，可以使用板子上的FTDI JTAG，而不必外接JTAG。

谢谢！
