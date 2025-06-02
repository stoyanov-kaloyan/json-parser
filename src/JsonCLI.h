#pragma once
#include <string>
#include <vector>
#include "JsonNode.h"
#include <regex>

/**
 * @class JsonCLI
 * @brief Команден интерфейс за работа с JSON файлове.
 *
 * Позволява отваряне, валидиране, визуализиране, търсене, редактиране и запис на JSON данни.
 */
class JsonCLI
{
public:
    /** @brief Стартира командния интерфейс и обработва потребителски команди. */
    void run();

    /** @brief Отваря и зарежда JSON файл. */
    void commandOpen(const std::string &filename);

    /** @brief Валидира съдържанието на текущо заредения JSON. */
    void commandValidate();

    /** @brief Извежда съдържанието на JSON файла в четим формат. */
    void commandPrint();

    /** @brief Търси всички срещания на даден ключ в JSON дървото. */
    void commandSearch(const std::string &key);

    /** @brief Търси ключове в JSON, съвпадащи с даден регулярен израз. */
    void commandSearchRegex(const std::string &patternText);

    /** @brief Затваря текущо отворения JSON файл. */
    void commandClose();

    /** @brief Извежда помощно меню с всички поддържани команди. */
    void commandHelp();

    /**
     * @brief Задава нова стойност на елемент в JSON структурата.
     * @param path Път до елемента (напр. "person/name").
     * @param valueStr Стойност във вид на JSON низ.
     */
    void commandSet(const std::string &path, const std::string &valueStr);

    /**
     * @brief Създава нов елемент на даден път в JSON структурата.
     * @param path Път до новия елемент.
     * @param valueStr Стойност във вид на JSON низ.
     */
    void commandCreate(const std::string &path, const std::string &valueStr);

    /**
     * @brief Изтрива ключ от JSON обект.
     * @param path Път до ключа за изтриване.
     */
    void commandDelete(const std::string &path);

    /**
     * @brief Премества елемент от един път на друг.
     * @param fromPath Изходен път.
     * @param toPath Целеви път.
     */
    void commandMove(const std::string &fromPath, const std::string &toPath);

    /**
     * @brief Записва JSON съдържанието в последно отворения файл.
     * @param path (по избор) Подпът от JSON структурата, който да се запише.
     */
    void commandSave(const std::string &path = "");

    /**
     * @brief Записва JSON съдържание във файл.
     * @param filename Име на файла.
     * @param path (по избор) Подпът от JSON, който да се запише.
     */
    void commandSaveAs(const std::string &filename, const std::string &path = "");

private:
    JsonNode *root = nullptr;   ///< Коренов елемент на JSON дървото.
    std::string currentFile;    ///< Път до текущо отворения файл.
    std::string currentContent; ///< Сурово съдържание на JSON файла.

    /** @brief Чете съдържанието на файл и го връща като низ. */
    std::string readFile(const std::string &filename);

    /** @brief Проверява дали стойността изглежда като валиден JSON литерал. */
    bool looksLikeJsonLiteral(const std::string &value);

    /** @brief Рекурсивно търси ключове с точно съвпадение в JSON дървото. */
    void searchJsonExact(JsonNode *node, const std::string &key, std::vector<JsonNode *> &results);

    /** @brief Рекурсивно търси ключове по регулярен израз в JSON дървото. */
    void searchJsonRegex(JsonNode *node, const std::regex &pattern, std::vector<JsonNode *> &results);
};
