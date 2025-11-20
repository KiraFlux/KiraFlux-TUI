#pragma once

#include <kf/abc/Widget.hpp>

namespace kf::ui {

/// @brief Добавляет метку к виджету
/// @tparam W Тип реализации виджета, к которому будет добавлена метка
template<typename W> struct Labeled final : kf::ui::Widget {
    static_assert(std::is_base_of<Widget, W>::value, "W must be a Widget Subclass");

    /// @brief Реализация виджета, к которому была добавлена метка
    using Content = W;

    /// @brief Метка
    const char *label;

    /// @brief Виджет
    W content;

    explicit Labeled(const char *label, W content) noexcept :
        label{label}, content{std::move(content)} {}

    bool onClick() override { return content.onClick(); }

    bool onChange(int direction) override { return content.onChange(direction); }

    void doRender(BufferStream &stream) const override {
        stream.print(label);
        stream.write(' ');
        stream.write(':');
        content.doRender(stream);
    }
};

}// namespace kf::tui
