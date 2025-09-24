#pragma once

#include <kf/abc/Widget.hpp>

namespace kf::tui {

template<typename W> struct Labeled final : kf::tui::Widget {
    static_assert(std::is_base_of<Widget, W>::value, "W must be a Widget Subclass");

    using Content = W;

    const char *label;
    W content;

    explicit Labeled(const char *label, W content) noexcept :
        label{label}, content{std::move(content)} {}

    bool onEvent(Event event) override {
        return content.onEvent(event);
    }

    void doRender(TextStream &stream) const override {
        stream.print(label);
        stream.write(0x82);
        stream.write(':');
        content.doRender(stream);
    }
};

}// namespace kf::tui
