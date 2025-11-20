#pragma once

#include <functional>

#include <kf/abc/Widget.hpp>


namespace kf::ui {

/// @brief Кнопка - Виджет, реагирующий на клик
struct Button final : Widget {

    /// @brief Обработчик клика
    using Handler = std::function<void()>;

private:
    /// @brief Метка кнопки
    const char *label;

    /// @brief Внешний обработчик клика
    Handler on_click;

public:
    explicit Button(const char *label, Handler on_click) noexcept:
        label{label}, on_click{std::move(on_click)} {}

    bool onClick() override {
        if (on_click) { on_click(); }
        return false;
    }

    void doRender(Render &render) const override {
        render.write('[');
        render.print(label);
        render.write(']');
    }
};

}
