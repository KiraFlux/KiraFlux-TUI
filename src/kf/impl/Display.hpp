#pragma once

namespace kf {

template<typename T> struct Display final : Widget {
    const T &value;

    explicit Display(const T &val) :
        value{val} {}

    bool onEvent(Event event) override { return false; }

    void doRender(TextStream &stream) const override { stream.print(value); }
};

}// namespace kf
