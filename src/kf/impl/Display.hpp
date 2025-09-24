#pragma once

#include <kf/abc/Widget.hpp>

namespace kf::tui {

template<typename T> struct Display final : kf::tui::Widget {
    const T &value;

    explicit Display(const T &val) :
        value{val} {}

    bool onEvent(Event event) override { return false; }

    void doRender(TextStream &stream) const override { stream.print(value); }
};

}// namespace kf::tui
