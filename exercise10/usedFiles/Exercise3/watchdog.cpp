#include <cstdint>

#include "hardware/i2c.h"
#include "hardware/watchdog.h"
#include "i2c_bus.h"
#include "log.h"
#include "pico/stdlib.h"
#include "shtc3.h"

using logging::Severity;

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t BUTTON_PIN {10};

constexpr std::uint32_t UPDATE_MS {1000};
constexpr std::uint32_t WATCHDOG_MS {3000};

// The watchdog waits while a debugger has stopped the Pico
constexpr bool PAUSE_ON_DEBUG {true};

int main()
{
    stdio_init_all();

    // TODO 1: if the watchdog caused this start, log a warning "the watchdog reset the
    //         Pico". If not, log an info "normal start".

    if (watchdog_caused_reboot())
    {
        logging::log<Severity::Warning>("the watchdog reset the Pico");
    }
    else
    {
        logging::log<Severity::Info>("normal start");
    }

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);

    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    hw::Shtc3 sensor {bus};

    // TODO 2: start the watchdog. It resets the Pico after WATCHDOG_MS.

    watchdog_enable(WATCHDOG_MS, PAUSE_ON_DEBUG);

    while (true)
    {
        // The bug: with the button down, the program never gets out of this loop
        if (gpio_get(BUTTON_PIN))
        {
            logging::log<Severity::Error>("stuck");
            while (true)
            {
                tight_loop_contents();
            }
        }

        if (sensor.measure())
        {
            logging::log<Severity::Info>("%.1f C, %.1f %%", sensor.temperature(),
                                         sensor.humidity());
        }
        else
        {
            logging::log<Severity::Error>("measure failed");
        }

        // TODO 3: tell the watchdog that the program is still running.
        watchdog_update();
        sleep_ms(UPDATE_MS);
    }
}
