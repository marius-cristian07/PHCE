#include "analog_sensors.h"

#include <cstdint>
#include <string>

#include "hardware/adc.h"

namespace
{
    constexpr std::uint32_t ADC_FIRST_PIN {26};     // GP26 is ADC input 0, so input = pin - 26
    constexpr std::uint32_t TEMPERATURE_INPUT {4};  // the chip's own sensor
    constexpr float PERCENT {100.0f};
    constexpr float ADC_VOLTS {3.3f};  // the voltage at MAX_RAW

    // RP2040 datasheet: T = 27 - (V - 0.706) / 0.001721
    constexpr float REFERENCE_CELSIUS {27.0f};
    constexpr float REFERENCE_VOLTS {0.706f};
    constexpr float VOLTS_PER_CELSIUS {0.001721f};
}

hw::Potentiometer::Potentiometer(std::uint32_t pin)
    : AdcSensor {"Potentiometer", pin - ADC_FIRST_PIN}
{
    adc_gpio_init(pin);
}

float hw::Potentiometer::read() const
{
    return PERCENT * read_raw() / MAX_RAW;
}

std::string hw::Potentiometer::unit() const
{
    return "%";
}

hw::LightSensor::LightSensor(std::uint32_t pin)
    : AdcSensor {"Light", pin - ADC_FIRST_PIN}
{
    adc_gpio_init(pin);
}

float hw::LightSensor::read() const
{
    // TODO 1: less light gives a higher raw value. Return the brightness, 0 to 100 %.
    return 0.0f;
}

std::string hw::LightSensor::unit() const
{
    return "%";
}

hw::ChipTemperature::ChipTemperature()
    : AdcSensor {"Chip temperature", TEMPERATURE_INPUT}
{
    adc_set_temp_sensor_enabled(true);
}

float hw::ChipTemperature::read() const
{
    // volts first, then degrees Celsius
    const float volts {ADC_VOLTS * read_raw() / MAX_RAW};
    return REFERENCE_CELSIUS - (volts - REFERENCE_VOLTS) / VOLTS_PER_CELSIUS;
}

std::string hw::ChipTemperature::unit() const
{
    return "C";
}
