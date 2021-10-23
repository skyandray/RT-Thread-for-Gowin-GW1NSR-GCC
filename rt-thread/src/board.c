/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "gw1ns4c.h"
#include "gw1ns4c_conf.h"

#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif
static void sys_tick_init(void)
{
	uint32_t temp;
	
	//24-bit register, max value is 16777215
	//When SystemCoreClock is 25MHz, it is 671ms
	//Set value of reload register
	temp = (SystemCoreClock / 1000) - 1;
	SysTick->LOAD = temp;
	
	SysTick->VAL = temp;	//Reset current counter value

	//Select clock source, enable interrupt, enable counter
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; 
}
/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Clock Update */
    SystemCoreClockUpdate();
    
    /* System Tick Configuration */
    sys_tick_init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}


static void uart_user_init(uint32_t baudrate)
{
	UART_InitTypeDef UART_InitStruct;

	UART_InitStruct.UART_Mode.UARTMode_Tx = ENABLE;
	UART_InitStruct.UART_Mode.UARTMode_Rx = ENABLE;
	UART_InitStruct.UART_Int.UARTInt_Tx = DISABLE;
	UART_InitStruct.UART_Int.UARTInt_Rx = DISABLE;
	UART_InitStruct.UART_Ovr.UARTOvr_Tx = DISABLE;
	UART_InitStruct.UART_Ovr.UARTOvr_Rx = DISABLE;
	UART_InitStruct.UART_Hstm = DISABLE;
	UART_InitStruct.UART_BaudRate = baudrate;

	UART_Init(UART0,&UART_InitStruct);
}


void Board_Init( void )
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	uart_user_init(115200);	
}
INIT_BOARD_EXPORT(Board_Init);

void rt_hw_console_output(const char *str)
{
	rt_size_t i = 0, size = 0;
	char a = '\r';
	size = rt_strlen(str);
	
	for (i = 0; i < size; i++)
	{
			if (*(str + i) == '\n')
			{
					UART_SendChar(UART0, a);
			}
			UART_SendChar(UART0, *(str + i));
	}
}

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
