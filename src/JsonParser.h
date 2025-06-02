#pragma once
#include "Tokenizer.h"
#include "JsonNode.h"
#include <string>
#include <sstream>

/**
 * @class JsonParser
 * @brief Парсва JSON текст във вътрешна структура от тип JsonNode.
 *
 * Използва Tokenizer, за да получи токени от JSON низ и създава съответното JSON дърво.
 */
class JsonParser
{
public:
    /**
     * @brief Конструктор, инициализира парсера с подаден JSON текст.
     * @param src Суров JSON като низ.
     */
    JsonParser(const std::string &src) : tokenizer(src), current(tokenizer.nextToken()) {}

    /**
     * @brief Започва парсването на JSON текста.
     * @return Указател към кореновия JsonNode.
     * @throws std::runtime_error при синтактични грешки.
     */
    JsonNode *parse();

private:
    Tokenizer tokenizer; ///< Използва се за разбиване на JSON текста на токени.
    Token current;       ///< Текущ токен, който се обработва.

    /** @brief Придвижва се до следващия токен. */
    void advance();

    /**
     * @brief Проверява дали текущият токен е очакваният.
     * @param expected Очакваният тип токен.
     * @throws std::runtime_error ако типът не съвпада.
     */
    void expect(TokenType expected);

    /**
     * @brief Парсва отделна JSON стойност (число, низ, обект и т.н.).
     * @return Указател към JsonNode, представящ стойността.
     */
    JsonNode *parseValue();

    /**
     * @brief Парсва JSON масив.
     * @return JsonNode от тип ARRAY.
     */
    JsonNode *parseArray();

    /**
     * @brief Парсва JSON обект.
     * @return JsonNode от тип OBJECT.
     */
    JsonNode *parseObject();
};

/**
 * @brief Помощна функция за директно парсване на JSON низ.
 * @param src Суров JSON текст.
 * @return Указател към JsonNode с резултата.
 * @throws std::runtime_error при невалиден JSON.
 */
JsonNode *parseJson(const std::string &src);
