#include "hardware/gpio.h"

void led_set(unsigned pin, int value)
{
    gpio_put(pin, value);
}