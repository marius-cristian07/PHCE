#include "progress_bar.h"

#include <algorithm>
#include <cstdint>
#include <string>

#include "ssd1306.h"

namespace
{
    constexpr std::uint32_t GAP {2};  // pixels between the border and the fill

    // Turns 42 into "42 %".
    std::string percent_text(std::uint32_t percent)
    {
        return std::to_string(percent) + " %";
    }
}

gui::ProgressBar::ProgressBar(std::uint32_t x, std::uint32_t y, std::uint32_t width,
                              std::uint32_t height)
    : Widget {x, y},
      label_ {x, y, percent_text(percent_)},
      border_ {x, y + hw::Ssd1306::PAGE_HEIGHT, width, height},
      fill_ {x + GAP, y + hw::Ssd1306::PAGE_HEIGHT + GAP, 0, height - 2 * GAP, true}
{
}

void gui::ProgressBar::set_percent(std::uint32_t percent)
{
    percent_ = std::min(percent, MAX_PERCENT);

    // The widest the fill can be: the border's width without a gap on each side
    const std::uint32_t full_width {border_.width() - 2 * GAP};

    // The fill gets its new width, the label its new text
    fill_.set_size(full_width * percent_ / MAX_PERCENT, fill_.height());
    label_.set_text(percent_text(percent_));
}

void gui::ProgressBar::draw(hw::Ssd1306& display) const
{
    // TODO 2: draw the three parts.
}
