#ifndef ADC_SENSOR_H
#define ADC_SENSOR_H

#include <cstdint>
#include <string>

#include "sensor.h"

namespace hw
{
    // A Sensor on one ADC input. Still abstract: it does not know what its value means.
    class AdcSensor : public Sensor
    {
    public:
        static constexpr std::uint16_t MAX_RAW {4095};  // the largest 12-bit value

    protected:
        AdcSensor(const std::string& name, std::uint32_t adc_input);

        // for derived classes: the raw value, 0 to MAX_RAW
        std::uint16_t read_raw() const;

    private:
        std::uint32_t adc_input_;
    };
}

#endif  // ADC_SENSOR_H
