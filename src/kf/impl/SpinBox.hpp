#pragma once

#include <kf/abc/Widget.hpp>
#include <type_traits>

namespace kf::tui {

/// @brief Спин-бокс - Виджет для изменения арифметического значенмия в указанном режиме
template<typename T> struct SpinBox final : kf::tui::Widget {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

    /// @brief Тип скалярной величины виджета
    using Scalar = T;

    /// @brief Режим изменения значения
    enum class Mode : unsigned char {
        /// @brief Арифметическое изменение
        Arithmetic,

        /// @brief Арифметическое изменение (Только положительные)
        ArithmeticPositiveOnly,

        /// @brief Геометрическое изменение
        Geometric
    };

private:
    /// @brief
    bool is_step_setting_mode{false};
    /// @brief Режим изменения значения
    const Mode mode;

    /// @brief Изменяемое значение
    T &value;

    /// @brief Шаг изменения значения
    T step;

public:
    explicit SpinBox(
        T &value,
        T step = static_cast<T>(1.0f),
        Mode mode = Mode::Arithmetic) :
        mode{mode}, value{value}, step{step} {}

    bool onClick() override {
        is_step_setting_mode ^= 1;
        return true;
    }

    bool onChange(int direction) override {
        if (is_step_setting_mode) {
            changeStep(direction);
        } else {
            changeValue(direction);
        }
        return true;
    }

    void doRender(TextStream &stream) const override {
        if (is_step_setting_mode) {
            stream.write('s');
        }

        stream.write('<');

        if (std::is_floating_point<T>::value) {
            stream.print(static_cast<float>(value), 4);
        } else {
            stream.print(value);
        }

        stream.write('>');
    }

private:
    void changeValue(int direction) {
        if (mode == Mode::Geometric) {
            calcGeometric(value, direction, step);
            return;
        }

        value += direction * step;

        if (mode == Mode::ArithmeticPositiveOnly and value < 0) {
            value = 0;
        }
    }

    void changeStep(int direction) {
        constexpr T step_step{static_cast<T>(10.0f)};
        calcGeometric(step, direction, step_step);
    }

    static void calcGeometric(T &value, int direction, T s) {
        if (direction > 0) {
            value *= s;
        } else {
            value /= s;
        }
    }
};

}// namespace kf::tui
