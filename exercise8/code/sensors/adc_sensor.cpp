#include "adc_sensor.h"

#include <cstdint>
#include <string>

#include "hardware/adc.h"

hw::AdcSensor::AdcSensor(const std::string& name, std::uint32_t adc_input)
    : Sensor {name}, adc_input_ {adc_input}
{
    adc_init();
}

std::uint16_t hw::AdcSensor::read_raw() const
{
    // another object may have switched the ADC to its own input
    adc_select_input(adc_input_);
    return adc_read();
}
