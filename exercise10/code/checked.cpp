#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "pico/stdlib.h"
#include "result.h"

// Raw temperatures, as the SHTC3 sends them. Two of them are not measurements: a bus
// that is stuck low gives 0x0000, a bus with nothing on it gives 0xFFFF.
constexpr std::size_t SAMPLE_COUNT {5};
constexpr std::array<std::uint16_t, SAMPLE_COUNT> RAW_SAMPLES {
    0x6666, 0x0000, 0x6800, 0xFFFF, 0x6A00};

// Datasheet: T = -45 + 175 * raw / 65536
constexpr float FULL_SCALE {65536.0f};
constexpr float TEMPERATURE_OFFSET {-45.0f};
constexpr float TEMPERATURE_SPAN {175.0f};

// The SHTC3 measures from -40 to 125 C
constexpr float MIN_C {-40.0f};
constexpr float MAX_C {125.0f};

constexpr std::uint32_t IDLE_MS {1000};

hw::Result<float, hw::Error> to_celsius(std::uint16_t raw)
{
    const float celsius {
        TEMPERATURE_OFFSET + (TEMPERATURE_SPAN * static_cast<float>(raw) / FULL_SCALE)};

    // TODO 1: return hw::Error::OutOfRange if celsius is below MIN_C or above MAX_C.

    return celsius;
}

int main()
{
    stdio_init_all();

    for (const std::uint16_t raw : RAW_SAMPLES)
    {
        const hw::Result<float, hw::Error> result {to_celsius(raw)};

        // TODO 2: only print the value if the result is ok. If not, print "error, " and
        //         the error as text.
        std::cout << result.value() << " C" << std::endl;
    }

    while (true)
    {
        sleep_ms(IDLE_MS);
    }
}
