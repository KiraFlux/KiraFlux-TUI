#pragma once

#include <Print.h>
#include <array>

namespace kf::tui {

struct TextStream final : Print {
    static constexpr size_t buffer_size = 128;

private:
    std::array<char, buffer_size> buffer{};
    size_t cursor{0};

public:
    struct Slice {
        const char *data;
        const size_t len;
    };

    Slice prepareData() {
        buffer[cursor] = '\0';

        return {
            buffer.data(),
            cursor,
        };
    }

    void reset() { cursor = 0; }

    size_t write(uint8_t c) override {
        if (cursor < buffer_size) {
            buffer[cursor] = static_cast<char>(c);
            cursor += 1;
            return 1;
        }
        return 0;
    }
};

}// namespace kf::tui
