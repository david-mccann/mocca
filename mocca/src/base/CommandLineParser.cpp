#include "mocca/base/CommandLineParser.h"

#include "mocca/base/Containers.h"
#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"

#include <iostream>
#include <sstream>

using namespace mocca;

CommandLineParser::CommandLineParser() {
    Option usage;
    usage.key = "--usage";
    usage.help = "prints usage";
    usage.callback = std::bind(&CommandLineParser::printUsage, this, std::placeholders::_1);
    options_.push_back(usage);
}

void CommandLineParser::addOption(const Option& option) {
    options_.push_back(option);
}

void mocca::CommandLineParser::printUsage(const std::string&) const {
    std::stringstream stream;
    for (const auto& opt : options_) {
        stream << opt.key << ": " << opt.help << std::endl;
        if (!opt.allowedValues.empty()) {
            for (const auto& val : opt.allowedValues) {
                stream << "\t" << val.value << ": " << val.help << std::endl;
            }
        }
    }
    std::cout << stream.str();
    std::exit(0);
}

void CommandLineParser::parse(int argc, const char** argv) {
    for (int i = 1; i < argc; ++i) { // skip first arg because it contains the program name
        auto arg = std::string(argv[i]);
        auto separatorIndex = arg.find_first_of("=");
        auto argKey = arg.substr(0, separatorIndex);
        if (findMemberEqual(options_, &Option::key, argKey) == end(options_)) {
            throw Error(formatString("Unknown option '%%'", argKey), __FILE__, __LINE__);
        }
        std::string argValue;
        if (separatorIndex != std::string::npos) {
            argValue = arg.substr(separatorIndex + 1, std::string::npos);
        }
        for (const auto& opt : options_) {
            if (opt.key == argKey) {
                if (!opt.allowedValues.empty()) {
                    if (findMemberEqual(opt.allowedValues, &OptionValue::value, argValue) == end(opt.allowedValues)) {
                        auto allowedValues = collectMembers(begin(opt.allowedValues), end(opt.allowedValues), &OptionValue::value);
                        auto allowedValuesStr = makeString(begin(allowedValues), end(allowedValues));
                        throw Error(
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