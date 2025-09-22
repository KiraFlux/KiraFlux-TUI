#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>

void kf::PageSetterButton::doRender(TextStream &stream) const {
    stream.write('>');
    stream.write(' ');
    stream.print(target.title);
}

bool kf::PageSetterButton::onEvent(Event event) {
    if (event != Event::Click) { return false; }
    kf::PageManager::instance().bind(target);
    return true;
}
