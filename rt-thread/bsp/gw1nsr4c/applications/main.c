#include <rthw.h>
#include <rtthread.h>
#include "gw1ns4c.h"
#include "gw1ns4c_conf.h"

#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        20


ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[512];
static struct rt_thread thread1;
static char thread2_stack[512];
static struct rt_thread thread2;


static void gpio_user_init(void);


static void thread1_entry(void *parameter)
{
    while (1)
    {
		GPIO_ResetBit(GPIO0, GPIO_Pin_0);
		rt_thread_mdelay(250);
		GPIO_SetBit(GPIO0, GPIO_Pin_0);
		rt_thread_mdelay(250);
    }
}

static void thread2_entry(void *param)
{
    while (1)
    {
		GPIO_ResetBit(GPIO0, GPIO_Pin_1);
		rt_thread_mdelay(500);
		GPIO_SetBit(GPIO0, GPIO_Pin_1);
		rt_thread_mdelay(500);
    }

}


int main(void)
{	
	gpio_user_init();
	
	rt_thread_init(&thread1,
							 "thread1",
							 thread1_entry,
							 RT_NULL,
							 &thread1_stack[0],
							 sizeof(thread1_stack),
							 3, THREAD_TIMESLICE);
	rt_thread_startup(&thread1);	
	
	rt_thread_init(&thread2,
							 "thread2",
							 thread2_entry,
							 RT_NULL,
							 &thread2_stack[0],
							 sizeof(thread2_stack),
							 4, THREAD_TIMESLICE);
	rt_thread_startup(&thread2);

	
	while(1)
	{
		rt_thread_mdelay(500);
	}
}

static void gpio_user_init(void)
{
	GPIO0->OUTENSET = 0xFFFFFFFF;
	GPIO_SetBit(GPIO0, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
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
