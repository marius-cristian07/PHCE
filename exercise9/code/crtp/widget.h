#ifndef WIDGET_H
#define WIDGET_H

#include <cstdint>
#include <string>

#include "ssd1306.h"

namespace gui
{
    // The base of everything that can be drawn. Nothing here is virtual.
    // Derived is the class that derives from it: class Rectangle : public Shape<Rectangle>.
    template <typename Derived>
    class Widget
    {
    public:
        std::uint32_t x() const { return x_; }
        std::uint32_t y() const { return y_; }

        // The same for every widget. The Derived class writes draw_widget().
        void draw(hw::Ssd1306& display) const
        {
            // TODO 1: this object is a Derived. Call its draw_widget().
        }

    protected:
        // Protected: only a derived class can make a Widget
        Widget(std::uint32_t x, std::uint32_t y) : x_ {x}, y_ {y} {}

    private:
        std::uint32_t x_;  // pixels from the left
        std::uint32_t y_;  // pixels from the top
    };

    // A Widget with a size. It hands Derived on to Widget.
    template <typename Derived>
    class Shape : public Widget<Derived>
    {
    public:
        std::uint32_t width() const { return width_; }
        std::uint32_t height() const { return height_; }

        void set_size(std::uint32_t width, std::uint32_t height)
        {
            width_ = width;
            height_ = height;
        }

    protected:
        Shape(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
            : Widget<Derived> {x, y}, width_ {width}, height_ {height}
        {
        }

    private:
        std::uint32_t width_;   // pixels
        std::uint32_t height_;  // pixels
    };

    // A Widget with a text. It hands Derived on to Widget.
    template <typename Derived>
    class Text : public Widget<Derived>
    {
    public:
        const std::string& text() const { return text_; }
        void set_text(const std::string& text) { text_ = text; }

    protected:
        Text(std::uint32_t x, std::uint32_t y, const std::string& text)
            : Widget<Derived> {x, y}, text_ {text}
        {
        }

    private:
        std::string text_;
    };
}

#endif  // WIDGET_H
