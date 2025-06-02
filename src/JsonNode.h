#pragma once
#include <iostream>

/**
 * @enum JsonType
 * @brief Вид на JSON стойността.
 */
enum class JsonType
{
    OBJECT,  ///< JSON обект (ключ-стойност).
    ARRAY,   ///< JSON масив.
    STRING,  ///< Низ (стринг).
    NUMBER,  ///< Числова стойност.
    BOOLEAN, ///< Булева стойност (true/false).
    NIL      ///< Null стойност.
};

/**
 * @struct JsonNode
 * @brief Основен възел в JSON дървото, представящ стойност от произволен тип.
 */
struct JsonNode;

/**
 * @struct KeyValue
 * @brief Представлява един член (ключ-стойност) от JSON обект.
 */
struct KeyValue
{
    char *key;       ///< Ключът в обекта.
    JsonNode *value; ///< Стойността, асоциирана с ключа.
    KeyValue *next;  ///< Указател към следващия елемент в обекта.
};

/**
 * @struct ArrayElement
 * @brief Представлява един елемент от JSON масив.
 */
struct ArrayElement
{
    JsonNode *value;    ///< Стойността на елемента.
    ArrayElement *next; ///< Следващият елемент в масива.
};

/**
 * @struct JsonNode
 * @brief Представлява възел от JSON дърво. Може да бъде обект, масив, низ, число, булева стойност или null.
 */
struct JsonNode
{
    JsonType type; ///< Тип на стойността.

    /// Стойностите, асоциирани с различните JSON типове.
    union
    {
        KeyValue *object;    ///< Използва се ако е JsonType::OBJECT.
        ArrayElement *array; ///< Използва се ако е JsonType::ARRAY.
        char *str;           ///< Използва се ако е JsonType::STRING.
        double number;       ///< Използва се ако е JsonType::NUMBER.
        bool boolean;        ///< Използва се ако е JsonType::BOOLEAN.
    };

    /**
     * @brief Конструктор с тип.
     * @param t Типът на стойността (OBJECT, ARRAY и т.н.)
     */
    JsonNode(JsonType t);

    /** @brief Деструктор – освобождава паметта за вложени елементи. */
    ~JsonNode();

    /**
     * @brief Добавя ключ-стойност двойка към обекта.
     * @param key Ключът.
     * @param value Стойността, асоциирана с ключа.
     */
    void addObjectMember(const char *key, JsonNode *value);

    /**
     * @brief Добавя стойност към JSON масив.
     * @param value Стойността за добавяне.
     */
    void addArrayElement(JsonNode *value);

    /**
     * @brief Връща стойността, асоциирана с даден ключ.
     * @param key Ключът за търсене.
     * @return Указател към стойността, ако съществува, иначе nullptr.
     */
    JsonNode *getObjectMember(const char *key);

    /**
     * @brief Извежда съдържанието на JSON възела с форматиране.
     * @param indent Брой интервали за отстъп (по подразбиране 0).
     */
    void print(int indent = 0);
};

/** @brief Създава JsonNode от тип STRING. */
JsonNode *makeString(const char *s);

/** @brief Създава JsonNode от тип NUMBER. */
JsonNode *makeNumber(double val);

/** @brief Създава JsonNode от тип BOOLEAN. */
JsonNode *makeBoolean(bool b);

/** @brief Създава JsonNode от тип NIL. */
JsonNode *makeNull();

/** @brief Създава JsonNode от тип OBJECT. */
JsonNode *makeObject();

/** @brief Създава JsonNode от тип ARRAY. */
JsonNode *makeArray();

/** @brief Връща указател към елемент в JSON масив по индекс. */
JsonNode *getArrayElementAt(JsonNode *arrayNode, int index);
