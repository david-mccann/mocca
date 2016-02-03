#pragma once

#include <string>

namespace mocca {
    namespace impl {
        std::string createIv();
    }

    std::string encryptString(const std::string& plainText, const std::string& key);
    std::string decryptString(const std::string& encodedText, const std::string& key);
}