#pragma once

#include <Print.h>
#include <array>
#include <kf/aliases.hpp>

namespace kf::tui {

/// @brief Бекенд для отрисовки
struct BufferStream final : Print {

private:
    std::array<char, 128> buffer{};
    usize cursor{0};

public:

    /// @brief Подготовить буффер
    slice<const char> prepareData() {
        buffer[cursor] = '\0';

        return {
            buffer.data(),
            cursor,
        };
    }

    /// @brief Сбросить буфер отрисовку
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

}// namespace kf::tui
