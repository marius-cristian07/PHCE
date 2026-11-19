#include "i2c_bus.h"

#include <cstddef>
#include <cstdint>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/time.h"

namespace
{
    // the longest wait for one byte. At 100 kHz a byte takes about 90 us.
    constexpr std::uint32_t BYTE_TIMEOUT_US {10'000};

    // freeing a stuck bus: a byte and its answer bit are 9 clock pulses
    constexpr auto MAX_CLEAR_PULSES {9};
    constexpr std::uint32_t CLEAR_HALF_PERIOD_US {5};

    // A device that was cut off in the middle of a byte keeps SDA low. A reset of the
    // Pico does not reach it. It lets go after a few clock pulses.
    void clear_bus(std::uint32_t sda_pin, std::uint32_t scl_pin)
    {
        // plain inputs with pull-ups. Made an output, a pin pulls its line low.
        gpio_init(sda_pin);
        gpio_init(scl_pin);
        gpio_pull_up(sda_pin);
        gpio_pull_up(scl_pin);
        sleep_us(CLEAR_HALF_PERIOD_US);

        if (gpio_get(sda_pin))
        {
            return;  // nobody holds SDA
        }

        for (auto pulse {0}; pulse < MAX_CLEAR_PULSES && !gpio_get(sda_pin); ++pulse)
        {
            gpio_set_dir(scl_pin, GPIO_OUT);
            sleep_us(CLEAR_HALF_PERIOD_US);
            gpio_set_dir(scl_pin, GPIO_IN);
            sleep_us(CLEAR_HALF_PERIOD_US);
        }

        // a STOP: SDA goes from low to high while SCL is high. It ends the transfer.
        gpio_set_dir(sda_pin, GPIO_OUT);
        sleep_us(CLEAR_HALF_PERIOD_US);
        gpio_set_dir(sda_pin, GPIO_IN);
        sleep_us(CLEAR_HALF_PERIOD_US);
    }
}

hw::I2cBus::I2cBus(i2c_inst_t* i2c, std::uint32_t sda_pin, std::uint32_t scl_pin,
                   std::uint32_t baudrate)
    : i2c_ {i2c}, sda_pin_ {sda_pin}, scl_pin_ {scl_pin}
{
    clear_bus(sda_pin_, scl_pin_);

    i2c_init(i2c_, baudrate);
    gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin_);
    gpio_pull_up(scl_pin_);
}

bool hw::I2cBus::write(std::uint8_t address, const std::uint8_t* data, std::size_t length)
{
    const int written {
        i2c_write_timeout_per_char_us(i2c_, address, data, length, false, BYTE_TIMEOUT_US)};
    return written == static_cast<int>(length);
}

bool hw::I2cBus::read(std::uint8_t address, std::uint8_t* data, std::size_t length)
{
    const int received {
        i2c_read_timeout_per_char_us(i2c_, address, data, length, false, BYTE_TIMEOUT_US)};
    return received == static_cast<int>(length);
}

bool hw::I2cBus::probe(std::uint8_t address)
{
    // a device that is there answers a read of one byte
    std::uint8_t ignored {0};
    return read(address, &ignored, 1);
}

bool hw::I2cBus::is_free() const
{
    // between transfers both lines are high
    return gpio_get(sda_pin_) && gpio_get(scl_pin_);
}
