#pragma once

#include <functional>

#include "kf/abc/Widget.hpp"

namespace kf::tui {

struct CheckBox final : kf::tui::Widget {

    using Handler = std::function<void(bool)>;

private:
    Handler on_change;
    bool state;

public:
    explicit CheckBox(Handler change_handler, bool default_state = false) :
        on_change{std::move(change_handler)}, state{default_state} {}

    bool onEvent(Event event) override {
        if (event == Event::Click) {
            setState(not state);
            return true;
        }

        if (event == Event::ChangeDecrement) {
            setState(false);
            return true;
        }

        if (event == Event::ChangeIncrement) {
            setState(true);
            return true;
        }

        return false;
    }

    void doRender(TextStream &stream) const override {
        stream.print(getLabel());
    }

private:
    [[nodiscard]] inline const char *getLabel() const { return state ? "[1]==" : "--[0]"; }

    void setState(bool new_state) {
        state = new_state;
        if (on_change) { on_change(state); }
    }
};

}// namespace kf::tui
