#include "text.h"

#include <cstdint>
#include <string>

#include "ssd1306.h"

gui::Text::Text(std::uint32_t x, std::uint32_t y, const std::string& text)
    : Widget {x, y}, text_ {text}
{
}

void gui::Text::set_text(const std::string& text)
{
    text_ = text;
}

gui::Label::Label(std::uint32_t x, std::uint32_t y, const std::string& text) : Text {x, y, text}
{
}

void gui::Label::draw(hw::Ssd1306& display) const
{
    display.draw_text(x(), y() / hw::Ssd1306::PAGE_HEIGHT, text());
}
