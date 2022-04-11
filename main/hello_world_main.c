#include <stdio.h>
#include "driver/gpio.h"
#define BLINK_GPIO 8
#define LOOP_COUNT 100000000
#include "rtthread.h"
#include "esp_private/panic_internal.h"
#include "hal/uart_hal.h"
#include "driver/timer.h"
#include "soc/periph_defs.h"
#include "hal/systimer_hal.h"
#include "hal/systimer_ll.h"
#include "esp_intr_alloc.h"

uint32_t scount = 0;

static uart_hal_context_t s_panic_uart = { .dev = CONFIG_ESP_CONSOLE_UART_NUM == 0 ? &UART0 :&UART1 };

void t_panic_print_char(const char c)
{
    uint32_t sz = 0;
    while (!uart_hal_get_txfifo_len(&s_panic_uart));
    uart_hal_write_txfifo(&s_panic_uart, (uint8_t *) &c, 1, &sz);
}
static systimer_hal_context_t systimer_hal;
IRAM_ATTR void t_SysTickIsrHandler(void *arg)
{
systimer_ll_clear_alarm_int(systimer_hal.dev, 1);
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
     systimer_ll_is_alarm_int_fired(systimer_hal.dev, 1);
}

void main_thread_entry(void)
{
    printf("Hello world!\n");
        uint32_t count = 0;
        uint8_t system_timer_counter=1;

    //rt_hw_interrupt_enable(0);
    esp_intr_alloc(ETS_SYSTIMER_TARGET1_EDGE_INTR_SOURCE, ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL1, t_SysTickIsrHandler, &systimer_hal, NULL);
    systimer_hal_init(&systimer_hal);
    systimer_ll_set_counter_value(systimer_hal.dev, system_timer_counter, 0);
    systimer_ll_apply_counter_value(systimer_hal.dev, system_timer_counter);
    uint32_t alarm_id = 1 ;
    systimer_hal_connect_alarm_counter(&systimer_hal, alarm_id, system_timer_counter);
    systimer_hal_set_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
    systimer_hal_select_alarm_mode(&systimer_hal, alarm_id, SYSTIMER_ALARM_MODE_PERIOD);
        systimer_hal_counter_can_stall_by_cpu(&systimer_hal, 1, 0, true);
    systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);


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
      //  rt_thread_mdelay(1000);


        /* Blink on (output high) */
       // t_panic_print_char('T');
       // printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        count= LOOP_COUNT;
        while(count)
        {
            count--;
        }
        //rt_thread_mdelay(1000);

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
// static struct rt_serial_device _serial_0;
// typedef struct
// {
//     struct rt_serial_device *serial;
//     uint8_t rx_byte;
//     uint16_t rx_length;
//     uint32_t rx_pin;
//     uint32_t tx_pin;
// } drv_uart_cfg_t;
// static struct rt_uart_ops _uart_ops = {
//     // _uart_cfg,
//     // _uart_ctrl,
//     // _uart_putc,
//     // _uart_getc
// };
// drv_uart_cfg_t m_uart0_cfg = {
//     .uart = NRFX_UART_INSTANCE(0),
//     .rx_byte = 0,
//     .rx_length = 0,
//     .rx_pin = BSP_UART0_RX_PIN,
//     .tx_pin = BSP_UART0_TX_PIN,
//     .event_handler = uart0_event_hander
// };
int rt_hw_uart_init(void)
{
    return 1;
}
void rt_hw_board_init(void)
{
    
    //rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);

  //  rt_hw_uart_init();

  //  rt_console_set_device("uart0");

}
void rtthread_startup(void)
{
    /* init board */
    rt_hw_board_init();
    /* show RT-Thread version */
    rt_show_version();

    /* timer system initialization */
    rt_system_timer_init();

    /* scheduler system initialization */
    rt_system_scheduler_init();

    /* create init_thread */
    rt_application_init();

    /* timer thread initialization */
    rt_system_timer_thread_init();

    /* idle thread initialization */
    rt_thread_idle_init();

    /* start scheduler */
    rt_system_scheduler_start();
    /* init scheduler system */
    // rt_system_scheduler_init();
    // //     /* init application */
    // rt_application_init();

    // // /* init idle thread */
    //  rt_thread_idle_init();

    // // /* start scheduler */
    // rt_system_scheduler_start();

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
// rt_base_t rt_hw_interrupt_disable(void)
// {
//     return 1;
// }
// void rt_hw_interrupt_enable(rt_base_t level)
// {
    
// }

// void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to)
// {
    
// }
// void rt_hw_context_switch_interrupt(rt_uint32_t from, rt_uint32_t to)
// {
    
// }
// void rt_hw_context_switch_to(rt_uint32_t to)
// {
    
// }

// struct exception_stack_frame
// {
//     rt_uint32_t r0;
//     rt_uint32_t r1;
//     rt_uint32_t r2;
//     rt_uint32_t r3;
//     rt_uint32_t r12;
//     rt_uint32_t lr;
//     rt_uint32_t pc;
//     rt_uint32_t psr;
// };

// struct stack_frame
// {

//     /* r4 ~ r11 register */
//     rt_uint32_t r4;
//     rt_uint32_t r5;
//     rt_uint32_t r6;
//     rt_uint32_t r7;
//     rt_uint32_t r8;
//     rt_uint32_t r9;
//     rt_uint32_t r10;
//     rt_uint32_t r11;

//     struct exception_stack_frame exception_stack_frame;
// };

// rt_uint8_t *rt_hw_stack_init(void       *tentry,
//                              void       *parameter,
//                              rt_uint8_t *stack_addr,
//                              void       *texit)
// {
//     struct stack_frame *stack_frame;
//     rt_uint8_t         *stk;
//     unsigned long       i;

//     stk  = stack_addr + sizeof(rt_uint32_t);
//     stk  = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stk, 8);
//     stk -= sizeof(struct stack_frame);

//     stack_frame = (struct stack_frame *)stk;

//     /* init all register */
//     for (i = 0; i < sizeof(struct stack_frame) / sizeof(rt_uint32_t); i ++)
//     {
//         ((rt_uint32_t *)stack_frame)[i] = 0xdeadbeef;
//     }

//     stack_frame->exception_stack_frame.r0  = (unsigned long)parameter; /* r0 : argument */
//     stack_frame->exception_stack_frame.r1  = 0;                        /* r1 */
//     stack_frame->exception_stack_frame.r2  = 0;                        /* r2 */
//     stack_frame->exception_stack_frame.r3  = 0;                        /* r3 */
//     stack_frame->exception_stack_frame.r12 = 0;                        /* r12 */
//     stack_frame->exception_stack_frame.lr  = (unsigned long)texit;     /* lr */
//     stack_frame->exception_stack_frame.pc  = (unsigned long)tentry;    /* entry point, pc */
//     stack_frame->exception_stack_frame.psr = 0x01000000L;              /* PSR */

//     /* return task's current stack address */
//     return stk;
// }
