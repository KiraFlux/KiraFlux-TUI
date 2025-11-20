#pragma once

#include <queue>
#include <utility>
#include <vector>

#include "kf/ui/abc/Widget.hpp"
#include "kf/ui/core/Render.hpp"
#include "kf/ui/core/Event.hpp"


namespace kf::ui {

/// @brief Страница, содержит виджеты и имеет заголовок.
struct Page {

private:

    /// @brief Специальный виджет для создания кнопки перехода на страницу
    /// @note Не используется в пользовательском коде. Для связывания страниц используйте <code>kf::ui::Page::link</code>
    struct PageSetter final : Widget {

    private:

        /// @brief Страница перехода
        Page &target;

    public:

        explicit PageSetter(
            Page &target
        ) :
            Widget{target},
            target{target} {}

        /// @brief Устанавливает активную страницу
        bool onClick() override;

    protected:

        void doRender(Render &render) const override;
    };

    /// @brief Виджеты данной страницы
    std::vector<Widget *> widgets{};

    /// @brief Заголовок страницы.
    const char *title;

    /// @brief Курсор (Индекс активного виджета)
    int cursor{0};

    /// @brief Виджет перехода к данной странице
    PageSetter to_this{*this};

public:
    explicit Page(const char *title) noexcept:
        title{title} {}

    /// @brief Добавить виджет в данную страницу
    /// @param widget Добавляемый виджет
    void addWidget(Widget &widget) { widgets.push_back(&widget); }

    /// @brief Связывание страниц
    /// @details Добавляет виджеты перехода к страницам
    /// @param other Связываемая страница
    void link(Page &other) {
        this->addWidget(other.to_this);
        other.addWidget(this->to_this);
    }

    /// @brief Отобразить страницу
    /// @param render Система отрисовки
    /// @param rows Кол-во строк
    void render(Render &render, int rows);

    /// @brief Отреагировать на событие
    /// @param event Входящее событие
    /// @returns true - Требуется перерисовка
    /// @returns false - перерисовка не требуется
    bool onEvent(Event event);

private:
    void moveCursor(int delta) {
        cursor += delta;
        cursor = std::max(cursor, 0);
        cursor = std::min(cursor, cursorPositionMax());
    }

    [[nodiscard]] inline int totalWidgets() const { return static_cast<int>(widgets.size()); }

    [[nodiscard]] inline int cursorPositionMax() const { return totalWidgets() - 1; }
};

}
