#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "analog_sensors.h"
#include "button.h"
#include "pico/stdlib.h"
#include "sensor.h"

constexpr std::uint32_t POT_PIN {26};
constexpr std::uint32_t LIGHT_PIN {27};
constexpr std::uint32_t BUTTON_PIN {10};
constexpr std::size_t SENSOR_COUNT {4};
constexpr std::uint32_t UPDATE_MS {1000};

// Prints one line for any kind of Sensor.
void print_sensor(const hw::Sensor& sensor)
{
    // TODO 2: print the name, the value from read() and the unit, on one line.
}

int main()
{
    stdio_init_all();

    std::cout << "sensors started. Turn the knob, cover the light sensor, press the button." << std::endl;

    const hw::Potentiometer pot {POT_PIN};
    const hw::LightSensor light {LIGHT_PIN};
    const hw::ChipTemperature chip;
    const hw::Button button {BUTTON_PIN};

    // pointers to the base class: one array holds every kind of Sensor
    const std::array<const hw::Sensor*, SENSOR_COUNT> sensors {&pot, &light, &chip, &button};

    while (true)
    {
        // TODO 3: call print_sensor() for each sensor in sensors.

        std::cout << std::endl;
        sleep_ms(UPDATE_MS);
    }
}
