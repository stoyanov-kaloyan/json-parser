#include "Tokenizer.h"
#include <cctype>
#include <cstdlib>
#include <stdexcept>

Token Tokenizer::nextToken()
{
    skipWhitespace();
    if (pos >= json.length())
        return {TokenType::END, {}};

    char c = json[pos];
    switch (c)
    {
    case '{':
        ++pos;
        return {TokenType::LEFT_BRACE, {}};
    case '}':
        ++pos;
        return {TokenType::RIGHT_BRACE, {}};
    case '[':
        ++pos;
        return {TokenType::LEFT_BRACKET, {}};
    case ']':
        ++pos;
        return {TokenType::RIGHT_BRACKET, {}};
    case ',':
        ++pos;
        return {TokenType::COMMA, {}};
    case ':':
        ++pos;
        return {TokenType::COLON, {}};
    case '"':
        return parseString();
    case 't':
        return matchLiteral("true", TokenType::TRUE);
    case 'f':
        return matchLiteral("false", TokenType::FALSE);
    case 'n':
        return matchLiteral("null", TokenType::NIL);
    default:
        if (c == '-' || std::isdigit(c))
            return parseNumber();
        throw std::runtime_error("Unexpected character in JSON input");
    }
}

void Tokenizer::skipWhitespace()
{
    while (pos < json.length() && std::isspace(json[pos]))
        ++pos;
}

Token Tokenizer::parseString()
{
    ++pos; // skip initial quote
    std::string result;
    while (pos < json.length())
    {
        char c = json[pos++];
        if (c == '"')
            break;
        if (c == '\\')
        {
            if (pos >= json.length())
                break;
            char esc = json[pos++];
            switch (esc)
            {
            case '"':
                result += '"';
                break;
            case '\\':
                result += '\\';
                break;
            case '/':
                result += '/';
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            default:
                throw std::runtime_error("Invalid escape character");
            }
        }
        else
        {
            result += c;
        }
    }
    return {TokenType::STRING, result};
}

Token Tokenizer::parseNumber()
{
    size_t start = pos;
    if (json[pos] == '-')
        ++pos;
    while (pos < json.length() && std::isdigit(json[pos]))
        ++pos;
    if (pos < json.length() && json[pos] == '.')
    {
        ++pos;
        while (pos < json.length() && std::isdigit(json[pos]))
            ++pos;
    }
    double value = std::stod(json.substr(start, pos - start));
    return {TokenType::NUMBER, value};
}

Token Tokenizer::matchLiteral(const std::string &literal, TokenType type)
{
    if (json.compare(pos, literal.length(), literal) != 0)
        throw std::runtime_error("Invalid literal");

    char next = (pos + literal.length() < json.length()) ? json[pos + literal.length()] : '\0';
    if (std::isalnum(next)) // ako lsedva alphanumeric ne e validen literal
        throw std::runtime_error("Invalid literal suffix");

    pos += literal.length();
    return {type, {}};
}