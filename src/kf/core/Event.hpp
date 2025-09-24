#pragma once

namespace kf::tui {

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

}
