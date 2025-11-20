#include "kf/ui/core/Page.hpp"
#include "kf/ui/abc/Widget.hpp"


kf::ui::Widget::Widget(kf::ui::Page &root) {
    root.addWidget(*this);
}
