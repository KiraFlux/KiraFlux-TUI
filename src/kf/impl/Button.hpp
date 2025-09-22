#pragma once

#include <functional>

#include <kf/abc/Widget.hpp>

namespace kf {

struct Button final : Widget {

    using ClickHandler = std::function<void(Button &)>;

    const char *label;
    ClickHandler on_click;

    explicit Button(const char *label, ClickHandler on_click = nullptr) noexcept :
        label{label}, on_click{std::move(on_click)} {}

    bool onEvent(Event event) override {
        if (event == Event::Click and on_click) {
            on_click(*this);
        }
        return false;
    }

    void doRender(TextStream &stream) const override {
        stream.write('[');
        stream.print(label);
        stream.write(']');
    }
};

}// namespace kf
