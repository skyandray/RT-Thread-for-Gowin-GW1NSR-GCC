
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
