#pragma once

#include <array>

#include "kf/abc/Widget.hpp"

namespace kf::tui {

/// @brief Комбо-бокс - выбор из списка значений
/// @tparam T Тип выбираемых значений
/// @tparam N Кол-во выбираемых значений
template<typename T, size_t N> struct ComboBox final : kf::tui::Widget {
    static_assert(N >= 1, "N >= 1");

public:
    /// @brief Элемент выбора
    struct Item {
        /// @brief Наименование элемениа
        const char *key;

        /// @brief Значение
        T value;
    };

    /// @brief Контейнер элементов
    using Container = std::array<Item, N>;

private:
    /// @brief Элементы выбора
    const Container items;

    /// @brief Изменяемое значение
    T &value;

    /// @brief Выбранное значение
    int cursor{0};

public:
    explicit ComboBox(Container items, T &val) :
        items{std::move(items)}, value{val} {}

    bool onChange(int direction) override {
        moveCursor(+direction);
        value = items[cursor].value;
        return true;
    }

    void doRender(TextStream &stream) const override {
        stream.write('<');
        stream.print(items[cursor].key);
        stream.write('>');
    }

private:
    /// @brief Сместить курсор
    /// @param d смещение
    void moveCursor(int d) {
        cursor += d;
        cursor += N;
        cursor %= N;
    }
};

}// namespace kf::tui
