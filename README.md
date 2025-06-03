# Json Parser

## Цел на проекта

Проектът има за цел да реализира CLI инструмент за работа с JSON файлове, поддържащ зареждане, преглед, валидиране, редактиране и запис на съдържание. Основната цел е да се приложат принципите на **обектно-ориентираното програмиране**.

---

## Документация

Пълната документация на проекта, генерирана чрез Doxygen, е налична [тук](https://stoyanov-kaloyan.github.io/json-parser/)

## Компилация

Проектът използва `CMake`. Използвайте следните команди, за да го билднете:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Архитектура

Основното предизвикателство при работа с JSON е неговата гъвкавост откъм структурата на данните. JSON може да съдържа обекти, масиви, низове, числа, булеви стойности и `null`, което изисква **динамично управление на паметта** и подходяща архитектура способна да работи с различни типове данни.
За разлика от масивите в език, като C++, където типът на елементите е фиксиран, JSON позволява **смесени типове** в масиви и обекти, което създава основно предизвикателство, при адаптацията на структурата от данни към нещо, което може да се използва в C++.

JSON е **рекурсивна, вложена** структура, която позволява обекти и масиви да съдържат други обекти и масиви. Поради тази **йерархична природа**, най-естественият начин за вътрешно представяне е чрез **дърво**, където всеки връх (`JsonNode`) може да бъде обект, масив, низ, число, булева стойност или `null`.

Дървото позволява:

- **Рекурсивна обхождане** (за търсене, печат, сериализация)
- **Лесно добавяне и изтриване** на елементи на произволна дълбочина
- **Интуитивно съответствие с JSON структурата**

Масивите са имплементирани, като списъци (`Linked Lists`) от върхове (`JsonNodes`), което позволява динамично добавяне и изтриване на елементи от различни типове в един и същ масив.

```c++
struct ArrayElement
{
    JsonNode *value;    ///< Стойността на елемента.
    ArrayElement *next; ///< Следващият елемент в масива.
};
```

Стойността на един връх е представена чрез **съюз** (`union`), който позволява различни типове данни да бъдат съхранявани в един и същ връх, като се използва само необходимото количество памет. Това е ключова оптимизация от страна на паметта.

```c++
struct JsonNode
{
    JsonType type;

    union
    {
        KeyValue *object;
        ArrayElement *array;
        char *str;
        double number;
        bool boolean;
    };

    //...
}
```

Класът `Tokenizer` служи за **лексикален анализ** – той разбива входен JSON низ на последователност от **токени**, които `JsonParser` използва за изграждане на синтактичното дърво (`JsonNode`).
Той разпонзава следните токени:

```c++
enum class TokenType
{
    LEFT_BRACE,    ///< {
    RIGHT_BRACE,   ///< }
    LEFT_BRACKET,  ///< [
    RIGHT_BRACKET, ///< ]
    COMMA,         ///< ,
    COLON,         ///< :
    STRING,        ///< Низ (напр. "име")
    NUMBER,        ///< Число (напр. 3.14, -2)
    TRUE,          ///< true
    FALSE,         ///< false
    NIL,           ///< null
    END            ///< Край на входа
};
```

Класовете `JsonParser` и `Tokenizer` работят съвместно, за да осигурят коректно синтактично разпознаване и валидиране на JSON. `Tokenizer` първо разбива входния низ на последователни токени – като низове, числа, обекти, масиви и ключови думи (`true`, `false`, `null`). След това `JsonParser` използва тези токени, за да изгради дървовидна структура (`JsonNode`), съответстваща на логическата структура на JSON файла. Всеки токен се проверява спрямо очакваните типове чрез метода `expect`(), което позволява ранно откриване на грешки и гарантира, че входният текст отговаря на синтаксиса на JSON.

---

## Използване

Следват редица примери, които демонстрират работата на програмата. За да стартирате програмата, компилирайте я и изпълнете:

Създайте файл `example.json`, със следното съдържание, в папката с изпълнимия файл:

```json
{
  "user": {
    "name": "Kaloyan",
    "age": 25,
    "is_active": true,
    "friends": ["Ivan", "Petar", "Maria"],
    "grades": [
      {
        "subject": "Math",
        "score": 5.5
      },
      {
        "subject": "Physics",
        "score": 6.0
      }
    ],
    "address": {
      "city": "Sofia",
      "country": "Bulgaria"
    },
    "settings": {
      "theme": "dark",
      "notifications": true
    }
  }
}
```

Стартирайте програмата от директорията с изпълнимия файл:

```bash
./JsonParser.exe
```

Отворете файла с командата `open`:

```bash
open example.json
```

Ако файлът съдържа валиден JSON, ще видите следното съобщение:

```bash
File loaded.
JSON is valid.
```

При зареждане на файл автоматично се извършва валидиране, чрез командата `validate`. Тя може да се изпълни и ръчно.

Командата `help` ще ви покаже всички налични команди:

```
help
// Изход
Commands:
  open <file>             - Load JSON file into memory
  validate                - Validate and parse the loaded JSON
  print                   - Pretty print the parsed JSON
  move "from" "to"        - Move a key and its contents from one path to another
  save ["path"]             - Save current JSON or a sub-object to the last opened file
  saveas "file" ["path"]   - Save current JSON or sub-object to a new file
  search <key>            - Search for all occurrences of a specific key
  search-regex <regex>    - Search for keys matching a regular expression
  set "path" "value"     - Replace the value at a given path (if it exists)
  create "path" "value"  - Create a new key with the given value
  delete "path"           - Delete the key at the given path
  close                   - Close the current file
  help                    - Show this help message
  exit                    - Quit the program
```

Можем да проверим съдържанието на JSON дървото в паметта с командата `print`:

```bash
print
```

Нека да променим оценката по математика на 6.0, като използваме командата `set`:

```bash
> set "user/grades/0/score" "6.0"
# Value at path 'user/grades/0/score' updated successfully.
```

Можем да създадем и нова оценка в масива с командата `create`:

```bash
> create "user/grades/2" {"subject":"Chemistry","score":"3.25"}
# Appended value to array at index 2.
```

Ще изтрием и една от оценките с командата `delete`:

```bash
> delete "user/grades/1"
# Deleted array element at index: 1
```

Нека демонстрираме и създаването на масив с командата `create`:

````bash
> create "user/favorite_colors" ["red", "green", "blue", [255, 0, 0], {"C":12, "M":34, "Y":56, "K":78}]
```

И нека запазим промените в същия файл с командата `save`:

```bash
> save
# Saved to file: example.json
````

Нека демонстрираме командата move, като преместим `settings` обекта извън `user` обекта:

```bash
> move "user/settings" "settings"
# Moved 'user/settings' to 'settings'
```

Така преместихме целия `settings` обект на по-високо ниво в JSON дървото.

```json
{
  "settings": {
    "theme": "dark",
    "notifications": true
  },
  "user": {
    //...
```

Можем да запазим обекта settings в нов файл с командата `saveas`:

```bash
> saveas "settings.json" "settings"
# Saved to file: settings.json
# You have written only a subpath of the JSON file.
# To prevent data loss or inconsistency, please reload the file manually using 'open settings.json'.
# Alternatively, use 'save' to overwrite the full in-memory JSON content.
> close
> open settings.json
> print
# ще изведе
# {
#   "settings": {
#     "notifications": true,
#     "theme": "dark"
#   }
# }
```

Можем да търсим по ключове с командата `search`:

```bash
> search notifications
# Found 1 matching entries:
# true
```

Или по регулярни изрази с командата `search-regex`:

```bash
> search-regex noti.*
# Found 1 matching entries:
# true
```

Можем да затворим текущия файл с командата `close`:

```bash
> close
```

И да излезем от програмата с командата `exit`:

```bash
> exit
```

## Kоманди

### Основни команди

#### 🔹 `open <file>`

Зарежда съдържание от JSON файл в паметта.
**Пример:** `open data.json`

---

#### 🔹 `validate`

Валидира синтаксиса на заредения JSON.
**Пример:** `validate`

---

#### 🔹 `print`

Извежда JSON съдържанието с форматиране.
**Пример:** `print`

---

#### 🔹 `save`

Записва JSON дървото обратно в последно отворения файл.
**Пример:** `save`

---

#### 🔹 `save "path"`

Записва само част от JSON дървото по зададен път.
**Пример:** `save "person/info"`

---

#### 🔹 `saveas "file"`

Записва пълното JSON съдържание в нов файл.
**Пример:** `saveas "new.json"`

---

#### 🔹 `saveas "file" "path"`

Записва само поддърво от JSON в нов файл.
**Пример:** `saveas "part.json" "person"`

---

### Манипулиране на данни

#### 🔹 `set "path" "value"`

Задава стойност на съществуващ елемент.
**Пример:** `set "user/name" "Ivan"`

---

#### 🔹 `create "path" "value"`

Създава нова стойност, дори по не-съществуващи пътища.
**Пример:** `create "settings/theme" "dark"`

---

#### 🔹 `delete "path"`

Изтрива стойност по зададен път.
**Пример:** `delete "user/age"`

---

#### 🔹 `move "from" "to"`

Премества стойност от един път към друг.
**Пример:** `move "user/name" "user/fullname"`

---

### Търсене

#### 🔹 `search <key>`

Намира всички срещания на ключ в JSON структурата.
**Пример:** `search age`

---

#### 🔹 `search-regex <pattern>`

Търси по регулярен израз в ключовете.
**Пример:** `search-regex ^user_.*`

---

### Други

#### 🔹 `close`

Затваря текущия файл и изчиства заредените данни.

#### 🔹 `help`

Показва всички команди с описания.

#### 🔹 `exit`

Излиза от програмата.

---

## Бъдещи подобрения

### Повече юнит тестове

В момента проектът включва основни тестове за команди като `set`, `create`, `delete`, `move` и `saveas`. За повишаване на надеждността и покритието е препоръчително да се добавят:

- Тестове за всички възможни грешки (напр. невалидни пътища, невалиден JSON, празен вход)
- Гранични случаи (`edge cases`) при масиви (индекси извън обхват, вложени масиви и т.н.)
- Тестове с дълбоко вложени JSON структури

### Автоматизирано тестване

Включване на автоматична тестова система (напр. с `CMake`, `CTest` или GitHub Actions), която да проверява правилното поведение при всяка промяна.

```

```
