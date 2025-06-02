#pragma once
#include <string>

/**
 * @class UnitTester
 * @brief Клас за модулно тестване на функционалностите на JsonCLI.
 *
 * Изпълнява тестове за създаване, редактиране, изтриване, преместване и запис на JSON елементи.
 */
class UnitTester
{
public:
    /** @brief Стартира всички налични тестове. */
    void runAll();

private:
    const std::string testFile = "test.json"; ///< Име на тестовия JSON файл.

    /** @brief Създава начално JSON съдържание за тестовете. */
    void createInitialTestFile();

    /** @brief Тества командите set и print. */
    void testSetAndPrint();

    /** @brief Тества създаването на нов път с команда create. */
    void testCreateNewPath();

    /** @brief Тества изтриване на елемент чрез delete. */
    void testDelete();

    /** @brief Тества преместването на елемент с команда move. */
    void testMove();

    /** @brief Тества запис на поддърво от JSON в нов файл. */
    void testSaveSubtree();
};
