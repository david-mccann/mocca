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

#pragma once

#include "mocca/base/Error.h"

#include <functional>
#include <string>
#include <vector>

namespace mocca {

class CommandLineParser {
public:
    class ParserError : public Error {
    public:
        ParserError(const std::string& msg, const std::string& file, int line)
            : Error(msg, file, line) {}
    };

    struct OptionValue {
        std::string value;
        std::string help;
    };
    struct Option {
        std::string key;
        std::string help;
        std::vector<OptionValue> allowedValues;
        std::function<void(const std::string& value)> callback;
    };

    CommandLineParser();

    void addOption(const Option& option);
    void parse(int argc, const char** argv);

private:
    void printHelp() const;

private:
    std::vector<Option> options_;
};
}