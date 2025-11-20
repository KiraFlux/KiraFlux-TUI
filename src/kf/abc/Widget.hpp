#pragma once

#include <kf/core/Event.hpp>
#include <kf/core/Render.hpp>


namespace kf::ui {

/// @brief Виджет
struct Widget {

    /// @brief Отрисовать виджет
    /// @param render Способ отрисовки
    virtual void doRender(Render &render) const = 0;

    /// @brief Действие при событии клика
    /// @return true - Нужна перерисовка
    virtual bool onClick() { return false; }

    /// @brief Действие при изменении значения
    /// @param direction направление изменения
    /// @return true - Нужна перерисовка
    virtual bool onChange(int direction) { return false; }

    /// @brief Внешняя отрисовка виджета
    /// @param render Способ отрисовки
    /// @param selected виджет был выбран
    void render(Render &render, bool selected) const {
        if (selected) {
            render.write(0x81);
            doRender(render);
            render.write(0x80);
        } else {
            doRender(render);
        }
    }
};

}
