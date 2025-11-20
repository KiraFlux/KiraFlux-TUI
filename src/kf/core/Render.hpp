#pragma once

#include <array>
#include <Print.h>
#include <kf/aliases.hpp>


namespace kf::ui {

/// @brief Бекенд для отрисовки
struct Render final : Print {

private:
    std::array<char, 128> buffer{};
    usize cursor{0};

public:

    /// @brief Подготовить буфер
    slice<const char> prepareData() {
        buffer[cursor] = '\0';

        return {
            buffer.data(),
            cursor,
        };
    }

    /// @brief Сбросить буфер отрисовки
    void reset() {
        cursor = 0;
    }

    /// @brief Реализация Print::write
    usize write(u8 c) override {
        if (cursor >= buffer.size()) {
            return 0;
        }

        buffer[cursor] = static_cast<char>(c);
        cursor += 1;
        return 1;
    }
};

}
