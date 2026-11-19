#include <cstdint>
#include <cstdio>

#include "c_log.h"
#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "log.h"
#include "pico/stdlib.h"
#include "shtc3.h"

using logging::Severity;

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t UPDATE_MS {2000};

// Humidity in percent. Breathe on the sensor to get there.
constexpr float HUMIDITY_WARNING {70.0f};
constexpr float HUMIDITY_ERROR {85.0f};

int main()
{
    stdio_init_all();

    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    hw::Shtc3 sensor {bus};

    auto sample {0};

    while (true)
    {
        ++sample;
        std::printf("sample %d\n", sample);

        // Every message is logged twice: the C way and the C++ way
        C_LOG_DEBUG("measuring");
        logging::log<Severity::Debug>("measuring");

        if (sensor.measure())
        {
            const float temperature {sensor.temperature()};
            const float humidity {sensor.humidity()};

            // %.1f prints a float with one decimal. %% prints a percent sign.
            C_LOG_INFO("%.1f C, %.1f %%", temperature, humidity);
            logging::log<Severity::Info>("%.1f C, %.1f %%", temperature, humidity);

            // TODO 3: if humidity is HUMIDITY_ERROR or more, log an error "too wet" both
            //         ways. If not, but it is HUMIDITY_WARNING or more, log a warning
            //         "getting wet" both ways.

            if (humidity >= HUMIDITY_ERROR)
            {
                C_LOG_ERROR("too wet");
                logging::log<Severity::Error>("too wet");
            }
            else if (humidity >= HUMIDITY_WARNING)
            {
                C_LOG_WARNING("getting wet");
                logging::log<Severity::Warning>("getting wet");
            }
        }
        else
        {
            C_LOG_ERROR("measure failed");
            logging::log<Severity::Error>("measure failed");
        }

        sleep_ms(UPDATE_MS);
    }
}
