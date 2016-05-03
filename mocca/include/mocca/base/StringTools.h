/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <ostream>

#include "mocca/base/Error.h"

namespace mocca {

// split a string into a number of substrings, e.g., splitString("Sherlock,Mycroft,Moriarty", ',')
// -> ["Sherlock","Mycroft","Moriarty"]
template <typename T> std::vector<T> splitString(const std::string& str, char delimiter) {
    std::vector<T> result;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        T value;
        std::istringstream(token) >> value;
        result.push_back(std::move(value));
    }
    return result;
}

// replace a number of placeholders by values in a string
// e.g., format("First %% Third %%, "Second", 4) -> "First Second Third 4"
template <typename T> std::string formatString(const std::string& str, const T& value) {
    auto pos = str.find("%%");
    if (pos == std::string::npos) {
        throw Error("Number of placeholders does not match number of arguments", __FILE__, __LINE__);
    }

    std::ostringstream oss;
    oss << value;
    std::string valueStr = oss.str();

    std::string result = str;
    return result.replace(pos, 2, valueStr);
}
template <typename T, typename... Args> std::string formatString(const std::string& str, const T& value, const Args&... args) {
    return formatString(formatString(str, value), args...);
}

template <typename Iter> std::string makeString(Iter it, Iter itEnd, const std::string& separator = ", ") {
    std::ostringstream oss;
    for (; it != itEnd - 1; ++it) {
        oss << *it << separator;
    }
    oss << *it;
    return oss.str();
}

// join a number of items to a string, e.g., join("Hello ", 42) -> "Hello 42"
template <typename T> std::string joinString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
template <typename T, typename... Args> std::string joinString(const T& value, const Args&... args) {
    return joinString(value) + joinString(args...);
}

// replace all occurrence of a search string in a target string with another string
void replaceAll(std::string& target, const std::string& search, const std::string& replace);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    std::stringstream str;
    str << "[";
    for (auto it = begin(vec); it != end(vec); ++it) {
        str << *it;
        if (it != --end(vec)) {
            str << ", ";
        }
    }
    str << "]";
    return os << str.str();
}