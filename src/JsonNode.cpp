#include "JsonNode.h"
#include <iostream>
#include <cstring>

JsonNode::JsonNode(JsonType t) : type(t)
{
    switch (type)
    {
    case JsonType::OBJECT:
        object = nullptr;
        break;
    case JsonType::ARRAY:
        array = nullptr;
        break;
    case JsonType::STRING:
        str = nullptr;
        break;
    case JsonType::NUMBER:
        number = 0;
        break;
    case JsonType::BOOLEAN:
        boolean = false;
        break;
    case JsonType::NIL:
        break;
    }
}

JsonNode::~JsonNode()
{
    switch (type)
    {
    case JsonType::OBJECT:
        while (object)
        {
            KeyValue *temp = object->next;
            delete[] object->key;
            delete object->value;
            delete object;
            object = temp;
        }
        break;
    case JsonType::ARRAY:
        while (array)
        {
            ArrayElement *temp = array->next;
            delete array->value;
            delete array;
            array = temp;
        }
        break;
    case JsonType::STRING:
        delete[] str;
        break;
    default:
        break;
    }
}

void JsonNode::addObjectMember(const char *key, JsonNode *value)
{
    if (type != JsonType::OBJECT)
        return;

    KeyValue **prev = &object;
    while (*prev)
    {
        if (strcmp((*prev)->key, key) == 0)
        {
            KeyValue *toDelete = *prev;
            *prev = toDelete->next;
            delete[] toDelete->key;
            delete toDelete->value;
            delete toDelete;
            continue;
        }
        prev = &(*prev)->next;
    }

    KeyValue *kv = new KeyValue;
    kv->key = new char[strlen(key) + 1];
    strcpy(kv->key, key);
    kv->value = value;
    kv->next = object;
    object = kv;
}

void JsonNode::addArrayElement(JsonNode *value)
{
    if (type != JsonType::ARRAY)
        return;
    ArrayElement *el = new ArrayElement{value, nullptr};
    if (!array)
        array = el;
    else
    {
        ArrayElement *curr = array;
        while (curr->next)
            curr = curr->next;
        curr->next = el;
    }
}

JsonNode *JsonNode::getObjectMember(const char *key)
{
    if (type != JsonType::OBJECT)
        return nullptr;
    for (KeyValue *kv = object; kv; kv = kv->next)
        if (strcmp(kv->key, key) == 0)
            return kv->value;
    return nullptr;
}

void JsonNode::print(int indent)
{
    switch (type)
    {
    case JsonType::OBJECT:
    {
        std::cout << "{\n";
        KeyValue *kv = object;
        while (kv)
        {
            std::cout << std::string(indent + 2, ' ')
                      << '"' << kv->key << "\": ";
            kv->value->print(indent + 2);
            if (kv->next)
                std::cout << ",";
            std::cout << "\n";
            kv = kv->next;
        }
        std::cout << std::string(indent, ' ') << "}";
        break;
    }
    case JsonType::ARRAY:
    {
        std::cout << "[\n";
        for (ArrayElement *el = array; el; el = el->next)
        {
            std::cout << std::string(indent + 2, ' ');
            el->value->print(indent + 2);
            if (el->next)
                std::cout << ",";
            std::cout << "\n";
        }
        std::cout << std::string(indent, ' ') << "]";
        break;
    }
    case JsonType::STRING:
        std::cout << '"' << str << '"';
        break;
    case JsonType::NUMBER:
        std::cout << number;
        break;
    case JsonType::BOOLEAN:
        std::cout << (boolean ? "true" : "false");
        break;
    case JsonType::NIL:
        std::cout << "null";
        break;
    }
}

JsonNode *makeString(const char *s)
{
    JsonNode *node = new JsonNode(JsonType::STRING);
    node->str = new char[strlen(s) + 1];
    strcpy(node->str, s);
    return node;
}

JsonNode *makeNumber(double val)
{
    JsonNode *node = new JsonNode(JsonType::NUMBER);
    node->number = val;
    return node;
}

JsonNode *makeBoolean(bool b)
{
    JsonNode *node = new JsonNode(JsonType::BOOLEAN);
    node->boolean = b;
    return node;
}

JsonNode *makeNull()
{
    return new JsonNode(JsonType::NIL);
}

JsonNode *makeObject()
{
    return new JsonNode(JsonType::OBJECT);
}

JsonNode *makeArray()
{
    return new JsonNode(JsonType::ARRAY);
}

JsonNode *getArrayElementAt(JsonNode *arrayNode, int index)
{
    if (!arrayNode || arrayNode->type != JsonType::ARRAY)
        return nullptr;

    ArrayElement *curr = arrayNode->array;
    int i = 0;
    while (curr)
    {
        if (i == index)
            return curr->value;
        curr = curr->next;
        ++i;
    }
    return nullptr;
}