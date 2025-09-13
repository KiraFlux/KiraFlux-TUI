# KiraFlux-TUI

*Минималистичный текстовый пользовательский интерфейс для встроенных систем на C++*

---

#### **TextStream**

Буферизированный поток для вывода текста, наследуемый от Arduino `Print`.

**Методы:**

```cpp
Slice prepareData();
```
Подготавливает данные для вывода, возвращая структуру с указателем на буфер и его длиной.

```cpp
void reset();
```
Очищает буфер, сбрасывая позицию курсора.

```cpp
size_t write(uint8_t c) override;
```
Записывает символ в буфер (наследование от Print).

---

#### **Widget**

Базовый абстрактный класс для всех элементов интерфейса.

**Методы:**

```cpp
virtual bool onEvent(Event event) = 0;
```
Обрабатывает событие элемента. Возвращает `true`, если требуется перерисовка.

```cpp
virtual void doRender(TextStream &stream) const = 0;
```
Рендерит содержимое элемента в поток вывода.

```cpp
void render(TextStream &stream, bool selected) const;
```
Рендерит элемент с учетом состояния выбора (инверсия цвета для выбранного элемента).

---

#### **Button**

Кнопка с обработчиком клика.

**Конструктор:**
```cpp
explicit Button(const char *label, ClickHandler on_click = nullptr) noexcept;
```
Создает кнопку с указанной меткой и обработчиком клика.

**Методы:**
```cpp
bool onEvent(Event event) override;
```
Обрабатывает события кнопки (реагирует на `Event::Click`).

```cpp
void doRender(TextStream &stream) const override;
```
Рендерит кнопку в формате `[Метка]`.

---

#### **Display**

Виджет для отображения значений любого типа.

**Конструктор:**
```cpp
explicit Display(const T &value) noexcept;
```
Создает виджет для отображения значения.

**Методы:**
```cpp
bool onEvent(Event event) override;
```
Не обрабатывает события (всегда возвращает `false`).

```cpp
void doRender(TextStream &stream) const override;
```
Выводит значение в поток.

---

#### **SpinBox**

Виджет для изменения числовых значений.

**Конструктор:**
```cpp
explicit SpinBox(T &value, const T &step, Mode mode = Mode::Arithmetic) noexcept;
```
Создает SpinBox для управления значением с указанным шагом и режимом.

**Режимы:**
- `Arithmetic`: Арифметическое изменение (+/-)
- `ArithmeticPositiveOnly`: Арифметическое изменение только положительных значений
- `Geometric`: Геометрическое изменение (*//)

**Методы:**
```cpp
bool onEvent(Event event) override;
```
Обрабатывает события изменения значения (`ChangeIncrement`/`ChangeDecrement`).

```cpp
void doRender(TextStream &stream) const override;
```
Рендерит значение в формате `<Значение>`.

---

#### **Labeled**

Контейнер для добавления метки к любому виджету.

**Конструктор:**
```cpp
explicit Labeled(const char *label, W content) noexcept;
```
Создает виджет с меткой и содержимым.

**Методы:**
```cpp
bool onEvent(Event event) override;
```
Передает события содержимому виджета.

```cpp
void doRender(TextStream &stream) const override;
```
Рендерит метку и содержимое в формате `Метка: Содержимое`.

---

#### **Page**

Страница интерфейса, содержащая набор виджетов.

**Конструктор:**
```cpp
explicit Page(const char *title) noexcept;
```
Создает страницу с указанным заголовком.

**Методы:**
```cpp
void add(Widget &widget);
```
Добавляет виджет на страницу.

```cpp
void link(Page &other);
```
Создает двустороннюю связь между страницами.

```cpp
void render(TextStream &stream, int rows);
```
Рендерит страницу с учетом количества доступных строк.

```cpp
bool onEvent(Event event);
```
Обрабатывает события навигации и передает события активному виджету.

---

#### **PageManager**

Менеджер для управления страницами и событиями.

**Методы:**
```cpp
static PageManager &instance();
```
Возвращает экземпляр менеджера (синглтон).

```cpp
void bind(Page &page);
```
Устанавливает активную страницу.

```cpp
void back();
```
Возвращает к предыдущей странице.

```cpp
TextStream::Slice render();
```
Рендерит активную страницу.

```cpp
void addEvent(Event event);
```
Добавляет событие в очередь.

```cpp
bool pollEvents();
```
Обрабатывает события из очереди.

---

#### **PageSetterButton**

Специальная кнопка для перехода между страницами.

**Конструктор:**
```cpp
explicit PageSetterButton(Page &target);
```
Создает кнопку для перехода на указанную страницу.

**Методы:**
```cpp
bool onEvent(Event event) override;
```
Обрабатывает клик для перехода на целевую страницу.

```cpp
void doRender(TextStream &stream) const override;
```
Рендерит кнопку в формате `> НазваниеСтраницы`.

---

### Типы событий

```cpp
enum class Event {
    None,               // Ничего
    Update,             // Принудительное обновление
    Click,              // Клик
    ElementNext,        // Выбор следующего элемента
    ElementPrevious,    // Выбор предыдущего элемента
    ChangeIncrement,    // Увеличение значения
    ChangeDecrement     // Уменьшение значения
};
```

---

### Примеры использования

**1. Простой интерфейс с кнопкой:**
```cpp
kftui::Page main_page("Main");
kftui::Button btn("Action", [](kftui::Button&) {
    // Обработчик клика
});

void setup() {
    main_page.add(btn);
    kftui::PageManager::instance().bind(main_page);
}

void loop() {
    auto& manager = kftui::PageManager::instance();
    if (manager.pollEvents()) {
        // Обновить дисплей
        auto output = manager.render();
        // Вывести output.data на дисплей
    }
}
```

**2. Интерфейс с несколькими страницами:**
```cpp
kftui::Page main("Main");
kftui::Page settings("Settings");

kftui::PageSetterButton to_settings(settings);
kftui::PageSetterButton to_main(main);

void setup() {
    main.add(to_settings);
    settings.add(to_main);
    main.link(settings); // Двусторонняя связь
    
    kftui::PageManager::instance().bind(main);
}
```

**3. Элементы управления с метками:**
```cpp
int value = 0;
kftui::SpinBox<int> spinner(value, 1);
kftui::Labeled labeled_spinner("Value", spinner);

kftui::Page page("Controls");
page.add(labeled_spinner);
```

---

### Особенности работы

1. **Архитектура**:
    - Основана на паттерне Observer для обработки событий
    - Использует синглтон PageManager для управления состоянием

2. **Производительность**:
    - Минимальное использование памяти
    - Буферизация вывода для уменьшения операций с дисплеем

3. **Навигация**:
    - Поддержка иерархической навигации между страницами
    - История навигации (кнопка "Назад")

4. **Расширяемость**:
    - Легкое добавление новых виджетов через наследование
    - Поддержка пользовательских обработчиков событий

**Рекомендации**:
- Для экономии памяти используйте статические экземпляры виджетов
- Обрабатывайте события в основном цикле приложения
- Используйте `Labeled` для улучшения UX

```cpp
// Экономное использование памяти
static kftui::Button btn("Test");
static kftui::Page page("Main");
```

Лицензия: MIT ([LICENSE](./LICENSE))