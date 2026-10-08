
#include <cstdint>
#include <iostream>

#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

constexpr std::uint32_t LED_PIN {7};
constexpr std::uint32_t POT_PIN {26};
constexpr std::uint32_t POT_ADC_INPUT {0};

constexpr std::uint16_t PWM_WRAP {4095};
constexpr std::uint32_t ADC_TO_PWM_SHIFT {0};
constexpr std::uint16_t FULL_ON_LEVEL {PWM_WRAP + 1};
constexpr std::uint32_t UPDATE_MS {100};

int main()
{
    stdio_init_all();

    adc_init();
    adc_gpio_init(POT_PIN);
    adc_select_input(POT_ADC_INPUT);

    // TODO 1
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    const std::uint32_t slice {pwm_gpio_to_slice_num(LED_PIN)};

    // TODO 2
    pwm_set_wrap(slice, PWM_WRAP);

    pwm_set_enabled(slice, true);

    std::cout << "pwm_led started. Turn the knob." << std::endl;

    while (true)
    {
        const std::uint16_t raw {adc_read()};

        // TODO 3
        std::uint16_t level {
            static_cast<std::uint16_t>(raw >> ADC_TO_PWM_SHIFT)
        };

        if (level == PWM_WRAP)
        {
            level = FULL_ON_LEVEL;
        }

        pwm_set_gpio_level(LED_PIN, level);

        std::cout << "raw " << raw
                  << "  level " << level << std::endl;

        sleep_ms(UPDATE_MS);
    }
}
