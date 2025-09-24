#pragma once

#include <kf/abc/Widget.hpp>
#include <type_traits>

namespace kf::tui {

template<typename T> struct SpinBox final : kf::tui::Widget {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

    using Scalar = T;

    enum class Mode {
        Arithmetic,
        ArithmeticPositiveOnly,
        Geometric
    };

    T &value;
    const T &step;
    const Mode mode;

    explicit SpinBox(T &value, const T &step, Mode mode = Mode::Arithmetic) noexcept :
        value{value}, step{step}, mode{mode} {}

    bool onEvent(Event event) override {
        if (event == Event::ChangeIncrement) {
            if (mode == Mode::Geometric) {
                value *= step;
            } else {
                value += step;
            }
            return true;
        }

        if (event == Event::ChangeDecrement) {
            if (mode == Mode::Geometric) {
                value /= step;
            } else {
                value -= step;
                if (mode == Mode::ArithmeticPositiveOnly && value < 0) {
                    value = 0;
                }
            }
            return true;
        }

        return false;
    }

    void doRender(TextStream &stream) const override {
        stream.write('<');

        if (std::is_floating_point<T>::value) {
            stream.print(static_cast<float>(value), 4);
        } else {
            stream.print(value);
        }
        stream.write('>');
    }
};

}// namespace kf::tui
