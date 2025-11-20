#pragma once

#include "kf/ui/abc/Widget.hpp"


namespace kf::ui {

/// @brief Отображает значение
template<typename T> struct Display final : Widget {

private:

    /// @brief Отображаемое значение
    const T &value;

public:

    explicit Display(
        Page &root,
        const T &val
    ) :
        Widget{root},
        value{val} {}

protected:

    void doRender(Render &render) const override {
        render.print(value);
    }
};

}
