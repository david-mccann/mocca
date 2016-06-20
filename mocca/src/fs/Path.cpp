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

#include "mocca/fs/Path.h"

#include <algorithm>

using namespace mocca::fs;

Path::Path(const std::string& path)
    : path_(path) {
    std::replace(begin(path_), end(path_), '\\', '/');
}

std::string Path::directory() const {
    std::string dir = path_.substr(0, path_.find_last_of("/"));
    if (path_.empty()) {
        dir = "./";
    }
    return dir;
}

std::string Path::filename() const {
    size_t index = path_.find_last_of("/") + 1;
    return path_.substr(index);
}

std::string Path::filenameNoExt() const {
    size_t indexSlash = path_.find_last_of("/") + 1;
    size_t indexDot = path_.find_last_of(".");
    return path_.substr(indexSlash, indexDot - indexSlash);
}

std::string Path::extension() const {
    size_t indexDot = path_.find_last_of(".");
    size_t indexSlash = path_.find_last_of("/");
    if (indexDot == std::string::npos || (indexSlash != std::string::npos && indexDot < indexSlash)) {
        return "";
    }
    return path_.substr(indexDot);
}

std::string Path::toString() const {
    return path_;
}

mocca::fs::Path::operator std::string() const {
    return toString();
}

Path Path::parent() const {
    size_t index = path_.find_last_of("/");
    if (index == std::string::npos) {
        return Path();
    }
    return Path(path_.substr(0, index));
}

bool mocca::fs::operator==(const Path& lhs, const Path& rhs) {
    return lhs.toString() == rhs.toString();
}

Path mocca::fs::operator+(const Path& lhs, const std::string& rhs) {
    std::string infix = lhs.toString().back() == '/' ? "" : "/";
    return Path(lhs.toString() + infix + rhs);
}

std::ostream& mocca::fs::operator<<(std::ostream& os, const Path& obj) {
    return os << obj.toString();
}
