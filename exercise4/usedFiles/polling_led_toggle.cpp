#include <cstdint>
#include <iostream>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

constexpr uint LED_PIN {7};
constexpr uint BUTTON_PIN {10};
constexpr std::uint32_t WORK_MS {500};  // pretend the main loop is busy with other work

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);

    bool led_on {false};
    bool last_button {false};
    unsigned int total_presses {0};

    std::cout << "polling_led_toggle started. Press the button." << std::endl;

    while (true)
    {
        const bool button {gpio_get(BUTTON_PIN)};

        if (button && !last_button)
        {
            led_on = !led_on;
            gpio_put(LED_PIN, led_on);
            ++total_presses;
            std::cout << "Presses counted: " << total_presses << std::endl;
        }
        last_button = button;

        sleep_ms(WORK_MS);
    }
}