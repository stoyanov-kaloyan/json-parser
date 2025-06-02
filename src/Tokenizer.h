#pragma once
#include <string>
#include <variant>

/**
 * @enum TokenType
 * @brief Типове токени, използвани при разбора на JSON текст.
 */
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

/**
 * @struct Token
 * @brief Представлява един токен, извлечен от JSON текста.
 *
 * Всеки токен има тип (TokenType) и стойност (ако е приложимо).
 */
struct Token
{
    TokenType type; ///< Типът на токена.

    /// Стойност, асоциирана с токена (низ или число, ако е приложимо).
    std::variant<std::monostate, std::string, double> value;
};

/**
 * @class Tokenizer
 * @brief Разбива JSON низ на поредица от токени за нуждите на синтактичен анализ.
 */
class Tokenizer
{
public:
    /**
     * @brief Конструктор.
     * @param src Суров JSON низ.
     */
    Tokenizer(const std::string &src) : json(src), pos(0) {}

    /**
     * @brief Връща следващия токен от входа.
     * @return Следващият токен.
     * @throws std::runtime_error при неочаквани символи или грешки.
     */
    Token nextToken();

private:
    std::string json; ///< Входният JSON низ.
    size_t pos;       ///< Текуща позиция в низа.

    /** @brief Прескача празни символи (whitespace). */
    void skipWhitespace();

    /**
     * @brief Парсва низов литерал (string).
     * @return Токен от тип STRING.
     */
    Token parseString();

    /**
     * @brief Парсва число.
     * @return Токен от тип NUMBER.
     */
    Token parseNumber();

    /**
     * @brief Проверява за съвпадение с фиксиран литерал (true, false, null).
     * @param literal Очакваният литерал.
     * @param type Типът токен, с който да се асоциира.
     * @return Токен от съответния тип.
     * @throws std::runtime_error ако литералът не съвпада.
     */
    Token matchLiteral(const std::string &literal, TokenType type);
};
