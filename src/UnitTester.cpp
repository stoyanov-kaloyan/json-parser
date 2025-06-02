#include "UnitTester.h"
#include "JsonCLI.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

void UnitTester::runAll()
{
    createInitialTestFile();
    testSetAndPrint();
    testCreateNewPath();
    testDelete();
    testMove();
    testSaveSubtree();
    std::cout << "All tests passed.\n";
}

void UnitTester::createInitialTestFile()
{
    std::ofstream out(testFile);
    out << R"({
                "person": {
                    "name": "Initial",
                    "age": 25
                },
                "old": {
                    "key": "value"
                }
            })";
    out.close();
}

void UnitTester::testSetAndPrint()
{
    JsonCLI cli;
    cli.commandOpen(testFile);
    cli.commandSet("person/name", "\"Kaloyan\"");

    std::ostringstream capture;
    std::streambuf *oldBuf = std::cout.rdbuf(capture.rdbuf());
    cli.commandPrint();
    std::cout.rdbuf(oldBuf);

    std::string output = capture.str();
    assert(output.find("\"Kaloyan\"") != std::string::npos);
}

void UnitTester::testCreateNewPath()
{
    JsonCLI cli;
    cli.commandOpen(testFile);
    cli.commandCreate("new/info", "\"data\"");

    std::ostringstream capture;
    std::streambuf *oldBuf = std::cout.rdbuf(capture.rdbuf());
    cli.commandPrint();
    std::cout.rdbuf(oldBuf);

    std::string output = capture.str();
    assert(output.find("\"info\": \"data\"") != std::string::npos);
}

void UnitTester::testDelete()
{
    JsonCLI cli;
    cli.commandOpen(testFile);
    cli.commandDelete("person/age");

    std::ostringstream capture;
    std::streambuf *oldBuf = std::cout.rdbuf(capture.rdbuf());
    cli.commandPrint();
    std::cout.rdbuf(oldBuf);

    std::string output = capture.str();
    assert(output.find("age") == std::string::npos);
}

void UnitTester::testMove()
{
    JsonCLI cli;
    cli.commandOpen(testFile);
    cli.commandMove("old/key", "newkey/moved");

    std::ostringstream capture;
    std::streambuf *oldBuf = std::cout.rdbuf(capture.rdbuf());
    cli.commandPrint();
    std::cout.rdbuf(oldBuf);

    std::string output = capture.str();
    assert(output.find("newkey") != std::string::npos);
    assert(output.find("moved") != std::string::npos);
    assert(output.find("\"key\":") == std::string::npos);
}

void UnitTester::testSaveSubtree()
{
    JsonCLI cli;
    cli.commandOpen(testFile);
    cli.commandSaveAs("subtree.json", "person");
    std::ifstream saved("subtree.json");
    std::string contents((std::istreambuf_iterator<char>(saved)), std::istreambuf_iterator<char>());
    assert(contents.find("name") != std::string::npos);
    assert(contents.find("age") != std::string::npos);
    std::cout << "testSaveSubtree passed.\n";
}
