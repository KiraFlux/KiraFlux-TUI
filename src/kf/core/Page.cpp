#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>


void kf::ui::Page::render(kf::ui::Render &render, int rows) {
    render.print(title);
    render.write('\n');

    rows -= 1;

    const auto start = (totalWidgets() > rows) ? std::min(cursor, totalWidgets() - rows) : 0;
    const auto end = std::min(start + rows, totalWidgets());

    for (int i = start; i < end; i += 1) {
        widgets[i]->render(render, i == cursor);
        render.write('\n');
    }
}

bool kf::ui::Page::onEvent(kf::ui::Event event) {
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

void kf::ui::Page::PageSetter::doRender(Render &render) const {
    render.write('>');
    render.write(' ');
    render.print(target.title);
}

bool kf::ui::Page::PageSetter::onClick() {
    PageManager::instance().bind(target);
    return true;
}
