#ifndef SCREEN_H
#define SCREEN_H

#include "ssd1306.h"

namespace gui
{
    // Draws widgets on one display.
    class Screen
    {
    public:
        explicit Screen(hw::Ssd1306& display) : display_ {display} {}

        void clear() { display_.clear(); }

        // Any number of objects of any class that has draw()
        template <typename... Widgets>
        void draw_all(const Widgets&... widgets)
        {
            (widgets.draw(display_), ...);
        }

        bool show() { return display_.show(); }  // true if the screen got it all

    private:
        hw::Ssd1306& display_;
    };
}

#endif  // SCREEN_H
