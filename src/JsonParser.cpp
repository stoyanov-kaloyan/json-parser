#include "JsonParser.h"

JsonNode *JsonParser::parse()
{
    JsonNode *node = parseValue();
    if (current.type != TokenType::END)
        throw std::runtime_error("Unexpected token after JSON value");
    return node;
}

const char *tokenTypeName(TokenType type)
{
    switch (type)
    {
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::LEFT_BRACKET:
        return "LEFT_BRACKET";
    case TokenType::RIGHT_BRACKET:
        return "RIGHT_BRACKET";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::COLON:
        return "COLON";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::FALSE:
        return "FALSE";
    case TokenType::NIL:
        return "NIL";
    case TokenType::END:
        return "END";
    default:
        return "UNKNOWN";
    }
}

void JsonParser::advance()
{
    current = tokenizer.nextToken();
}
void JsonParser::expect(TokenType expected)
{
    if (current.type != expected)
    {
        std::ostringstream oss;
        oss << "Unexpected token: expected "
            << tokenTypeName(expected)
            << ", but got "
            << tokenTypeName(current.type);
        throw std::runtime_error(oss.str());
    }
    advance();
}

JsonNode *JsonParser::parseValue()
{
    switch (current.type)
    {
    case TokenType::STRING:
    {
        auto val = makeString(std::get<std::string>(current.value).c_str());
        advance();
        return val;
    }
    case TokenType::NUMBER:
    {
        auto val = makeNumber(std::get<double>(current.value));
        advance();
        return val;
    }
    case TokenType::TRUE:
        advance();
        return makeBoolean(true);
    case TokenType::FALSE:
        advance();
        return makeBoolean(false);
    case TokenType::NIL:
        advance();
        return makeNull();
    case TokenType::LEFT_BRACE:
        return parseObject();
    case TokenType::LEFT_BRACKET:
        return parseArray();
    default:
        throw std::runtime_error("Invalid JSON value");
    }
}

JsonNode *JsonParser::parseArray()
{
    expect(TokenType::LEFT_BRACKET);
    JsonNode *arr = makeArray();
    if (current.type == TokenType::RIGHT_BRACKET)
    {
        advance();
        return arr;
    }
    while (true)
    {
        arr->addArrayElement(parseValue());
        if (current.type == TokenType::RIGHT_BRACKET)
        {
            advance();
            break;
        }
        expect(TokenType::COMMA);
    }
    return arr;
}

JsonNode *JsonParser::parseObject()
{
    expect(TokenType::LEFT_BRACE);
    JsonNode *obj = makeObject();

    if (current.type == TokenType::RIGHT_BRACE)
    {
        advance();
        return obj;
    }

    while (true)
    {
        if (current.type != TokenType::STRING)
            throw std::runtime_error("Expected string as key in object");

        std::string key = std::get<std::string>(current.value);
        advance();

        expect(TokenType::COLON);
        obj->addObjectMember(key.c_str(), parseValue());

        if (current.type == TokenType::RIGHT_BRACE)
        {
            advance();
            break;
        }

        expect(TokenType::COMMA);
    }

    return obj;
}

JsonNode *parseJson(const std::string &src)
{
    JsonParser parser(src);
    return parser.parse();
}
