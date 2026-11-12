#include "ssd1306.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "font.h"

namespace
{
    // The first byte of each write: what the rest is
    constexpr std::uint8_t COMMAND_FOLLOWS {0x00};
    constexpr std::uint8_t PIXELS_FOLLOW {0x40};

    // Commands and their values, from the SSD1306 datasheet
    constexpr std::uint8_t DISPLAY_OFF {0xAE};
    constexpr std::uint8_t DISPLAY_ON {0xAF};
    constexpr std::uint8_t SET_CLOCK {0xD5};
    constexpr std::uint8_t CLOCK_DEFAULT {0x80};
    constexpr std::uint8_t SET_MULTIPLEX {0xA8};
    constexpr std::uint8_t ROWS_64 {0x3F};  // 64 rows, counted from 0
    constexpr std::uint8_t SET_OFFSET {0xD3};
    constexpr std::uint8_t NO_OFFSET {0x00};
    constexpr std::uint8_t START_LINE_0 {0x40};
    constexpr std::uint8_t CHARGE_PUMP {0x8D};
    constexpr std::uint8_t CHARGE_PUMP_ON {0x14};
    constexpr std::uint8_t MEMORY_MODE {0x20};
    constexpr std::uint8_t HORIZONTAL {0x00};  // left to right, then the next page
    constexpr std::uint8_t MIRROR_COLUMNS {0xA1};
    constexpr std::uint8_t MIRROR_ROWS {0xC8};
    constexpr std::uint8_t COM_PINS {0xDA};
    constexpr std::uint8_t COM_PINS_128X64 {0x12};
    constexpr std::uint8_t CONTRAST {0x81};
    constexpr std::uint8_t CONTRAST_HIGH {0xCF};
    constexpr std::uint8_t PRECHARGE {0xD9};
    constexpr std::uint8_t PRECHARGE_DEFAULT {0xF1};
    constexpr std::uint8_t VCOM_DETECT {0xDB};
    constexpr std::uint8_t VCOM_DEFAULT {0x40};
    constexpr std::uint8_t SHOW_MEMORY {0xA4};
    constexpr std::uint8_t NOT_INVERTED {0xA6};
    constexpr std::uint8_t COLUMN_RANGE {0x21};
    constexpr std::uint8_t PAGE_RANGE {0x22};

    constexpr std::array INIT_COMMANDS {
        DISPLAY_OFF,
        SET_CLOCK, CLOCK_DEFAULT,
        SET_MULTIPLEX, ROWS_64,
        SET_OFFSET, NO_OFFSET,
        START_LINE_0,
        CHARGE_PUMP, CHARGE_PUMP_ON,
        MEMORY_MODE, HORIZONTAL,
        MIRROR_COLUMNS,
        MIRROR_ROWS,
        COM_PINS, COM_PINS_128X64,
        CONTRAST, CONTRAST_HIGH,
        PRECHARGE, PRECHARGE_DEFAULT,
        VCOM_DETECT, VCOM_DEFAULT,
        SHOW_MEMORY,
        NOT_INVERTED,
        DISPLAY_ON,
    };

    constexpr std::uint32_t CHAR_STEP {font::CHAR_WIDTH + 1};  // one empty column between
    constexpr std::size_t COMMAND_BYTES {2};
}

hw::Ssd1306::Ssd1306(I2cBus& bus) : bus_ {bus}
{
    buffer_[0] = PIXELS_FOLLOW;
    for (const std::uint8_t command : INIT_COMMANDS)
    {
        send_command(command);
    }
}

bool hw::Ssd1306::send_command(std::uint8_t command)
{
    const std::array<std::uint8_t, COMMAND_BYTES> bytes {COMMAND_FOLLOWS, command};
    return bus_.write(ADDRESS, bytes.data(), bytes.size());
}

void hw::Ssd1306::clear()
{
    // Keep byte 0, PIXELS_FOLLOW
    std::fill(buffer_.begin() + 1, buffer_.end(), 0);
}

void hw::Ssd1306::draw_pixel(std::uint32_t x, std::uint32_t y, bool on)
{
    if (x >= WIDTH || y >= PAGES * PAGE_HEIGHT)
    {
        return;
    }
    // One byte holds the 8 rows of a page, bit 0 is the top row
    const std::uint32_t page {y / PAGE_HEIGHT};
    const std::uint8_t bit_mask {static_cast<std::uint8_t>(1U << (y % PAGE_HEIGHT))};
    const std::size_t index {1 + page * WIDTH + x};

    if (on)
    {
        buffer_[index] |= bit_mask;
    }
    else
    {
        buffer_[index] &= static_cast<std::uint8_t>(~bit_mask);
    }
}

void hw::Ssd1306::draw_rect(std::uint32_t x, std::uint32_t y, std::uint32_t width,
                            std::uint32_t height, bool filled)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    if (filled)
    {
        for (std::uint32_t row {0}; row < height; ++row)
        {
            for (std::uint32_t col {0}; col < width; ++col)
            {
                draw_pixel(x + col, y + row, true);
            }
        }
    }
    else
    {
        // Top and bottom, then left and right
        for (std::uint32_t col {0}; col < width; ++col)
        {
            draw_pixel(x + col, y, true);
            draw_pixel(x + col, y + height - 1, true);
        }
        for (std::uint32_t row {0}; row < height; ++row)
        {
            draw_pixel(x, y + row, true);
            draw_pixel(x + width - 1, y + row, true);
        }
    }
}

void hw::Ssd1306::draw_char(std::uint32_t column, std::uint32_t page, char character)
{
    if (character < font::FIRST_CHAR || character > font::LAST_CHAR)
    {
        character = '?';
    }
    const font::Glyph& glyph {font::GLYPHS[character - font::FIRST_CHAR]};

    for (std::size_t i {0}; i < font::CHAR_WIDTH && column + i < WIDTH; ++i)
    {
        buffer_[1 + page * WIDTH + column + i] = glyph[i];
    }
}

void hw::Ssd1306::draw_text(std::uint32_t column, std::uint32_t page, const std::string& text)
{
    if (page >= PAGES)
    {
        return;
    }
    for (const char character : text)
    {
        draw_char(column, page, character);
        column += CHAR_STEP;
    }
}

bool hw::Ssd1306::show()
{
    // The whole screen: columns 0 .. WIDTH - 1, pages 0 .. PAGES - 1
    const bool range_set {send_command(COLUMN_RANGE) && send_command(0) &&
                          send_command(WIDTH - 1) && send_command(PAGE_RANGE) &&
                          send_command(0) && send_command(PAGES - 1)};
    if (!range_set)
    {
        return false;
    }

    // Byte 0 of buffer_ is PIXELS_FOLLOW, so one write sends it all
    return bus_.write(ADDRESS, buffer_.data(), buffer_.size());
}
