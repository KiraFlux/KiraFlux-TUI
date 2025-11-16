#pragma once

#include <functional>

#include "kf/abc/Widget.hpp"

namespace kf::tui {

/// @brief Чек-Бокс - Ввод булевого значения
struct CheckBox final : kf::tui::Widget {

    /// @brief Тип внешнего обработчика изменения
    using Handler = std::function<void(bool)>;

private:
    /// @brief Обработчик изменения
    Handler on_change;

    /// @brief Состояние
    bool state;

public:
    explicit CheckBox(Handler change_handler, bool default_state = false) :
        on_change{std::move(change_handler)}, state{default_state} {}

    bool onClick() override {
        setState(not state);
        return true;
    }

    bool onChange(int direction) override {
        setState(direction > 0);
        return true;
    }

    void doRender(BufferStream &stream) const override {
        stream.print(getLabel());
    }

private:
    [[nodiscard]] inline const char *getLabel() const { return state ? "[ 1 ]==" : "--[ 0 ]"; }

    void setState(bool new_state) {
        state = new_state;
        if (on_change) { on_change(state); }
    }
};

}// namespace kf::tui
