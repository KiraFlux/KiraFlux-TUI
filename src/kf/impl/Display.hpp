#pragma once

#include <kf/abc/Widget.hpp>


namespace kf::ui {

/// @brief Отображает значение
template<typename T> struct Display final : Widget {

    /// @brief Отображаемое значение
    const T &value;

    explicit Display(const T &val) :
        value{val} {}

    void doRender(Render &render) const override {
        render.print(value);
    }
};

}// namespace kf::tui
