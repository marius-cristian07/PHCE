#ifndef ANALOG_SENSORS_H
#define ANALOG_SENSORS_H

#include <cstdint>
#include <string>

#include "adc_sensor.h"

namespace hw
{
    // The potentiometer. read() gives how far it is turned, 0 to 100 %.
    class Potentiometer : public AdcSensor
    {
    public:
        explicit Potentiometer(std::uint32_t pin);

        float read() const override;
        std::string unit() const override;
    };

    // The light sensor. read() gives how bright it is, 0 to 100 %.
    class LightSensor : public AdcSensor
    {
    public:
        explicit LightSensor(std::uint32_t pin);

        float read() const override;
        std::string unit() const override;
    };

    // The temperature sensor inside the RP2040. It has no pin: it is ADC input 4.
    class ChipTemperature : public AdcSensor
    {
    public:
        ChipTemperature();

        float read() const override;
        std::string unit() const override;
    };
}

#endif  // ANALOG_SENSORS_H
