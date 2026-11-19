#include <cstdint>
#include <iostream>
#include <limits>

#include "hardware/adc.h"
#include "pico/stdlib.h"

constexpr std::uint32_t HARDWARE_BITS {12};  // the ADC of the Pico measures 12 bits
constexpr std::uint32_t MIN_BITS {8};
constexpr std::uint32_t ADC_BITS {12};  // the bits this program keeps
constexpr std::uint32_t ADC_MAX {(1U << ADC_BITS) - 1};
constexpr float ADC_VREF {3.3f};

// Only these pins reach the ADC
constexpr std::uint32_t FIRST_ADC_PIN {26};
constexpr std::uint32_t LAST_ADC_PIN {28};
constexpr std::uint32_t POT_PIN {26};

constexpr std::uint32_t UPDATE_MS {1000};

// The type a reading is stored in
using Sample = std::uint16_t;

// TODO 1: stop the build if ADC_BITS is below MIN_BITS or above HARDWARE_BITS.
static_assert(ADC_BITS >= MIN_BITS && ADC_BITS <= HARDWARE_BITS,
              "ADC_BITS must be between MIN_BITS and HARDWARE_BITS");
// TODO 2: stop the build if a Sample can't hold ADC_MAX.
static_assert(std::numeric_limits<Sample>::max() >= ADC_MAX,
              "Sample cannot hold ADC_MAX");

// One ADC input. The pin is a template argument, so the compiler knows it.
template <std::uint32_t Pin>
class AdcInput
{
    // TODO 3: stop the build if Pin is not from FIRST_ADC_PIN to LAST_ADC_PIN.
    static_assert(Pin >= FIRST_ADC_PIN && Pin <= LAST_ADC_PIN,
              "ADC pin must be between GP26 and GP28");
public:
    AdcInput()
    {
        adc_init();
        adc_gpio_init(Pin);
    }

    // From 0 to ADC_MAX
    Sample read() const
    {
        adc_select_input(Pin - FIRST_ADC_PIN);
        return static_cast<Sample>(adc_read() >> (HARDWARE_BITS - ADC_BITS));
    }
};

float to_volts(Sample raw)
{
    return ADC_VREF * static_cast<float>(raw) / static_cast<float>(ADC_MAX);
}

int main()
{
    stdio_init_all();

    AdcInput<POT_PIN> pot;

    std::cout << "ADC_BITS " << ADC_BITS << ", ADC_MAX " << ADC_MAX << std::endl;

    while (true)
    {
        const Sample raw {pot.read()};
        std::cout << raw << " = " << to_volts(raw) << " V" << std::endl;
        sleep_ms(UPDATE_MS);
    }
}
