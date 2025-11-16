#pragma once

#include <kf/abc/Widget.hpp>


namespace kf::tui {

/// @brief Отображает значение
template<typename T> struct Display final : kf::tui::Widget {

    /// @brief Отображаемое значение
    const T &value;

    explicit Display(const T &val) :
        value{val} {}

    void doRender(BufferStream &stream) const override {
        stream.print(value);
    }
};

}// namespace kf::tui
