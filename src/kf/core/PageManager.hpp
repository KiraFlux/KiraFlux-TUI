#pragma once

#include <queue>

#include <kf/core/Event.hpp>
#include <kf/core/Page.hpp>
#include <kf/core/BufferStream.hpp>


namespace kf::tui {

struct PageManager final {

private:
    std::queue<Event> events{};
    BufferStream stream{};
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

    slice<const char> render() {
        static const slice<const char> null_page_slice{"null page", sizeof("null page")};

        if (nullptr == active_page) {
            return null_page_slice;
        }

        stream.reset();
        active_page->render(stream, rows);

        return stream.prepareData();
    }

    void addEvent(Event event) {
        events.push(event);
    }

    bool pollEvents() {
        if (nullptr == active_page or events.empty()) { return false; }

        const bool render_required = active_page->onEvent(events.front());
        events.pop();
        return render_required;
    }
};

}// namespace kf::tui
