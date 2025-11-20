#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>


void kf::ui::PageSetterButton::doRender(BufferStream &stream) const {
    stream.write('>');
    stream.write(' ');
    stream.print(target.title);
}

bool kf::ui::PageSetterButton::onClick() {
    PageManager::instance().bind(target);
    return true;
}
