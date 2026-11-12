#ifndef TEXT_H
#define TEXT_H

#include <cstdint>
#include <string>

#include "ssd1306.h"
#include "widget.h"

namespace gui
{
    // A Widget with a text. Still abstract: it does not know how to show it.
    class Text : public Widget
    {
    public:
        const std::string& text() const { return text_; }
        void set_text(const std::string& text);

    protected:
        Text(std::uint32_t x, std::uint32_t y, const std::string& text);

    private:
        std::string text_;
    };

    // One line of Text. It goes on the page that y is in: a page is 8 rows.
    class Label : public Text
    {
    public:
        Label(std::uint32_t x, std::uint32_t y, const std::string& text);

        void draw(hw::Ssd1306& display) const override;
    };
}

#endif  // TEXT_H
