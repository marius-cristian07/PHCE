
#include <cstdint>
#include <iostream>

#include "pico/stdlib.h"

constexpr std::uint32_t LED_PIN {7};
constexpr std::uint32_t BLINK_MS {1000};

// TODO 1: Define the Led class
class Led
{
private:
    std::uint32_t pin_;
    bool state_;

public:
    // Constructor
    explicit Led(std::uint32_t pin)
        : pin_ {pin}, state_ {false}
    {
        gpio_init(pin_);
        gpio_set_dir(pin_, GPIO_OUT);
        gpio_put(pin_, false);
    }

    // Turn LED on
    void on()
    {
        state_ = true;
        gpio_put(pin_, true);
    }

    // Turn LED off
    void off()
    {
        state_ = false;
        gpio_put(pin_, false);
    }

    // Toggle LED state
    void toggle()
    {
        if (state_)
        {
            off();
        }
        else
        {
            on();
        }
    }

    // Return current LED state
    bool is_on() const
    {
        return state_;
    }
};

int main()
{
    stdio_init_all();

    // TODO 2: Create an Led object
    Led led {LED_PIN};

    while (true)
    {
        // TODO 3: Toggle and print LED state
        led.toggle();

        if (led.is_on())
        {
            std::cout << "LED is on" << std::endl;
        }
        else
        {
            std::cout << "LED is off" << std::endl;
        }

        sleep_ms(BLINK_MS);
    }
}
