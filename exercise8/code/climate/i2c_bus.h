#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <cstddef>
#include <cstdint>

#include "hardware/i2c.h"

namespace hw
{
    // One I2C bus: a controller and its SDA and SCL pins. Many devices can share it.
    class I2cBus
    {
    public:
        // If a device still holds SDA low from before a reset, this frees the bus first.
        I2cBus(i2c_inst_t* i2c, std::uint32_t sda_pin, std::uint32_t scl_pin,
               std::uint32_t baudrate);

        // Both return true if all length bytes went through. They give up on a byte that
        // takes too long, so a stuck bus does not stop the program.
        bool write(std::uint8_t address, const std::uint8_t* data, std::size_t length);
        bool read(std::uint8_t address, std::uint8_t* data, std::size_t length);

        // true if a device answers at address
        bool probe(std::uint8_t address);

        // false if a device holds SDA or SCL low. Then nothing can be sent.
        bool is_free() const;

    private:
        i2c_inst_t* i2c_;  // i2c0 or i2c1
        std::uint32_t sda_pin_;
        std::uint32_t scl_pin_;
    };
}

#endif  // I2C_BUS_H
