#pragma once

#include "kf/ui/core/Event.hpp"
#include "kf/ui/core/Render.hpp"


namespace kf::ui {

struct Page;

/// @brief Виджет
struct Widget {

public:

    /// @brief Конструктор виджета
    /// @param root Страница, которая будет содержать данный виджет
    /// @details Вызывает <code>Page::addWidget</code>
    explicit Widget(Page &root);

protected:

    /// @brief Отрисовать виджет
    /// @param render Способ отрисовки
    virtual void doRender(Render &render) const = 0;

public:

    /// @brief Действие при событии клика
    /// @returns true - Нужна перерисовка
    /// @returns false - Перерисовка не требуется
    virtual bool onClick() { return false; }

    /// @brief Действие при изменении значения
    /// @param direction Величина изменения
    /// @returns true - Нужна перерисовка
    /// @returns false - Перерисовка не требуется
    virtual bool onChange(int direction) { return false; }

    /// @brief Внешняя отрисовка виджета
    /// @param render Способ отрисовки
    /// @param focused Виджет в фокусе курсора
    void render(Render &render, bool focused) const {
        if (focused) {
            render.write(0x81);
            doRender(render);
            render.write(0x80);
        } else {
            doRender(render);
        }
    }
};

}
