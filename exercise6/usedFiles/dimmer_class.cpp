
#include <cstdint>
#include <iostream>

#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

constexpr std::uint32_t LED_PIN {7};
constexpr std::uint32_t POT_PIN {26};

// GP26 is ADC input 0, GP27 is input 1
constexpr std::uint32_t ADC_FIRST_PIN {26};
constexpr std::uint16_t PWM_WRAP {255};
constexpr std::uint32_t ADC_TO_PWM_SHIFT {4};

constexpr std::uint16_t FULL_ON_LEVEL {PWM_WRAP + 1};
constexpr int DEMO_UPDATES {100};
constexpr std::uint32_t UPDATE_MS {100};
constexpr std::uint32_t IDLE_MS {1000};

// One class does everything: ADC, PWM, maths and printing.
class Dimmer
{
public:
    // TODO 1: Member initializer list
    Dimmer(std::uint32_t led_pin, std::uint32_t pot_pin)
        : led_pin_ {led_pin},
          pot_pin_ {pot_pin},
          level_ {0}
    {
        adc_init();
        adc_gpio_init(pot_pin_);
        adc_select_input(pot_pin_ - ADC_FIRST_PIN);

        gpio_set_function(led_pin_, GPIO_FUNC_PWM);

        const std::uint32_t slice {
            pwm_gpio_to_slice_num(led_pin_)
        };

        pwm_set_wrap(slice, PWM_WRAP);
        pwm_set_enabled(slice, true);

        std::cout << "Dimmer created" << std::endl;
    }

    // Destructor
    ~Dimmer()
    {
        pwm_set_gpio_level(led_pin_, 0);
        std::cout << "Dimmer destroyed" << std::endl;
    }

    // Read potentiometer and update LED brightness
    void update()
    {
        const std::uint16_t raw {adc_read()};

        level_ = raw >> ADC_TO_PWM_SHIFT;

        if (level_ == PWM_WRAP)
        {
            level_ = FULL_ON_LEVEL;
        }

        pwm_set_gpio_level(led_pin_, level_);

        std::cout << "raw " << raw
                  << "  level " << level_
                  << std::endl;
    }

    // TODO 2: Const member function
    std::uint16_t level() const
    {
        return level_;
    }

private:
    std::uint32_t led_pin_;
    std::uint32_t pot_pin_;
    std::uint16_t level_;
};

// TODO 3: Print the last brightness level
void print_level(const Dimmer& dimmer)
{
    std::cout << "last level "
              << dimmer.level()
              << std::endl;
}

int main()
{
    stdio_init_all();

    {
        Dimmer dimmer {LED_PIN, POT_PIN};

        for (int i {0}; i < DEMO_UPDATES; ++i)
        {
            dimmer.update();
            sleep_ms(UPDATE_MS);
        }

        print_level(dimmer);

    } // Dimmer destructor runs here

    std::cout << "main() is done" << std::endl;

    while (true)
    {
        sleep_ms(IDLE_MS);
    }
}
