#include "hardware/gpio.h"
#include "pico/stdlib.h"

constexpr uint LED_PIN = 7;
constexpr uint BUTTON_PIN = 10;

int main()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);  // enable internal pull-down: unpressed reads LOW

    while (true)
    {
        gpio_put(LED_PIN, gpio_get(BUTTON_PIN));  // gpio_get: read the pin's current level
        sleep_ms(10);
    }
}
