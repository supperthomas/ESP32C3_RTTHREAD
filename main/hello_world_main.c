#include <stdio.h>
#include "driver/gpio.h"
#define BLINK_GPIO 8
#define LOOP_COUNT 50000000
#include "rtthread.h"
void main_thread_entry(void)
{
    printf("Hello world!\n");

    uint32_t count = 0;
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        count= LOOP_COUNT;
        while(count)
        {
            count--;
        }
        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        count= LOOP_COUNT;
        while(count)
        {
            count--;
        }
    }
}
static struct rt_thread _main_thread;
static rt_uint8_t _main_thread_stack[512];
void rt_application_init(void)
{
    rt_thread_t tid;

       rt_thread_init(&_main_thread,
                   "main",
                   main_thread_entry,
                   RT_NULL,
                   &_main_thread_stack[0],
                   sizeof(_main_thread_stack),
                   2,
                   10);

    rt_thread_startup(&_main_thread);
}

void rtthread_startup(void)
{
    /* init board */
    //rt_hw_board_init();

    /* init scheduler system */
     rt_system_scheduler_init();
    //     /* init application */
    rt_application_init();


    // /* init idle thread */
     rt_thread_idle_init();

    // /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return ;
}

void app_main(void)
{
    /* startup RT-Thread RTOS */
    //main_thread_entry()                      
       rtthread_startup();                                                                                                                                                                                                                                 ;
    return;
 
}
rt_base_t rt_hw_interrupt_disable(void)
{
    return 1;
}
void rt_hw_interrupt_enable(rt_base_t level)
{
    
}

void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to)
{
    
}
void rt_hw_context_switch_interrupt(rt_uint32_t from, rt_uint32_t to)
{
    
}
void rt_hw_context_switch_to(rt_uint32_t to)
{
    
}

struct exception_stack_frame
{
    rt_uint32_t r0;
    rt_uint32_t r1;
    rt_uint32_t r2;
    rt_uint32_t r3;
    rt_uint32_t r12;
    rt_uint32_t lr;
    rt_uint32_t pc;
    rt_uint32_t psr;
};

struct stack_frame
{

    /* r4 ~ r11 register */
    rt_uint32_t r4;
    rt_uint32_t r5;
    rt_uint32_t r6;
    rt_uint32_t r7;
    rt_uint32_t r8;
    rt_uint32_t r9;
    rt_uint32_t r10;
    rt_uint32_t r11;

    struct exception_stack_frame exception_stack_frame;
};

rt_uint8_t *rt_hw_stack_init(void       *tentry,
                             void       *parameter,
                             rt_uint8_t *stack_addr,
                             void       *texit)
{
    struct stack_frame *stack_frame;
    rt_uint8_t         *stk;
    unsigned long       i;

    stk  = stack_addr + sizeof(rt_uint32_t);
    stk  = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stk, 8);
    stk -= sizeof(struct stack_frame);

    stack_frame = (struct stack_frame *)stk;

    /* init all register */
    for (i = 0; i < sizeof(struct stack_frame) / sizeof(rt_uint32_t); i ++)
    {
        ((rt_uint32_t *)stack_frame)[i] = 0xdeadbeef;
    }

    stack_frame->exception_stack_frame.r0  = (unsigned long)parameter; /* r0 : argument */
    stack_frame->exception_stack_frame.r1  = 0;                        /* r1 */
    stack_frame->exception_stack_frame.r2  = 0;                        /* r2 */
    stack_frame->exception_stack_frame.r3  = 0;                        /* r3 */
    stack_frame->exception_stack_frame.r12 = 0;                        /* r12 */
    stack_frame->exception_stack_frame.lr  = (unsigned long)texit;     /* lr */
    stack_frame->exception_stack_frame.pc  = (unsigned long)tentry;    /* entry point, pc */
    stack_frame->exception_stack_frame.psr = 0x01000000L;              /* PSR */

    /* return task's current stack address */
    return stk;
}
