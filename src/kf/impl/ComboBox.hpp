#pragma once

#include <array>

#include "kf/abc/Widget.hpp"

namespace kf {

template<typename T, size_t N> struct ComboBox final : Widget {
    static_assert(N >= 1, "N >= 1");

public:
    struct Item {
        const char *key;
        T value;
    };

    using Container = std::array<Item, N>;

private:
    const Container items;
    T &value;
    int cursor{0};

public:
    explicit ComboBox(Container items, T &val) :
        items{std::move(items)}, value{val} {}

    bool onEvent(kf::Event event) override {
        if (event == kf::Event::ChangeDecrement) {
            moveCursor(-1);
            value = items[cursor].value;
            return true;
        }
        if (event == kf::Event::ChangeIncrement) {
            moveCursor(+1);
            value = items[cursor].value;
            return true;
        }
        return false;
    }

    void doRender(kf::TextStream &stream) const override {
        stream.write('<');
        stream.print(items[cursor].key);
        stream.write('>');
    }

private:
    void moveCursor(int d) {
        cursor += d;
        cursor += N;
        cursor %= N;
    }
};

}// namespace kf