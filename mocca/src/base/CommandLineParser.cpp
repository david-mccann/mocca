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

#include "mocca/base/CommandLineParser.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"

#include <iostream>
#include <sstream>

using namespace mocca;

CommandLineParser::CommandLineParser() {
    Option usage;
    usage.key = "--help";
    usage.help = "prints help";
    usage.callback = [&](const std::string&) {
        printHelp();
        std::exit(0);
    };
    options_.push_back(usage);
}

void CommandLineParser::addOption(const Option& option) {
    options_.push_back(option);
}

void CommandLineParser::printHelp() const {
    std::stringstream stream;
    for (const auto& opt : options_) {
        stream << opt.key << ": " << opt.help << std::endl;
        if (!opt.allowedValues.empty()) {
            for (const auto& val : opt.allowedValues) {
                stream << "\t" << val.value << ": " << val.help << std::endl;
            }
        }
        stream << std::endl;
    }
    std::cout << stream.str();
}

void CommandLineParser::parse(int argc, const char** argv) {
    for (int i = 1; i < argc; ++i) { // skip first arg because it contains the program name
        auto arg = std::string(argv[i]);
        auto separatorIndex = arg.find_first_of("=");
        auto argKey = arg.substr(0, separatorIndex);
        if (findMemberEqual(begin(options_), end(options_), &Option::key, argKey) == end(options_)) {
            printHelp();
            throw ParserError("Unknown option: " + argKey, __FILE__, __LINE__);
        }
        std::string argValue;
        if (separatorIndex != std::string::npos) {
            argValue = arg.substr(separatorIndex + 1, std::string::npos);
        }
        for (const auto& opt : options_) {
            if (opt.key == argKey) {
                if (!opt.allowedValues.empty()) {
                    if (findMemberEqual(begin(opt.allowedValues), end(opt.allowedValues), &OptionValue::value, argValue) ==
                        end(opt.allowedValues)) {
                        auto allowedValues = collectMembers(begin(opt.allowedValues), end(opt.allowedValues), &OptionValue::value);
                        auto allowedValuesStr = makeString(begin(allowedValues), end(allowedValues));
                        throw ParserError(
                            formatString("Invalid value '%%' for option '%%'. Allowed values are: %%", argValue, opt.key, allowedValuesStr),
                            __FILE__, __LINE__);
                    }
                }
                opt.callback(argValue);
                break;
            }
        }
    }
}