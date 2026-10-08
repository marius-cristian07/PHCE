#include "application.h"

#include <cstdint>
#include <iostream>

#include "pico/stdlib.h"

namespace
{
    constexpr std::uint32_t ADC_TO_PWM_SHIFT {4};  // 12-bit pot value to 8-bit LED level
    constexpr std::uint32_t UPDATE_MS {100};
}

app::Application::Application(const hw::Potentiometer& pot, hw::PwmLed& led)
    : pot_ {pot}, led_ {led}
{
}

void app::Application::run()
{
    while (true)
    {
        const std::uint16_t raw {pot_.read()};

        const std::uint16_t level {
            static_cast<std::uint16_t>(raw >> ADC_TO_PWM_SHIFT)
        };

        led_.set_level(level);

        std::cout << "raw " << raw
                  << "  level " << led_.level()
                  << std::endl;

        sleep_ms(UPDATE_MS);
    }
}
