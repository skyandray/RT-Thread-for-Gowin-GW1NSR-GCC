/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rthw.h>
#include <rtconfig.h>
#include "gw1ns4c.h"
#include "gw1ns4c_conf.h"

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h 
#endif

#ifdef RT_USING_FINSH

char rt_hw_console_getchar(void)
{
      int ch = -1;

    if (UART0->STATE & UART_STATE_RXBF)
    {
        ch = (char)(UART0->DATA);
    }
        else
        {
            rt_thread_mdelay(10);
        }
    return ch;
}

#endif /* RT_USING_FINSH */

