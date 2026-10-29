#include "button.h"

#include <cstdint>
#include <string>

#include "hardware/gpio.h"

namespace
{
    constexpr float PRESSED {1.0f};
    constexpr float RELEASED {0.0f};
}

hw::Button::Button(std::uint32_t pin) : Sensor {"Button"}, pin_ {pin}
{
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_down(pin_);
}

float hw::Button::read() const
{
    return gpio_get(pin_) ? PRESSED : RELEASED;
}

std::string hw::Button::unit() const
{
    return "(1 = pressed)";
}
