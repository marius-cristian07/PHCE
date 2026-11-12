#ifndef SCREEN_H
#define SCREEN_H

#include "ssd1306.h"
#include "widget.h"

namespace gui
{
    // Draws widgets on one display.
    class Screen
    {
    public:
        explicit Screen(hw::Ssd1306& display) : display_ {display} {}

        void clear() { display_.clear(); }

        // Any kind of Widget
        void draw(const Widget& widget) { widget.draw(display_); }

        bool show() { return display_.show(); }  // true if the screen got it all

    private:
        hw::Ssd1306& display_;
    };
}

#endif  // SCREEN_H
