#pragma once

#include <array>
#include <kf/aliases.hpp>

#include <kf/abc/Widget.hpp>


namespace kf::ui {

/// @brief ComboBox - выбор из списка значений
/// @tparam T Тип выбираемых значений
/// @tparam N Кол-во выбираемых значений
template<typename T, usize N> struct ComboBox final : Widget {
    static_assert(N >= 1, "N >= 1");

public:

    /// @brief Элемент выбора
    struct Item {

        /// @brief Наименование элемента
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

protected:

    void doRender(Render &render) const override {
        render.write('<');
        render.print(items[cursor].key);
        render.write('>');
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

}
