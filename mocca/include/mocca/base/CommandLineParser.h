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