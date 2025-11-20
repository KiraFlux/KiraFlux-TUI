#pragma once

#include <functional>

#include "kf/ui/abc/Widget.hpp"


namespace kf::ui {

/// @brief Чек-Бокс - Ввод булевого значения
struct CheckBox final : Widget {

    /// @brief Тип внешнего обработчика изменения
    using ChangeHandler = std::function<void(bool)>;

private:

    /// @brief Обработчик изменения
    ChangeHandler on_change;

    /// @brief Состояние
    bool state;

public:

    explicit CheckBox(
        Page &root,
        ChangeHandler change_handler,
        bool default_state = false
    ) :
        Widget{root},
        on_change{std::move(change_handler)},
        state{default_state} {}

    bool onClick() override {
        setState(not state);
        return true;
    }

    bool onChange(int direction) override {
        setState(direction > 0);
        return true;
    }

protected:

    void doRender(Render &render) const override {
        render.print(state ? "[ 1 ]==" : "--[ 0 ]");
    }

private:

    void setState(bool new_state) {
        state = new_state;

        if (on_change) {
            on_change(state);
        }
    }
};

}
