#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <cstdint>

#include "shape.h"
#include "ssd1306.h"
#include "text.h"
#include "widget.h"

namespace gui
{
    // A bar that fills from the left, with its value as text above it.
    // It is a Widget. It has a Label and two Rectangles.
    class ProgressBar : public Widget
    {
    public:
        static constexpr std::uint32_t MAX_PERCENT {100};

        // x and y are the top left of the label. The bar is one page below it.
        // width and height are the size of the bar's border.
        ProgressBar(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);

        std::uint32_t percent() const { return percent_; }
        // More than MAX_PERCENT counts as MAX_PERCENT
        void set_percent(std::uint32_t percent);

        void draw(hw::Ssd1306& display) const override;

    private:
        std::uint32_t percent_ {0};
        Label label_;       // "42 %"
        Rectangle border_;  // not filled
        Rectangle fill_;    // Filled, inside the border. Its width follows percent_.
    };
}

#endif  // PROGRESS_BAR_H
