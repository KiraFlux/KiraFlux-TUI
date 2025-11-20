#pragma once

#include <queue>

#include "kf/ui/core/Render.hpp"
#include "kf/ui/core/Event.hpp"
#include "kf/ui/core/Page.hpp"

// todo унаследовать от Singleton
namespace kf::ui {

struct PageManager final {

private:

    std::queue<Event> events{};
    Render render_system{};
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

        render_system.reset();
        active_page->render(render_system, rows);

        return render_system.prepareData();
    }

    void addEvent(Event event) {
        events.push(event);
    }

    bool pollEvents() {
        if (events.empty() or nullptr == active_page) { return false; }

        const bool render_required = active_page->onEvent(events.front());
        events.pop();
        return render_required;
    }
};

}
