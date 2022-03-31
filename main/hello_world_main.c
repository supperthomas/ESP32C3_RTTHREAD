#include <stdio.h>
#include "driver/gpio.h"
#define BLINK_GPIO 8
#define LOOP_COUNT 50000000
#include "rtthread.h"
void app_main(void)
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
