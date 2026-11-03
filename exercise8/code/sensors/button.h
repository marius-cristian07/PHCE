#ifndef BUTTON_H
#define BUTTON_H

#include <cstdint>
#include <string>

#include "sensor.h"

namespace hw
{
    // The PicoBricks button. A Sensor, but not on the ADC: read() gives 1 when pressed.
    class Button : public Sensor
    {
    public:
        explicit Button(std::uint32_t pin);

        float read() const override;
        std::string unit() const override;

    private:
        std::uint32_t pin_;
    };
}

#endif  // BUTTON_H
