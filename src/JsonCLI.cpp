#include "JsonCLI.h"
#include "JsonParser.h"
#include "JsonNode.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

void JsonCLI::run()
{
    std::string command;
    std::cout << "Type 'help' for a list of commands.\n";

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.substr(0, 5) == "open ")
        {
            commandOpen(command.substr(5));
        }
        else if (command == "validate")
        {
            commandValidate();
        }
        else if (command == "print")
        {
            commandPrint();
        }
        else if (command == "save" || command.rfind("save ", 0) == 0)
        {
            if (command == "save")
            {
                commandSave(""); // zapazva celiq koren
            }
            else
            {
                size_t quote = command.find('"', 5);
                if (quote != std::string::npos)
                {
                    size_t endQuote = command.find('"', quote + 1);
                    if (endQuote != std::string::npos)
                    {
                        std::string path = command.substr(quote + 1, endQuote - quote - 1);
                        commandSave(path);
                    }
                    else
                    {
                        std::cerr << "Invalid save syntax. Use: save or save \"path/to/object\"\n";
                    }
                }
                else
                {
                    std::cerr << "Invalid save syntax. Use: save or save \"path/to/object\"\n";
                }
            }
        }
        else if (command.substr(0, 7) == "search ")
        {
            commandSearch(command.substr(7));
        }
        else if (command.substr(0, 13) == "search-regex ")
        {
            commandSearchRegex(command.substr(13));
        }
        else if (command == "close")
        {
            commandClose();
        }
        else if (command == "help")
        {
            commandHelp();
        }
        else if (command == "exit")
        {
            break;
        }
        else if (command.substr(0, 7) == "create ")
        {
            size_t firstQuote = command.find('"', 7);
            size_t secondQuote = command.find('"', firstQuote + 1);
            size_t thirdQuote = command.find('"', secondQuote + 1);
            size_t fourthQuote = command.find('"', thirdQuote + 1);

            if (firstQuote != std::string::npos && secondQuote != std::string::npos &&
                thirdQuote != std::string::npos && fourthQuote != std::string::npos)
            {
                std::string path = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                std::string value = command.substr(thirdQuote + 1, fourthQuote - thirdQuote - 1);
                commandCreate(path, value);
            }
            else
            {
                std::cerr << "Invalid create syntax. Use: create \"path/to/key\" \"json_value\"\n";
            }
        }

        else if (command.substr(0, 7) == "delete ")
        {
            size_t firstQuote = command.find('"', 7);
            size_t secondQuote = command.find('"', firstQuote + 1);
            if (firstQuote != std::string::npos && secondQuote != std::string::npos)
            {
                std::string path = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                commandDelete(path);
            }
            else
            {
                std::cerr << "Invalid delete syntax. Use: delete \"path/to/key\"\n";
            }
        }
        else if (command.substr(0, 4) == "set ")
        {
            size_t firstQuote = command.find('"', 4);
            size_t secondQuote = command.find('"', firstQuote + 1);
            size_t thirdQuote = command.find('"', secondQuote + 1);
            size_t fourthQuote = command.find('"', thirdQuote + 1);

            if (firstQuote != std::string::npos && secondQuote != std::string::npos && thirdQuote != std::string::npos && fourthQuote != std::string::npos)
            {
                std::string path = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                std::string value = command.substr(thirdQuote + 1, fourthQuote - thirdQuote - 1);
                commandSet(path, value);
            }
            else
            {
                std::cerr << "Invalid set command syntax. Use: set \"path/to/key\" \"json_value\"";
            }
        }
        else if (command.substr(0, 5) == "move ")
        {
            size_t firstQuote = command.find('"', 5);
            size_t secondQuote = command.find('"', firstQuote + 1);
            size_t thirdQuote = command.find('"', secondQuote + 1);
            size_t fourthQuote = command.find('"', thirdQuote + 1);

            if (firstQuote != std::string::npos && secondQuote != std::string::npos &&
                thirdQuote != std::string::npos && fourthQuote != std::string::npos)
            {
                std::string fromPath = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                std::string toPath = command.substr(thirdQuote + 1, fourthQuote - thirdQuote - 1);
                commandMove(fromPath, toPath);
            }
            else
            {
                std::cerr << "Invalid move syntax. Use: move \"from/path\" \"to/path\"\n";
            }
        }

        else if (command.substr(0, 5) == "save ")
        {
            size_t quote = command.find('"', 5);
            if (quote == std::string::npos)
            {
                commandSave("");
            }
            else
            {
                size_t endQuote = command.find('"', quote + 1);
                if (endQuote != std::string::npos)
                {
                    std::string path = command.substr(quote + 1, endQuote - quote - 1);
                    commandSave(path);
                }
                else
                {
                    std::cerr << "Invalid save syntax. Use: save or save \"path/to/object\"\n";
                }
            }
        }
        else if (command.substr(0, 7) == "saveas ")
        {
            size_t firstQuote = command.find('"', 7);
            size_t secondQuote = command.find('"', firstQuote + 1);
            size_t thirdQuote = command.find('"', secondQuote + 1);
            size_t fourthQuote = command.find('"', thirdQuote + 1);

            if (firstQuote != std::string::npos && secondQuote != std::string::npos)
            {
                std::string filename = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                if (thirdQuote != std::string::npos && fourthQuote != std::string::npos)
                {
                    std::string path = command.substr(thirdQuote + 1, fourthQuote - thirdQuote - 1);
                    commandSaveAs(filename, path);
                }
                else
                {
                    commandSaveAs(filename);
                }
            }
            else
            {
                std::cerr << "Invalid saveas syntax. Use: saveas \"file\" or saveas \"file\" \"path\"\n";
            }
        }
        else
        {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    if (root)
        delete root;
}

bool isArrayIndex(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool JsonCLI::looksLikeJsonLiteral(const std::string &value)
{
    if (value == "true" || value == "false" || value == "null")
        return true;

    if (!value.empty() && (value[0] == '{' || value[0] == '[' || value[0] == '"'))
        return true;

    char *endptr = nullptr;
    std::strtod(value.c_str(), &endptr);
    return endptr == value.c_str() + value.size();
}

std::string JsonCLI::readFile(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("Cannot open file: " + filename);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

void JsonCLI::commandOpen(const std::string &filename)
{
    if (root)
        delete root;
    root = nullptr;
    currentFile = filename;

    try
    {
        currentContent = readFile(filename);
        std::cout << "File loaded.\n";

        // avtomatichno validirane sled open komadna
        root = parseJson(currentContent);
        std::cout << "JSON is valid.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void JsonCLI::commandValidate()
{
    if (currentContent.empty())
    {
        std::cout << "No file loaded. Use 'open <file>' first.\n";
        return;
    }
    try
    {
        JsonNode *newRoot = parseJson(currentContent);

        if (!newRoot || (newRoot->type != JsonType::OBJECT && newRoot->type != JsonType::ARRAY))
        {
            delete newRoot;
            throw std::runtime_error("Root must be a JSON object or array, not a primitive.");
        }

        if (root)
            delete root;
        root = newRoot;
        std::cout << "JSON is valid.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "JSON validation failed: " << e.what() << "\n";
    }
}

void JsonCLI::commandPrint()
{
    if (!root)
    {
        std::cout << "No JSON loaded.\n";
        return;
    }
    root->print();
    std::cout << std::endl;
}

void JsonCLI::commandSave(const std::string &path)
{
    if (currentFile.empty())
    {
        std::cerr << "No file previously opened. Use saveas <file>.\n";
        return;
    }
    commandSaveAs(currentFile, path);
}
void JsonCLI::commandSaveAs(const std::string &filename, const std::string &path)
{
    if (!root)
    {
        std::cerr << "No JSON loaded.\n";
        return;
    }

    JsonNode *target = root;
    std::string lastKey = "";
    bool isSubPath = !path.empty();
    if (isSubPath)
    {
        std::istringstream ss(path);
        std::string segment;
        while (std::getline(ss, segment, '/'))
        {
            if (target->type != JsonType::OBJECT)
            {
                std::cerr << "Invalid path: '" << segment << "' is not an object.\n";
                return;
            }
            target = target->getObjectMember(segment.c_str());
            if (!target)
            {
                std::cerr << "Path segment '" << segment << "' not found.\n";
                return;
            }
            lastKey = segment;
        }
    }

    std::ofstream out(filename);
    if (!out)
    {
        std::cerr << "Cannot open file for writing: " << filename << "\n";
        return;
    }

    std::streambuf *oldBuf = std::cout.rdbuf(out.rdbuf());
    if (isSubPath)
    {
        std::cout << "{\n  \"" << lastKey << "\": ";
        target->print(2);
        std::cout << "\n}\n";
    }
    else
    {
        target->print(0);
        std::cout << "\n";
    }
    std::cout.rdbuf(oldBuf);

    std::cout << "Saved to file: " << filename << "\n";

    if (!path.empty())
    {
        std::cout << "You've written only a subpath of the JSON file.\n"
                  << "To prevent data loss or inconsistency, please reload the file manually using 'open " << filename << "'.\n"
                  << "Alternatively, use 'save' to overwrite the full in-memory JSON content.\n";
    }
}

void JsonCLI::commandSearch(const std::string &key)
{
    if (!root)
    {
        std::cout << "No JSON loaded.\n";
        return;
    }
    std::vector<JsonNode *> results;
    searchJsonExact(root, key, results);
    std::cout << "Found " << results.size() << " matching entries:\n";
    for (JsonNode *match : results)
    {
        match->print();
        std::cout << std::endl;
    }
}

void JsonCLI::commandSearchRegex(const std::string &patternText)
{
    if (!root)
    {
        std::cout << "No JSON loaded.\n";
        return;
    }
    try
    {
        std::regex pattern(patternText);
        std::vector<JsonNode *> results;
        searchJsonRegex(root, pattern, results);
        std::cout << "Found " << results.size() << " matching entries:\n";
        for (JsonNode *match : results)
        {
            match->print();
            std::cout << std::endl;
        }
    }
    catch (const std::regex_error &e)
    {
        std::cerr << "Invalid regex: " << e.what() << "\n";
    }
}

void JsonCLI::commandClose()
{
    if (root)
    {
        delete root;
        root = nullptr;
        currentFile.clear();
        currentContent.clear();
        std::cout << "Closed current file.\n";
    }
    else
    {
        std::cout << "No file is open.\n";
    }
}

void JsonCLI::commandHelp()
{
    std::cout << "Commands:\n"
                 "  open <file>             - Load JSON file into memory\n"
                 "  validate                - Validate and parse the loaded JSON\n"
                 "  print                   - Pretty print the parsed JSON\n"
                 "  move \"from\" \"to\"        - Move a key and its contents from one path to another\n"
                 "  save [\"path\"]             - Save current JSON or a sub-object to the last opened file\n"
                 "  saveas \"file\" [\"path\"]   - Save current JSON or sub-object to a new file\n"
                 "  search <key>            - Search for all occurrences of a specific key\n"
                 "  search-regex <regex>    - Search for keys matching a regular expression\n"
                 "  set \"path\" \"value\"     - Replace the value at a given path (if it exists)\n"
                 "  create \"path\" \"value\"  - Create a new key with the given value (recursively if needed)\n"
                 "  delete \"path\"           - Delete the key at the given path\n"
                 "  close                   - Close the current file\n"
                 "  help                    - Show this help message\n"
                 "  exit                    - Quit the program\n";
}

void JsonCLI::searchJsonExact(JsonNode *node, const std::string &key, std::vector<JsonNode *> &results)
{
    if (!node)
        return;
    if (node->type == JsonType::OBJECT)
    {
        KeyValue *kv = node->object;
        while (kv)
        {
            if (key == kv->key)
            {
                results.push_back(kv->value);
            }
            searchJsonExact(kv->value, key, results);
            kv = kv->next;
        }
    }
    else if (node->type == JsonType::ARRAY)
    {
        ArrayElement *el = node->array;
        while (el)
        {
            searchJsonExact(el->value, key, results);
            el = el->next;
        }
    }
}

void JsonCLI::searchJsonRegex(JsonNode *node, const std::regex &pattern, std::vector<JsonNode *> &results)
{
    if (!node)
        return;
    if (node->type == JsonType::OBJECT)
    {
        KeyValue *kv = node->object;
        while (kv)
        {
            if (std::regex_match(kv->key, pattern))
            {
                results.push_back(kv->value);
            }
            searchJsonRegex(kv->value, pattern, results);
            kv = kv->next;
        }
    }
    else if (node->type == JsonType::ARRAY)
    {
        ArrayElement *el = node->array;
        while (el)
        {
            searchJsonRegex(el->value, pattern, results);
            el = el->next;
        }
    }
}

void JsonCLI::commandSet(const std::string &path, const std::string &valueStr)
{
    if (!root)
    {
        std::cerr << "No JSON loaded.\n";
        return;
    }

    try
    {
        std::string actualValue = looksLikeJsonLiteral(valueStr) ? valueStr : "\"" + valueStr + "\"";
        JsonNode *newValue = parseJson(actualValue);

        std::istringstream ss(path);
        std::string segment;
        JsonNode *current = root;
        JsonNode *parent = nullptr;
        std::string lastSegment;

        std::vector<std::string> segments;
        while (std::getline(ss, segment, '/'))
        {
            segments.push_back(segment);
        }

        for (size_t i = 0; i < segments.size() - 1; ++i)
        {
            const std::string &seg = segments[i];
            parent = current;

            if (current->type == JsonType::OBJECT)
            {
                current = current->getObjectMember(seg.c_str());
            }
            else if (current->type == JsonType::ARRAY && isArrayIndex(seg))
            {
                current = getArrayElementAt(current, std::stoi(seg));
            }
            else
            {
                delete newValue;
                throw std::runtime_error("Invalid path segment: " + seg);
            }

            if (!current)
            {
                delete newValue;
                throw std::runtime_error("Path segment not found: " + seg);
            }
        }

        lastSegment = segments.back();

        if (current->type == JsonType::OBJECT)
        {
            current->addObjectMember(lastSegment.c_str(), newValue);
        }
        else if (current->type == JsonType::ARRAY && isArrayIndex(lastSegment))
        {
            int idx = std::stoi(lastSegment);
            ArrayElement *el = current->array;
            for (int i = 0; el; el = el->next, ++i)
            {
                if (i == idx)
                {
                    delete el->value;
                    el->value = newValue;
                    std::cout << "Value at path '" << path << "' updated successfully.\n";
                    return;
                }
            }
            delete newValue;
            throw std::runtime_error("Array index out of range.");
        }
        else
        {
            delete newValue;
            throw std::runtime_error("Cannot set value: unsupported node type.");
        }

        std::cout << "Value at path '" << path << "' updated successfully.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to set value: " << e.what() << "\n";
    }
}

void JsonCLI::commandCreate(const std::string &path, const std::string &valueStr)
{
    if (!root)
    {
        std::cerr << "No JSON loaded.\n";
        return;
    }

    if (root->type == JsonType::NIL)
    {
        root->type = JsonType::OBJECT;
        root->object = nullptr;
    }

    try
    {
        std::string actualValue = looksLikeJsonLiteral(valueStr) ? valueStr : "\"" + valueStr + "\"";
        JsonNode *newValue = parseJson(actualValue);

        std::istringstream ss(path);
        std::string segment;
        JsonNode *current = root;

        std::vector<std::string> segments;
        while (std::getline(ss, segment, '/'))
        {
            segments.push_back(segment);
        }

        for (size_t i = 0; i < segments.size() - 1; ++i)
        {
            const std::string &seg = segments[i];
            JsonNode *next = nullptr;

            if (current->type == JsonType::OBJECT)
            {
                next = current->getObjectMember(seg.c_str());
                if (!next)
                {
                    next = makeObject();
                    current->addObjectMember(seg.c_str(), next);
                }
                else if (next->type != JsonType::OBJECT && next->type != JsonType::ARRAY)
                {
                    delete newValue;
                    throw std::runtime_error("Path segment '" + seg + "' is not traversable");
                }
            }
            else if (current->type == JsonType::ARRAY && isArrayIndex(seg))
            {
                int idx = std::stoi(seg);
                ArrayElement *el = current->array;
                for (int j = 0; el && j < idx; el = el->next, ++j)
                    ;
                if (!el)
                {
                    delete newValue;
                    throw std::runtime_error("Array index out of range while traversing.");
                }
                next = getArrayElementAt(current, idx);
            }
            else
            {
                delete newValue;
                throw std::runtime_error("Invalid path segment type.");
            }

            current = next;
        }

        const std::string &lastSegment = segments.back();

        if (current->type == JsonType::OBJECT)
        {
            if (current->getObjectMember(lastSegment.c_str()))
            {
                delete newValue;
                throw std::runtime_error("Key '" + lastSegment + "' already exists.");
            }
            current->addObjectMember(lastSegment.c_str(), newValue);
            std::cout << "Created key '" << lastSegment << "' in object.\n";
        }
        else if (current->type == JsonType::ARRAY && isArrayIndex(lastSegment))
        {
            int idx = std::stoi(lastSegment);
            ArrayElement *el = current->array;
            int count = 0;
            while (el)
            {
                if (count == idx)
                {
                    delete newValue;
                    throw std::runtime_error("Array index '" + lastSegment + "' already exists.");
                }
                el = el->next;
                ++count;
            }
            current->addArrayElement(newValue);
            std::cout << "Appended value to array at index " << idx << ".\n";
        }
        else
        {
            delete newValue;
            throw std::runtime_error("Cannot create at this path — unsupported parent type.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create value: " << e.what() << "\n";
    }
}

void JsonCLI::commandDelete(const std::string &path)
{
    if (!root)
    {
        std::cerr << "No JSON loaded.";
        return;
    }

    std::istringstream ss(path);
    std::string segment;
    JsonNode *current = root;
    JsonNode *parent = nullptr;
    std::string lastKey;

    try
    {
        while (std::getline(ss, segment, '/'))
        {
            if (current->type != JsonType::OBJECT)
            {
                throw std::runtime_error("Path segment '" + segment + "' is not an object");
            }
            parent = current;
            current = current->getObjectMember(segment.c_str());
            if (!current)
            {
                throw std::runtime_error("Key '" + segment + "' not found.");
            }
            lastKey = segment;
        }

        if (parent)
        {
            KeyValue **prev = &parent->object;
            while (*prev)
            {
                if (std::string((*prev)->key) == lastKey)
                {
                    KeyValue *toDelete = *prev;
                    *prev = toDelete->next;
                    delete[] toDelete->key;
                    delete toDelete->value;
                    delete toDelete;
                    std::cout << "Deleted key: " << lastKey << "\n";
                    return;
                }
                prev = &(*prev)->next;
            }
        }
        else
        {
            throw std::runtime_error("Invalid path.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to delete: " << e.what() << "";
    }
}
void cleanupEmptyObjects(JsonNode *node)
{
    if (!node || node->type != JsonType::OBJECT)
        return;

    KeyValue **prev = &node->object;
    while (*prev)
    {
        KeyValue *current = *prev;
        JsonNode *child = current->value;

        if (child->type == JsonType::OBJECT && child->object == nullptr)
        {
            // iztriva obekta ako e prazen
            *prev = current->next;
            delete[] current->key;
            delete current->value;
            delete current;
        }
        else
        {
            cleanupEmptyObjects(child);
            prev = &(*prev)->next;
        }
    }
}

void JsonCLI::commandMove(const std::string &fromPath, const std::string &toPath)
{
    if (!root)
    {
        std::cerr << "No JSON loaded.\n";
        return;
    }

    std::istringstream fromSS(fromPath);
    std::string segment;
    JsonNode *fromCurrent = root;
    JsonNode *fromParent = nullptr;
    std::string fromKey;

    try
    {
        while (std::getline(fromSS, segment, '/'))
        {
            if (fromCurrent->type != JsonType::OBJECT)
                throw std::runtime_error("Invalid from-path: '" + segment + "' is not an object");
            fromParent = fromCurrent;
            fromCurrent = fromCurrent->getObjectMember(segment.c_str());
            if (!fromCurrent)
                throw std::runtime_error("Key '" + segment + "' not found in from-path");
            fromKey = segment;
        }

        if (!fromParent)
            throw std::runtime_error("Invalid from-path");

        JsonNode *valueToMove = fromCurrent;

        KeyValue **kv = &fromParent->object;
        while (*kv)
        {
            if (std::string((*kv)->key) == fromKey)
            {
                KeyValue *toDelete = *kv;
                *kv = toDelete->next;
                delete[] toDelete->key;
                delete toDelete;
                break;
            }
            kv = &(*kv)->next;
        }

        std::istringstream toSS(toPath);
        JsonNode *toCurrent = root;
        std::string toSegment;
        std::string toKey;

        while (std::getline(toSS, toSegment, '/'))
        {
            if (toSS.peek() == EOF)
            {
                toKey = toSegment;
                break;
            }

            JsonNode *next = toCurrent->getObjectMember(toSegment.c_str());
            if (!next)
            {
                JsonNode *newObj = makeObject();
                toCurrent->addObjectMember(toSegment.c_str(), newObj);
                next = newObj;
            }
            else if (next->type != JsonType::OBJECT)
            {
                throw std::runtime_error("Path segment '" + toSegment + "' is not an object");
            }

            toCurrent = next;
        }

        if (toCurrent->getObjectMember(toKey.c_str()))
            throw std::runtime_error("Destination key already exists: " + toKey);

        toCurrent->addObjectMember(toKey.c_str(), valueToMove);
        std::cout << "Moved '" << fromPath << "' to '" << toPath << "'\n";
        cleanupEmptyObjects(root);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Move failed: " << e.what() << "\n";
    }
}
