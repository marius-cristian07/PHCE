#ifndef WIDGETS_H
#define WIDGETS_H

#include <cstdint>
#include <string>

#include "ssd1306.h"
#include "widget.h"

namespace gui
{
    // A Shape with four straight sides. Filled, or only the border.
    class Rectangle : public Shape<Rectangle>
    {
    public:
        Rectangle(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height,
                  bool filled = false);

        void draw_widget(hw::Ssd1306& display) const;

    private:
        bool filled_;
    };

    // One line of Text. It goes on the page that y is in: a page is 8 rows.
    class Label : public Text<Label>
    {
    public:
        Label(std::uint32_t x, std::uint32_t y, const std::string& text);

        void draw_widget(hw::Ssd1306& display) const;
    };
}

#endif  // WIDGETS_H
