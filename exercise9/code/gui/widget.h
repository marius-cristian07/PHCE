#ifndef WIDGET_H
#define WIDGET_H

#include <cstdint>

#include "ssd1306.h"

namespace gui
{
    // The base of everything that can be drawn. It is abstract: draw() has no body here.
    class Widget
    {
    public:
        virtual ~Widget() = default;

        // Not virtual: every widget keeps its place the same way
        std::uint32_t x() const { return x_; }
        std::uint32_t y() const { return y_; }

        // Virtual: each kind of widget draws itself in its own way
        virtual void draw(hw::Ssd1306& display) const = 0;

    protected:
        // Protected: only a derived class can make a Widget
        Widget(std::uint32_t x, std::uint32_t y) : x_ {x}, y_ {y} {}

    private:
        std::uint32_t x_;  // pixels from the left
        std::uint32_t y_;  // pixels from the top
    };
}

#endif  // WIDGET_H
