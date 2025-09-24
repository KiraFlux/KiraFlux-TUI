#include "Event.hpp"
#include "PageManager.hpp"
#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>

void kf::tui::PageSetterButton::doRender(TextStream &stream) const {
    stream.write('>');
    stream.write(' ');
    stream.print(target.title);
}

bool kf::tui::PageSetterButton::onEvent(tui::Event event) {
    if (event != tui::Event::Click) { return false; }
    PageManager::instance().bind(target);
    return true;
}
