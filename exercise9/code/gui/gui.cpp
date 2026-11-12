#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "pico/stdlib.h"
#include "progress_bar.h"
#include "screen.h"
#include "shape.h"
#include "ssd1306.h"
#include "text.h"
#include "widget.h"

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t POT_PIN {26};
constexpr std::uint32_t POT_ADC_INPUT {0};  // GP26 is ADC input 0
constexpr std::uint32_t FULL_RAW {4000};    // The knob at its end. 12 bits go to 4095
constexpr auto POT_SAMPLES {64};
constexpr std::uint32_t POT_SAMPLE_GAP_US {100};
constexpr std::uint32_t POT_DEAD_BAND {10};  // Smaller changes than this are noise
constexpr std::uint32_t UPDATE_MS {100};

// Where the widgets go, in pixels
constexpr std::uint32_t LEFT {8};
constexpr std::uint32_t TITLE_Y {0};
constexpr std::uint32_t BAR_Y {24};
constexpr std::uint32_t BAR_WIDTH {112};
constexpr std::uint32_t BAR_HEIGHT {16};
constexpr std::size_t WIDGET_COUNT {1};

// How far the pot is turned, 0 to 100 %.
std::uint32_t read_pot_percent()
{
    adc_select_input(POT_ADC_INPUT);

    // One reading jumps up and down by about 100. The noise comes in waves, so the
    // readings are spread over a few milliseconds. Their average is steady.
    std::uint32_t sum {0};
    for (auto sample {0}; sample < POT_SAMPLES; ++sample)
    {
        sum += adc_read();
        sleep_us(POT_SAMPLE_GAP_US);
    }
    const std::uint32_t average {sum / POT_SAMPLES};

    // The value in use only follows a change larger than the dead band. So the percent
    // does not flip when the knob sits between two numbers.
    static std::uint32_t raw {0};  // Static: it keeps its value between calls
    const std::uint32_t change {average > raw ? average - raw : raw - average};
    if (change > POT_DEAD_BAND)
    {
        raw = average;
    }

    // Everything from FULL_RAW up is 100 %
    return std::min(raw * gui::ProgressBar::MAX_PERCENT / FULL_RAW,
                    gui::ProgressBar::MAX_PERCENT);
}

int main()
{
    stdio_init_all();

    std::cout << "GUI started. Turn the knob." << std::endl;

    adc_init();
    adc_gpio_init(POT_PIN);

    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    if (!bus.is_free())
    {
        std::cout << "I2C bus is stuck. Take the power off the Pico and the PicoBricks, "
                  << "then put it back." << std::endl;
    }
    hw::Ssd1306 display {bus};
    gui::Screen screen {display};

    const gui::Label title {LEFT, TITLE_Y, "Potentiometer"};
    gui::ProgressBar bar {LEFT, BAR_Y, BAR_WIDTH, BAR_HEIGHT};

    // Pointers to the base class: one array holds every kind of Widget
    // TODO 3: add bar to widgets. Make WIDGET_COUNT 2.
    const std::array<const gui::Widget*, WIDGET_COUNT> widgets {&title};

    while (true)
    {
        bar.set_percent(read_pot_percent());

        screen.clear();
        for (const gui::Widget* widget : widgets)
        {
            screen.draw(*widget);
        }
        screen.show();

        sleep_ms(UPDATE_MS);
    }
}
