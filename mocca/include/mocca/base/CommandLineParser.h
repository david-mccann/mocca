#pragma once

#include <functional>
#include <vector>
#include <string>

namespace mocca {

class CommandLineParser {
public:
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
    void printHelp(const std::string&) const;

private:
    std::vector<Option> options_;
};
}