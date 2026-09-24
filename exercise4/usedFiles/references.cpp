#include <iostream>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

constexpr uint LED_PIN {7};

void toggle_by_value(bool led_on)  // gets a copy
{
    led_on = !led_on;
}

void toggle_by_pointer(bool* led_on)  // gets the address
{
    // TODO 1: flip the bool that led_on points at
    if (led_on != nullptr)
    {
        *led_on = !(*led_on);
    }
}

void toggle_by_reference(bool& led_on)  // gets another name for the same bool
{
    // TODO 2: flip the bool
    led_on = !led_on;
}

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    bool led_on {false};

    while (true)
    {
        // --- STEP 2 TEST: Pass by value ---
        // toggle_by_value(led_on);

        // --- STEP 3 TEST: Pass by pointer ---
         toggle_by_pointer(&led_on);

        // --- STEP 4 TEST: Pass by reference ---
        // toggle_by_reference(led_on);

        gpio_put(LED_PIN, led_on);
        std::cout << "led_on is " << (led_on ? "ON" : "OFF") << std::endl;
        sleep_ms(500);
    }
}