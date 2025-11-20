#include <kf/core/Page.hpp>
#include <kf/core/PageManager.hpp>


void kf::ui::PageSetterButton::doRender(Render &render) const {
    render.write('>');
    render.write(' ');
    render.print(target.title);
}

bool kf::ui::PageSetterButton::onClick() {
    PageManager::instance().bind(target);
    return true;
}
