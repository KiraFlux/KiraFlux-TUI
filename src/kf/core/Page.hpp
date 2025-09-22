#pragma once

#include <queue>
#include <utility>
#include <vector>

#include <kf/core/Event.hpp>
#include <kf/core/TextStream.hpp>
#include <kf/abc/Widget.hpp>

namespace kf {

struct Page;

struct PageSetterButton final : Widget {
    Page &target;

    explicit PageSetterButton(Page &target) :
        target{target} {}

    bool onEvent(Event event) override;

    void doRender(TextStream &stream) const override;
};

struct Page {

    const char *title;

private:
    std::vector<Widget *> widgets{};
    int cursor{0};
    PageSetterButton to_this{*this};

public:
    explicit Page(const char *title) noexcept :
        title{title} {}

    void add(Widget &widget) { widgets.push_back(&widget); }

    void link(Page &other) {
        this->add(other.to_this);
        other.add(this->to_this);
    }

    void render(TextStream &stream, int rows) {
        stream.print(title);
        stream.write('\n');

        rows -= 1;

        const int start = (totalWidgets() > rows) ? std::min(cursor, totalWidgets() - rows) : 0;
        const int end = std::min(start + rows, totalWidgets());

        for (int i = start; i < end; i++) {
            widgets[i]->render(stream, i == cursor);
            stream.write('\n');
        }
    }

    bool onEvent(Event event) {
        switch (event) {
            case Event::None:
                return false;

            case Event::Update:
                return true;

            case Event::ElementNext:
                cursorMove(+1);
                return true;

            case Event::ElementPrevious:
                cursorMove(-1);
                return true;

            case Event::Click:
            case Event::ChangeIncrement:
            case Event::ChangeDecrement:
                return widgets[cursor]->onEvent(event);
        }

        return false;
    }

private:
    void cursorMove(int delta) {
        cursor += delta;
        cursor = std::max(cursor, 0);
        cursor = std::min(cursor, cursorPositionMax());
    }

    inline int totalWidgets() const { return static_cast<int>(widgets.size()); }

    inline int cursorPositionMax() const { return totalWidgets() - 1; }
};

}// namespace kf
