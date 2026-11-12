#include "shape.h"

#include <cstdint>

#include "ssd1306.h"

gui::Shape::Shape(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
    : Widget {x, y}, width_ {width}, height_ {height}
{
}

void gui::Shape::set_size(std::uint32_t width, std::uint32_t height)
{
    width_ = width;
    height_ = height;
}

gui::Rectangle::Rectangle(std::uint32_t x, std::uint32_t y, std::uint32_t width,
                          std::uint32_t height, bool filled)
    : Shape {x, y, width, height}, filled_ {filled}
{
}

void gui::Rectangle::draw(hw::Ssd1306& display) const
{
    // TODO 1: draw this rectangle on display.
}
