#pragma once

#include <queue>

#include "TextStream.hpp"
#include <kf/core/Event.hpp>
#include <kf/core/Page.hpp>
#include <kf/core/TextStream.hpp>

namespace kf::tui {

struct PageManager final {

private:
    std::queue<Event> events{};
    TextStream stream{};
    Page *active_page{nullptr};
    Page *previous_page{nullptr};

    explicit PageManager() = default;

public:
    PageManager(const PageManager &) = delete;

    static PageManager &instance() {
        static PageManager instance;
        return instance;
    }

    int rows{8};

    void bind(Page &page) {
        previous_page = active_page;
        active_page = &page;
    }

    void back() {
        std::swap(previous_page, active_page);
    }

    TextStream::Slice render() {
        static constexpr char null_page_content[] = "null page";
        static constexpr TextStream::Slice null_page_slice{null_page_content, sizeof(null_page_content)};

        if (active_page == nullptr) { return null_page_slice; }

        stream.reset();
        active_page->render(stream, rows);

        return stream.prepareData();
    }

    void addEvent(Event event) {
        events.push(event);
    }

    bool pollEvents() {
        if (active_page == nullptr) { return false; }

        if (events.empty()) { return false; }

        const bool render_required = active_page->onEvent(events.front());
        events.pop();
        return render_required;
    }
};

}// namespace kf::tui
