#pragma once

#include <kf/core/Event.hpp>
#include <kf/core/BufferStream.hpp>


namespace kf::ui {

/// @brief Виджет
struct Widget {

    /// @brief Отрисовать виджет
    /// @param stream Поток вывода символов
    virtual void doRender(BufferStream &stream) const = 0;

    /// @brief Действие при событии клика
    /// @return true - Нужна перерисовка
    virtual bool onClick() { return false; }

    /// @brief Действие при изменении значения
    /// @param direction направление изменения
    /// @return true - Нужна перерисовка
    virtual bool onChange(int direction) { return false; }

    /// @brief Внешняя отрисовка виджета
    /// @param stream Поток вывода
    /// @param selected виджет был выбран
    void render(BufferStream &stream, bool selected) const {
        if (selected) {
            stream.write(0x81);
            doRender(stream);
            stream.write(0x80);
        } else {
            doRender(stream);
        }
    }
};

}// namespace kf::tui
