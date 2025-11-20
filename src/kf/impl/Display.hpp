#pragma once

#include <kf/abc/Widget.hpp>


namespace kf::ui {

/// @brief Отображает значение
template<typename T> struct Display final : kf::ui::Widget {

    /// @brief Отображаемое значение
    const T &value;

    explicit Display(const T &val) :
        value{val} {}

    void doRender(BufferStream &stream) const override {
        stream.print(value);
    }
};

}// namespace kf::tui
