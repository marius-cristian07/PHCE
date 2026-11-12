#include <cstdint>
#include <iostream>
#include <string>

#include "hardware/i2c.h"
#include "i2c_bus.h"
#include "pico/stdlib.h"
#include "progress_bar.h"
#include "screen.h"
#include "ssd1306.h"
#include "widget.h"
#include "widgets.h"

constexpr std::uint32_t SDA_PIN {4};
constexpr std::uint32_t SCL_PIN {5};
constexpr std::uint32_t I2C_BAUDRATE {100'000};  // 100 kHz
constexpr std::uint32_t SETTLE_MS {1000};
constexpr std::uint32_t COUNT_FROM {100};  // seconds
constexpr std::uint32_t COUNT_MS {900};     // with the time to send the picture: 1 s
constexpr std::uint32_t BOOM_MS {1000};

// The buzzer: a tone that falls from high to low
constexpr std::uint32_t BUZZER_PIN {20};
constexpr std::uint32_t BOOM_START_US {300};  // half a wave, at the start
constexpr std::uint32_t BOOM_END_US {4000};   // half a wave, at the end
constexpr std::uint32_t BOOM_STEP_US {25};
constexpr auto WAVES_PER_STEP {2};
constexpr auto REPEATS {1000};  // how many times the widgets are drawn

const std::string COUNTDOWN_TEXT {"Bomb in "};
const std::string BOOM_TEXT {"BOOM!"};

// Where the widgets go, in pixels: the same as in Exercise 2
constexpr std::uint32_t LEFT {8};
constexpr std::uint32_t TITLE_Y {8};
constexpr std::uint32_t BAR_Y {24};
constexpr std::uint32_t BAR_WIDTH {112};
constexpr std::uint32_t BAR_HEIGHT {16};
constexpr std::uint32_t SCREEN_HEIGHT {hw::Ssd1306::PAGES * hw::Ssd1306::PAGE_HEIGHT};

// A falling tone on the buzzer.
void boom()
{
    for (std::uint32_t half_wave_us {BOOM_START_US}; half_wave_us <= BOOM_END_US;
         half_wave_us += BOOM_STEP_US)
    {
        for (auto wave {0}; wave < WAVES_PER_STEP; ++wave)
        {
            gpio_put(BUZZER_PIN, true);
            sleep_us(half_wave_us);
            gpio_put(BUZZER_PIN, false);
            sleep_us(half_wave_us);
        }
    }
}

int main()
{
    stdio_init_all();

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    std::cout << "crtp started" << std::endl;

    hw::I2cBus bus {i2c0, SDA_PIN, SCL_PIN, I2C_BAUDRATE};
    if (!bus.is_free())
    {
        std::cout << "I2C bus is stuck. Take the power off the Pico and the PicoBricks, "
                  << "then put it back." << std::endl;
    }
    hw::Ssd1306 display {bus};
    gui::Screen screen {display};

    // The time is taken on the picture that every countdown ends with: BOOM! and an
    // empty bar. So each round measures the same picture.
    gui::Label title {LEFT, TITLE_Y, BOOM_TEXT};
    const gui::Rectangle frame {0, 0, hw::Ssd1306::WIDTH, SCREEN_HEIGHT};
    gui::ProgressBar bar {LEFT, BAR_Y, BAR_WIDTH, BAR_HEIGHT};

    // The first moments after a reset are not steady: wait before timing
    sleep_ms(SETTLE_MS);

    while (true)
    {
        screen.clear();

        // Static: the same loop as in Exercise 2
        const std::uint32_t static_start_us {time_us_32()};
        for (auto i {0}; i < REPEATS; ++i)
        {
            screen.draw_all(title, bar, frame);
        }
        const std::uint32_t static_us {time_us_32() - static_start_us};

        std::cout << "static:  " << static_us << " us for " << REPEATS << " frames" << std::endl;
        std::cout << "sizeof: Rectangle " << sizeof(gui::Rectangle) << ", Label "
                  << sizeof(gui::Label) << " bytes" << std::endl;
        std::cout << "sizeof: ProgressBar " << sizeof(gui::ProgressBar) << " bytes" << std::endl;
        std::cout << std::endl;

        // Then the show: a countdown. The bar is the fuse.
        for (std::uint32_t count {COUNT_FROM}; count > 0; --count)
        {
            title.set_text(COUNTDOWN_TEXT + std::to_string(count) + "s");
            bar.set_percent(count * gui::ProgressBar::MAX_PERCENT / COUNT_FROM);
            screen.clear();
            screen.draw_all(title, bar, frame);
            screen.show();
            sleep_ms(COUNT_MS);
        }

        title.set_text(BOOM_TEXT);
        bar.set_percent(0);
        screen.clear();
        screen.draw_all(title, bar, frame);
        screen.show();
        boom();
        sleep_ms(BOOM_MS);
    }
}
