/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "gtest/gtest.h"

#include "mocca/base/CommandLineParser.h"
#include "mocca/base/Error.h"

using namespace mocca;

class CommandLineParserTest : public ::testing::Test {
protected:
    CommandLineParserTest() {
        // You can do set-up work for each test here.
    }

    virtual ~CommandLineParserTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(CommandLineParserTest, Callback) {
    CommandLineParser target;
    bool called = false;
    target.addOption({"someKey", "help", {}, [&](const std::string& val) { called = true; }});
    const char* argv[] = {"progName", "someKey"};
    target.parse(2, argv);
    ASSERT_TRUE(called);
}

TEST_F(CommandLineParserTest, CallbackValue) {
    CommandLineParser target;
    std::string result;
    target.addOption({ "someKey", "help",{}, [&](const std::string& val) { result = val; } });
    const char* argv[] = { "progName", "someKey=someVal" };
    target.parse(2, argv);
    ASSERT_EQ("someVal", result);
}

TEST_F(CommandLineParserTest, InvalidOption) {
    CommandLineParser target;
    bool called = false;
    target.addOption({"someKey", "help", {}, [&](const std::string& val) { called = true; }});
    const char* argv[] = {"progName", "invalidKey"};
    ASSERT_THROW(target.parse(2, argv), Error);
}

TEST_F(CommandLineParserTest, InvalidValue) {
    CommandLineParser target;
    bool called = false;
    target.addOption({"someKey", "help", {{"val1", "helpVal1"}, {"val2", "helpVal2"}}, [&](const std::string& val) { called = true; }});
    const char* argv[] = {"progName", "someKey=invalidVal"};
    ASSERT_THROW(target.parse(2, argv), Error);
}