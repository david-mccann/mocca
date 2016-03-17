#include "mocca/base/StringTools.h"

void mocca::replaceAll(std::string& target, const std::string& search, const std::string& replace) {
    if (search.empty()) {
        return;
    }
    size_t pos = 0;
    while ((pos = target.find(search, pos)) != std::string::npos) {
        target.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}