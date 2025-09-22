#pragma once

namespace kf {

enum class Event {

    /// Ничего
    None,

    /// Принудительное обновление
    Update,

    /// Клик
    Click,

    /// Выбор следующего элемента
    ElementNext,

    /// Смена элемента
    ElementPrevious,

    /// Изменить элемент +
    ChangeIncrement,

    /// Изменить элемент -
    ChangeDecrement,
};

}// namespace kf
