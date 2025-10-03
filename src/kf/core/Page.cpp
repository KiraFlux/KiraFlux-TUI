#include <kf/core/Event.hpp>
#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>

void kf::tui::PageSetterButton::doRender(TextStream &stream) const {
    stream.write('>');
    stream.write(' ');
    stream.print(target.title);
}

bool kf::tui::PageSetterButton::onClick() {
    PageManager::instance().bind(target);
    return true;
}
