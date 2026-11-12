#include "widgets.h"

#include <cstdint>
#include <string>

#include "ssd1306.h"

gui::Rectangle::Rectangle(std::uint32_t x, std::uint32_t y, std::uint32_t width,
                          std::uint32_t height, bool filled)
    : Shape<Rectangle> {x, y, width, height}, filled_ {filled}
{
}

void gui::Rectangle::draw_widget(hw::Ssd1306& display) const
{
    display.draw_rect(x(), y(), width(), height(), filled_);
}

gui::Label::Label(std::uint32_t x, std::uint32_t y, const std::string& text)
    : Text<Label> {x, y, text}
{
}

void gui::Label::draw_widget(hw::Ssd1306& display) const
{
    display.draw_text(x(), y() / hw::Ssd1306::PAGE_HEIGHT, text());
}
