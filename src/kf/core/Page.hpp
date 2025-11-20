#pragma once

#include <queue>
#include <utility>
#include <vector>

#include <kf/abc/Widget.hpp>
#include <kf/core/Render.hpp>
#include <kf/core/Event.hpp>


namespace kf::ui {

/// @brief Страница, содержит виджеты. Имеет заголовок.
struct Page;

/// @brief Специальный виджет для создания кнопки перехода на страницу
struct PageSetterButton final : Widget {

    /// @brief Страница перехода
    Page &target;

    explicit PageSetterButton(Page &target) :
        target{target} {}

    /// @brief Реализация Widget::onClick - устанавливает активную страницу
    bool onClick() override;

    void doRender(Render &render) const override;
};

struct Page {

    /// @brief Заголовок страницы.
    const char *title;

private:

    /// @brief Виджеты данной страницы
    std::vector<Widget *> widgets{};

    /// @brief Курсор (Активный виджет)
    int cursor{0};

    /// @brief Виджет перехода к данной странице
    PageSetterButton to_this{*this};

public:
    explicit Page(const char *title) noexcept:
        title{title} {}

    /// @brief Добавить виджет в данную страницу
    /// @param widget Добавляемый виджет
    void add(Widget &widget) { widgets.push_back(&widget); }

    /// @brief Связывание страниц
    /// @details Добавляет виджеты перехода к страницам
    /// @param other Связываемая страница
    void link(Page &other) {
        this->add(other.to_this);
        other.add(this->to_this);
    }

    /// @brief Отобразить страницу
    /// @param render Система отрисовки
    /// @param rows Кол-во строк
    void render(Render &render, int rows) {
        render.print(title);
        render.write('\n');

        rows -= 1;

        const auto start = (totalWidgets() > rows) ? std::min(cursor, totalWidgets() - rows) : 0;
        const auto end = std::min(start + rows, totalWidgets());

        for (int i = start; i < end; i += 1) {
            widgets[i]->render(render, i == cursor);
            render.write('\n');
        }
    }

    /// @brief Отреагировать на событие
    /// @param event Входящее событие
    /// @returns true - Требуется перерисовка; false - перерисовка не требуется
    bool onEvent(Event event) {
        switch (event) {
            case Event::None: {
                return false;
            }
            case Event::Update: {
                return true;
            }
            case Event::ElementNext: {
                moveCursor(+1);
                return true;
            }
            case Event::ElementPrevious: {
                moveCursor(-1);
                return true;
            }
            case Event::Click: {
                return widgets[cursor]->onClick();
            }
            case Event::ChangeIncrement: {
                return widgets[cursor]->onChange(+1);
            }
            case Event::ChangeDecrement: {
                return widgets[cursor]->onChange(-1);
            }
        }

        return false;
    }

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
