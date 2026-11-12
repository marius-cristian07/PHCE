#ifndef SHAPE_H
#define SHAPE_H

#include <cstdint>

#include "ssd1306.h"
#include "widget.h"

namespace gui
{
    // A Widget with a size. Still abstract: it does not know what it looks like.
    class Shape : public Widget
    {
    public:
        std::uint32_t width() const { return width_; }
        std::uint32_t height() const { return height_; }
        void set_size(std::uint32_t width, std::uint32_t height);

    protected:
        Shape(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);

    private:
        std::uint32_t width_;   // pixels
        std::uint32_t height_;  // pixels
    };

    // A Shape with four straight sides. Filled, or only the border.
    class Rectangle : public Shape
    {
    public:
        Rectangle(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height,
                  bool filled = false);

        void draw(hw::Ssd1306& display) const override;

    private:
        bool filled_;
    };
}

#endif  // SHAPE_H
