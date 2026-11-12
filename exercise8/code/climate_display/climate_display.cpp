#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "pico/stdlib.h"
#include "shtc3.h"
#include "ssd1306.h"

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t UPDATE_MS {2000};

constexpr int DECIMALS {1};
constexpr std::uint32_t LEFT {0};  // column
constexpr std::uint32_t TITLE_PAGE {0};
constexpr std::uint32_t TEMPERATURE_PAGE {2};
constexpr std::uint32_t HUMIDITY_PAGE {3};

// Turns 23.456 into "23.5".
std::string one_decimal(float value)
{
    std::ostringstream text;
    text << std::fixed << std::setprecision(DECIMALS) << value;
    return text.str();
}

int main()
{
    stdio_init_all();

    std::cout << "climate_display started" << std::endl;

    // one bus, two devices on it
    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    if (!bus.is_free())
    {
        std::cout << "I2C bus is stuck. Take the power off the Pico and the PicoBricks, "
                  << "then put it back." << std::endl;
    }
    hw::Shtc3 sensor {bus};
    // the same bus goes into the display
    hw::Ssd1306 display {bus};

    while (true)
    {
        display.clear();
        display.draw_text(LEFT, TITLE_PAGE, "Climate");
        if (sensor.measure())
        {
            // TODO 2: draw the temperature on TEMPERATURE_PAGE and the humidity on
            //         HUMIDITY_PAGE. one_decimal() turns a float into text.
        }
        else
        {
            display.draw_text(LEFT, TEMPERATURE_PAGE, "measure failed");
        }

        const bool shown {display.show()};
        std::cout << "show() " << (shown ? "worked" : "failed") << std::endl;

        sleep_ms(UPDATE_MS);
    }
}
