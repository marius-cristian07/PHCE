#include <cstdint>
#include <cstdio>
#include <exception>
#include <string_view>

#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "pico/stdlib.h"
#include "result.h"
#include "shtc3.h"

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};

// A second bus, on pins where nothing is connected
constexpr std::uint32_t EMPTY_SDA_PIN {2};
constexpr std::uint32_t EMPTY_SCL_PIN {3};

constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t UPDATE_MS {2000};
constexpr auto REPEATS {1000};  // how many times each way fails for the clock

// Way 1: the error is the return value.
hw::Result<float, hw::Error> read_temperature(const hw::I2cBus& bus, hw::Shtc3& sensor)
{
    // TODO 1: return hw::Error::BusStuck if the bus is not free.
    //         Return hw::Error::NoAnswer if measure() fails.

    return sensor.temperature();
}

void print_result(std::string_view name, const hw::I2cBus& bus, hw::Shtc3& sensor)
{
    // %.*s prints a string_view: first its size, then its characters
    const hw::Result<float, hw::Error> result {read_temperature(bus, sensor)};
    if (result.ok())
    {
        std::printf("%.*s: %.1f C\n", static_cast<int>(name.size()), name.data(),
                    result.value());
    }
    else
    {
        const std::string_view text {hw::to_string(result.error())};
        std::printf("%.*s: error, %.*s\n", static_cast<int>(name.size()), name.data(),
                    static_cast<int>(text.size()), text.data());
    }
}

// The compiler defines __cpp_exceptions only when exceptions are switched on.
#ifdef __cpp_exceptions

// What gets thrown: an object that carries the Error.
class SensorError : public std::exception
{
public:
    explicit SensorError(hw::Error error) : error_ {error} {}

    hw::Error error() const { return error_; }
    // what() comes from std::exception, which fixes its type: it can't return a
    // string_view. The texts in to_string() are literals, so data() ends with '\0'.
    const char* what() const noexcept override { return hw::to_string(error_).data(); }

private:
    hw::Error error_;
};

// Way 2: the error is thrown.
float read_temperature_or_throw(const hw::I2cBus& bus, hw::Shtc3& sensor)
{
    // TODO 2: the same two checks as in TODO 1. Throw a SensorError with the error.

    return sensor.temperature();
}

void print_or_catch(std::string_view name, const hw::I2cBus& bus, hw::Shtc3& sensor)
{
    try
    {
        const float temperature {read_temperature_or_throw(bus, sensor)};
        std::printf("%.*s: %.1f C\n", static_cast<int>(name.size()), name.data(),
                    temperature);
    }
    catch (const SensorError& error)
    {
        std::printf("%.*s: caught, %s\n", static_cast<int>(name.size()), name.data(),
                    error.what());
    }
}

#endif  // __cpp_exceptions

int main()
{
    stdio_init_all();

    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    hw::Shtc3 inside {bus};

    // Nothing is connected to this bus, so this sensor never answers
    hw::I2cBus empty_bus {i2c1, EMPTY_SDA_PIN, EMPTY_SCL_PIN, I2C_BAUDRATE};
    hw::Shtc3 outside {empty_bus};

    // The clock: REPEATS reads that fail, each way
    auto failed {0};
    std::uint32_t start {time_us_32()};
    for (auto repeat {0}; repeat < REPEATS; ++repeat)
    {
        if (!read_temperature(empty_bus, outside).ok())
        {
            ++failed;
        }
    }
    const std::uint32_t result_us {time_us_32() - start};
    std::printf("%d failed reads with Result: %u us\n", failed,
                static_cast<unsigned>(result_us));

#ifdef __cpp_exceptions
    failed = 0;
    start = time_us_32();
    for (auto repeat {0}; repeat < REPEATS; ++repeat)
    {
        try
        {
            read_temperature_or_throw(empty_bus, outside);
        }
        catch (const SensorError&)
        {
            ++failed;
        }
    }
    const std::uint32_t exception_us {time_us_32() - start};
    std::printf("%d failed reads with exceptions: %u us\n", failed,
                static_cast<unsigned>(exception_us));
#else
    std::printf("exceptions are off\n");
#endif

    while (true)
    {
        print_result("inside", bus, inside);
        print_result("outside", empty_bus, outside);

#ifdef __cpp_exceptions
        print_or_catch("inside", bus, inside);
        print_or_catch("outside", empty_bus, outside);
#endif

        sleep_ms(UPDATE_MS);
    }
}
