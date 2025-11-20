#pragma once

#include <type_traits>

#include "kf/ui/abc/Widget.hpp"


namespace kf::ui {

/// @brief Добавляет метку к виджету
/// @tparam W Тип реализации виджета, к которому будет добавлена метка
template<typename W> struct Labeled final : Widget {
    static_assert(std::is_base_of<Widget, W>::value, "W must be a Widget Subclass");

    /// @brief Реализация виджета, к которому была добавлена метка
    using Content = W;

private:

    /// @brief Метка
    const char *label;

    /// @brief Виджет
    W content;

public:

    explicit Labeled(
        Page &root,
        const char *label,
        W content
    ) :
        Widget{root},
        label{label},
        content{std::move(content)} {}

    bool onClick() override { return content.onClick(); }

    bool onChange(int direction) override { return content.onChange(direction); }

protected:

    void doRender(Render &render) const override {
        render.print(label);
        render.write(' ');
        render.write(':');
        content.doRender(render);
    }
};

}
