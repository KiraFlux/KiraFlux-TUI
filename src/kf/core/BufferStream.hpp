#pragma once

#include <Print.h>
#include <kf/aliases.hpp>
#include <array>


namespace kf::tui {

struct BufferStream final : Print {

private:
    std::array<char, 128> buffer{};
    usize cursor{0};

public:

    slice<const char> prepareData() {
        buffer[cursor] = '\0';

        return {
            buffer.data(),
            cursor,
        };
    }

    void reset() {
        cursor = 0;
    }

    usize write(u8 c) override {
        if (cursor < buffer.size()) {
            buffer[cursor] = static_cast<char>(c);
            cursor += 1;
            return 1;
        }

        return 0;
    }
};

}// namespace kf::tui
