/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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