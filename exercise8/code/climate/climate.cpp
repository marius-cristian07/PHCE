#include <cstdint>
#include <iostream>

#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "pico/stdlib.h"
#include "shtc3.h"

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {400'000};  // 400 kHz
constexpr std::uint32_t UPDATE_MS {2000};

// addresses below 0x08 and above 0x77 are reserved
constexpr unsigned FIRST_ADDRESS {0x08};
constexpr unsigned LAST_ADDRESS {0x77};

// Prints the address of every device that answers.
void scan(hw::I2cBus& bus)
{
    std::cout << "I2C devices:";
    for (unsigned address {FIRST_ADDRESS}; address <= LAST_ADDRESS; ++address)
    {
        if (bus.probe(static_cast<std::uint8_t>(address)))
        {
            std::cout << " 0x" << std::hex << address << std::dec;
        }
    }
    std::cout << std::endl;
}

int main()
{
    stdio_init_all();

    std::cout << "climate started" << std::endl;

    // main owns the bus. It hands it to each device that needs it.
    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};

    // the bus goes in through the constructor
    hw::Shtc3 sensor {bus};
    scan(bus);

    while (true)
    {
        // TODO 3: call measure(). If it worked, print temperature() and humidity().
        //         If not, print "measure failed".

        sleep_ms(UPDATE_MS);
    }
}
