#pragma once

#include <kf/core/Event.hpp>
#include <kf/core/TextStream.hpp>

namespace kf::tui {

/// @brief Виджет
struct Widget {

    /// @brief Отрисовать виджет
    /// @param stream Поток вывода символов
    virtual void doRender(TextStream &stream) const = 0;

    /// @brief Действие при событии клика
    /// @return true - Нужна перерисовка
    virtual bool onClick() { return false; }

    /// @brief Действие при изменениии значения
    /// @param direction направление изменения
    /// @return true - Нужна перерисовка
    virtual bool onChange(int direction) { return false; }

    /// @brief Отреагировать на событие
    /// @param event входящее событие
    /// @return true - Нужна перерисовка
    bool onEvent(Event event) {
        switch (event) {
            case Event::Click:
                return onClick();

            case Event::ChangeIncrement:
                return onChange(+1);

            case Event::ChangeDecrement:
                return onChange(-1);

            default:
                return false;
        }
    }

    /// @brief Внешняя отрисовка виджета
    /// @param stream Поток вывода
    /// @param selected виджет был выбран
    void render(TextStream &stream, bool selected) const {
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
