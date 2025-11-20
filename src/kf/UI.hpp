#pragma once

#include <type_traits>
#include <utility>
#include <functional>
#include <queue>
#include <array>
#include <vector>

#include <Print.h>

#include <kf/aliases.hpp>
#include <kf/tools/meta/Singleton.hpp>


namespace kf {

/// @brief Пользовательский интерфейс
struct UI final : tools::Singleton<UI> {
    friend struct Singleton<UI>;

    /// @brief Событие
    enum class Event {

        // Управление

        /// @brief Ничего (Заглушка)
        None,

        /// @brief Принудительное обновление
        Update,

        // События страницы

        /// @brief Выбор следующего элемента
        ElementNext,

        /// @brief Смена элемента
        ElementPrevious,

        // События виджета

        /// @brief Клик
        Click,

        /// @brief Изменить элемент +
        ChangeIncrement,

        /// @brief Изменить элемент -
        ChangeDecrement,
    };

    /// @brief Бекенд для отрисовки
    struct Render final : Print {

    private:
        std::array<char, 128> buffer{};
        usize cursor{0};

    public:
        u16 rows{8}, cols{21};

        /// @brief Подготовить буфер
        slice<const char> data() {
            buffer[cursor] = '\0';

            return {
                buffer.data(),
                cursor,
            };
        }

        /// @brief Сбросить буфер отрисовки
        void reset() {
            cursor = 0;
        }

        /// @brief Реализация Print::write
        usize write(u8 c) override {
            if (cursor >= buffer.size()) {
                return 0;
            }

            buffer[cursor] = static_cast<char>(c);
            cursor += 1;
            return 1;
        }
    };

    struct Page;

    /// @brief Виджет
    struct Widget {

    public:

        /// @brief Конструктор виджета
        /// @param root Страница, которая будет содержать данный виджет
        /// @details Вызывает <code>Page::addWidget</code>
        explicit Widget(Page &root);

    protected:

        /// @brief Отрисовать виджет
        /// @param render Способ отрисовки
        virtual void doRender(Render &render) const = 0;

    public:

        /// @brief Действие при событии клика
        /// @returns true - Нужна перерисовка
        /// @returns false - Перерисовка не требуется
        virtual bool onClick() { return false; }

        /// @brief Действие при изменении значения
        /// @param direction Величина изменения
        /// @returns true - Нужна перерисовка
        /// @returns false - Перерисовка не требуется
        virtual bool onChange(int direction) { return false; }

        /// @brief Внешняя отрисовка виджета
        /// @param render Способ отрисовки
        /// @param focused Виджет в фокусе курсора
        void render(Render &render, bool focused) const {
            if (focused) {
                render.write(0x81);
                doRender(render);
                render.write(0x80);
            } else {
                doRender(render);
            }
        }
    };

    /// @brief Страница, содержит виджеты и обладает заголовком.
    struct Page {

    private:

        /// @brief Специальный виджет для создания кнопки перехода на страницу
        /// @note Не используется в пользовательском коде. Для связывания страниц используйте <code>kf::ui::Page::link</code>
        struct PageSetter final : Widget {

        private:

            /// @brief Страница перехода
            Page &target;

        public:

            explicit PageSetter(
                Page &target
            ) :
                Widget{target},
                target{target} {}

            /// @brief Устанавливает активную страницу
            bool onClick() override;

        protected:

            void doRender(Render &render) const override;
        };

        /// @brief Виджеты данной страницы
        std::vector<Widget *> widgets{};

        /// @brief Заголовок страницы.
        const char *title;

        /// @brief Курсор (Индекс активного виджета)
        int cursor{0};

        /// @brief Виджет перехода к данной странице
        PageSetter to_this{*this};

    public:

        explicit Page(const char *title) noexcept:
            title{title} {}

        /// @brief Добавить виджет в данную страницу
        /// @param widget Добавляемый виджет
        void addWidget(Widget &widget) { widgets.push_back(&widget); }

        /// @brief Связывание страниц
        /// @details Добавляет виджеты перехода к страницам
        /// @param other Связываемая страница
        void link(Page &other) {
            this->addWidget(other.to_this);
            other.addWidget(this->to_this);
        }

        /// @brief Отобразить страницу
        /// @param render Система отрисовки
        /// @param rows Кол-во строк
        void render(Render &render);

        /// @brief Отреагировать на событие
        /// @param event Входящее событие
        /// @returns true - Требуется перерисовка
        /// @returns false - перерисовка не требуется
        bool onEvent(Event event);

    private:
        void moveCursor(int delta) {
            cursor += delta;
            cursor = std::max(cursor, 0);
            cursor = std::min(cursor, cursorPositionMax());
        }

        [[nodiscard]] inline int totalWidgets() const { return static_cast<int>(widgets.size()); }

        [[nodiscard]] inline int cursorPositionMax() const { return totalWidgets() - 1; }
    };

private:

    /// @brief Входящие события
    std::queue<Event> events{};

    /// @brief Активная страница
    Page *active_page{nullptr};

    /// @brief Предыдущая страница
    Page *previous_page{nullptr};

    /// @brief Система отображения
    Render render_system{};

public:

    /// @brief Установить активную страницу
    void bind(Page &page) {
        previous_page = active_page;
        active_page = &page;
    }

    /// @brief Вернуться на предыдущую страницу
    void back() {
        std::swap(previous_page, active_page);
    }

    /// @brief Рендеринг активной страницы
    slice<const char> render() {
        if (nullptr == active_page) {
            constexpr char s[] = "null page";
            return {s, sizeof(s)};;
        }

        render_system.reset();
        active_page->render(render_system);

        return render_system.data();
    }

    /// @brief Добавить событие в очередь
    void addEvent(Event event) {
        events.push(event);
    }

    /// @brief Прокрутка событий
    /// @returns true - Необходима перерисовка
    /// @returns false - Перерисовка не требуется
    bool pollEvents() {
        // mostly time active page is not null, so consider to null-check at last.
        if (events.empty() or nullptr == active_page) {
            return false;
        }

        const bool render_required = active_page->onEvent(events.front());
        events.pop();
        return render_required;
    }

public:
    // built-in widgets

    /// @brief Кнопка - Виджет, реагирующий на клик
    struct Button final : Widget {

        /// @brief Обработчик клика
        using ClickHandler = std::function<void()>;

    private:

        /// @brief Метка кнопки
        const char *label;

        /// @brief Внешний обработчик клика
        ClickHandler on_click;

    public:

        explicit Button(
            Page &root,
            const char *label,
            ClickHandler on_click
        ) :
            Widget{root},
            label{label},
            on_click{std::move(on_click)} {}

        bool onClick() override {
            if (on_click) {
                on_click();
            }

            return false;
        }

    protected:

        void doRender(Render &render) const override {
            render.write('[');
            render.print(label);
            render.write(']');
        }
    };

    /// @brief Чек-Бокс - Ввод булевого значения
    struct CheckBox final : Widget {

        /// @brief Тип внешнего обработчика изменения
        using ChangeHandler = std::function<void(bool)>;

    private:

        /// @brief Обработчик изменения
        ChangeHandler on_change;

        /// @brief Состояние
        bool state;

    public:

        explicit CheckBox(
            Page &root,
            ChangeHandler change_handler,
            bool default_state = false
        ) :
            Widget{root},
            on_change{std::move(change_handler)},
            state{default_state} {}

        bool onClick() override {
            setState(not state);
            return true;
        }

        bool onChange(int direction) override {
            setState(direction > 0);
            return true;
        }

    protected:

        void doRender(Render &render) const override {
            render.print(state ? "[ 1 ]==" : "--[ 0 ]");
        }

    private:

        void setState(bool new_state) {
            state = new_state;

            if (on_change) {
                on_change(state);
            }
        }
    };

    /// @brief ComboBox - выбор из списка значений
    /// @tparam T Тип выбираемых значений
    /// @tparam N Кол-во выбираемых значений
    template<typename T, usize N> struct ComboBox final : Widget {
        static_assert(N >= 1, "N >= 1");

    public:

        /// @brief Элемент выбора
        struct Item {

            /// @brief Наименование элемента
            const char *key;

            /// @brief Значение
            T value;
        };

        /// @brief Контейнер элементов
        using Container = std::array<Item, N>;

    private:

        /// @brief Элементы выбора
        const Container items;

        /// @brief Изменяемое значение
        T &value;

        /// @brief Выбранное значение
        int cursor{0};

    public:

        explicit ComboBox(
            Page &root,
            Container items,
            T &val
        ) :
            Widget{root},
            items{std::move(items)},
            value{val} {}

        bool onChange(int direction) override {
            moveCursor(direction);

            value = items[cursor].value;

            return true;
        }

    protected:

        void doRender(Render &render) const override {
            render.write('<');
            render.print(items[cursor].key);
            render.write('>');
        }

    private:

        /// @brief Сместить курсор
        /// @param delta смещение
        void moveCursor(int delta) {
            cursor += delta;
            cursor += N;
            cursor %= N;
        }
    };

    /// @brief Отображает значение
    template<typename T> struct Display final : Widget {

    private:

        /// @brief Отображаемое значение
        const T &value;

    public:

        explicit Display(
            Page &root,
            const T &val
        ) :
            Widget{root},
            value{val} {}

    protected:

        void doRender(Render &render) const override {
            render.print(value);
        }
    };

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

    /// @brief Спин-бокс - Виджет для изменения арифметического значения в указанном режиме
    template<typename T> struct SpinBox final : Widget {
        static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

        /// @brief Тип скалярной величины виджета
        using Scalar = T;

        /// @brief Режим изменения значения
        enum class Mode : unsigned char {
            /// @brief Арифметическое изменение
            Arithmetic,

            /// @brief Арифметическое изменение (Только положительные)
            ArithmeticPositiveOnly,

            /// @brief Геометрическое изменение
            Geometric
        };

    private:

        /// @brief
        bool is_step_setting_mode{false};

        /// @brief Режим изменения значения
        const Mode mode;

        /// @brief Изменяемое значение
        T &value;

        /// @brief Шаг изменения значения
        T step;

    public:
        explicit SpinBox(
            Page &root,
            T &value,
            T step = static_cast<T>(1),
            Mode mode = Mode::Arithmetic
        ) :
            Widget{root},
            mode{mode},
            value{value},
            step{step} {}

        bool onClick() override {
            is_step_setting_mode = not is_step_setting_mode;
            return true;
        }

        bool onChange(int direction) override {
            if (is_step_setting_mode) {
                changeStep(direction);
            } else {
                changeValue(direction);
            }
            return true;
        }

    protected:

        void doRender(Render &render) const override {
            if (is_step_setting_mode) {
                render.write('s');
            }

            render.write('<');

            if (std::is_floating_point<T>::value) {
                render.print(static_cast<float>(value), 4);
            } else {
                render.print(value);
            }

            render.write('>');
        }

    private:
        void changeValue(int direction) {
            if (mode == Mode::Geometric) {
                calcGeometric(value, direction, step);
                return;
            }

            value += direction * step;

            if (mode == Mode::ArithmeticPositiveOnly and value < 0) {
                value = 0;
            }
        }

        void changeStep(int direction) {
            constexpr T step_step{static_cast<T>(10.0f)};
            calcGeometric(step, direction, step_step);
        }

        static void calcGeometric(T &value, int direction, T s) {
            if (direction > 0) {
                value *= s;
            } else {
                value /= s;
            }
        }
    };

};

}
