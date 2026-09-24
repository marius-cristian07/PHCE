#include "hardware/gpio.h"
#include "pico/stdlib.h"

constexpr unsigned LED_PIN = 7;

// Tells the C++ compiler to use C linkage for this function
extern "C" void led_set(unsigned pin, int value);

class LedController
{
public:
    explicit LedController(unsigned pin) : pin_(pin) {}

    void set(bool on) { led_set(pin_, on ? 1 : 0); }

private:
    unsigned pin_;
};

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LedController led(LED_PIN);
    led.set(true); // Turns on the LED via C++ class -> C function -> SDK

    while (true)
    {
        sleep_ms(1000);
    }
}