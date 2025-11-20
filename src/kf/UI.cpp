#include "kf/UI.hpp"


// kf::UI::Widget

kf::UI::Widget::Widget(UI::Page &root) {
    root.addWidget(*this);
}


// kf::UI::Page

void kf::UI::Page::render(kf::UI::Render &render) {
    render.print(title);
    render.write('\n');

    const auto rows = render.rows - 1;

    const auto start = (totalWidgets() > rows) ? std::min(cursor, totalWidgets() - rows) : 0;
    const auto end = std::min(start + rows, totalWidgets());

    for (int i = start; i < end; i += 1) {
        widgets[i]->render(render, i == cursor);
        render.write('\n');
    }
}

bool kf::UI::Page::onEvent(kf::UI::Event event) {
    switch (event) {
        case Event::None: {
            return false;
        }
        case Event::Update: {
            return true;
        }
        case Event::ElementNext: {
            moveCursor(+1);
            return true;
        }
        case Event::ElementPrevious: {
            moveCursor(-1);
            return true;
        }
        case Event::Click: {
            return widgets[cursor]->onClick();
        }
        case Event::ChangeIncrement: {
            return widgets[cursor]->onChange(+1);
        }
        case Event::ChangeDecrement: {
            return widgets[cursor]->onChange(-1);
        }
    }
    return false;
}


// kf::UI::Page::PageSetter

void kf::UI::Page::PageSetter::doRender(Render &render) const {
    render.write('>');
    render.write(' ');
    render.print(target.title);
}

bool kf::UI::Page::PageSetter::onClick() {
    UI::instance().bind(target);
    return true;
}
