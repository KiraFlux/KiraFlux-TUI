#pragma once

#include <kf/core/Event.hpp>
#include <kf/core/TextStream.hpp>

namespace kf::tui {

struct Widget {
    virtual bool onEvent(Event event) = 0;

    virtual void doRender(TextStream &stream) const = 0;

    void render(TextStream &stream, bool selected) const {
        if (selected) {
            stream.write(0x81);
            doRender(stream);
            stream.write(0x80);
        } else {
            doRender(stream);
        }
    }
};

}// namespace kf::tui
