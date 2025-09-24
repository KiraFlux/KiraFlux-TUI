#pragma once

#include <array>

#include "kf/abc/Widget.hpp"

namespace kf::tui {

template<typename T, size_t N> struct ComboBox final : kf::tui::Widget {
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

    bool onEvent(Event event) override {
        if (event == Event::ChangeDecrement) {
            moveCursor(-1);
            value = items[cursor].value;
            return true;
        }
        if (event == Event::ChangeIncrement) {
            moveCursor(+1);
            value = items[cursor].value;
            return true;
        }
        return false;
    }

    void doRender(TextStream &stream) const override {
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

}// namespace kf::tui
