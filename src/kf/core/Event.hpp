#pragma once

namespace kf::tui {

enum class Event {

    // Управление

    /// @brief Ничего (Заглушка)
    None,

    /// @brief Принудительное обновление
    Update,

    // События страницы

    /// @brief Выбор следующего элемента
    ElementNext,

    /// @brief Смена элемента
    ElementPrevious,

    // События виджета

    /// @brief Клик
    Click,

    /// @brief Изменить элемент +
    ChangeIncrement,

    /// @brief Изменить элемент -
    ChangeDecrement,
};

}
