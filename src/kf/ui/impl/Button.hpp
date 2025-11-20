#pragma once

#include <functional>

#include "kf/ui/abc/Widget.hpp"


namespace kf::ui {

/// @brief Кнопка - Виджет, реагирующий на клик
struct Button final : Widget {

    /// @brief Обработчик клика
    using ClickHandler = std::function<void()>;

private:

    /// @brief Метка кнопки
    const char *label;

    /// @brief Внешний обработчик клика
    ClickHandler on_click;

public:

    explicit Button(
        Page &root,
        const char *label,
        ClickHandler on_click
    ) :
        Widget{root},
        label{label},
        on_click{std::move(on_click)} {}

    bool onClick() override {
        if (on_click) {
            on_click();
        }

        return false;
    }

protected:

    void doRender(Render &render) const override {
        render.write('[');
        render.print(label);
        render.write(']');
    }
};

}
