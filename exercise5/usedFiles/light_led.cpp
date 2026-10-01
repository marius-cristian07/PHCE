#include <array>
#include <cstdint>
#include <iostream>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/status_led.h"
#include "pico/stdlib.h"

constexpr uint LED_PIN {7};
constexpr uint LIGHT_PIN {27};        // GP27 is ADC input 1
constexpr uint LIGHT_ADC_INPUT {1};
constexpr std::uint16_t DARK_THRESHOLD {700};  // above this is dark
constexpr std::uint16_t OFFSET {50};           // gap on each side of the threshold

// Colours for the RGB LED on GP6.
constexpr std::array<std::uint32_t, 4> COLORS {
    PICO_COLORED_STATUS_LED_COLOR_FROM_RGB(40, 0, 0),   // red
    PICO_COLORED_STATUS_LED_COLOR_FROM_RGB(0, 40, 0),   // green
    PICO_COLORED_STATUS_LED_COLOR_FROM_RGB(0, 0, 40),   // blue
    PICO_COLORED_STATUS_LED_COLOR_FROM_RGB(40, 0, 40),  // purple
};

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    status_led_init();  // the RGB LED on GP6

    adc_init();
    adc_gpio_init(LIGHT_PIN);
    adc_select_input(LIGHT_ADC_INPUT);

    std::cout << "light_led started. Cover the sensor." << std::endl;

    bool is_dark {false};  // true: the LEDs are on
    std::size_t color_index {0};  // Keeps track of the active disco color

    while (true)
    {
        const std::uint16_t raw {adc_read()};  // higher when darker

        // TODO 1: Dual-threshold hysteresis logic to prevent LED flickering
        if (raw > (DARK_THRESHOLD + OFFSET))
        {
            is_dark = true;
        }
        else if (raw < (DARK_THRESHOLD - OFFSET))
        {
            is_dark = false;
        }

        gpio_put(LED_PIN, is_dark);

        // TODO 2 & 3: RGB LED Disco mode
        if (is_dark)
        {
            colored_status_led_set_on_with_color(COLORS[color_index]);
            color_index = (color_index + 1) % COLORS.size();  // Cycle 0 -> 1 -> 2 -> 3 -> 0
        }
        else
        {
            colored_status_led_set_state(false);
        }

        std::cout << "raw " << raw << "  LED " << (is_dark ? "ON" : "OFF") << std::endl;

        sleep_ms(200);
    }
}