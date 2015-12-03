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
    std::string name = path_.substr(index, path_.length() - index);
    return name;
}

std::string Path::extension() const {
    size_t indexDot = path_.find_last_of(".");
    size_t indexSlash = path_.find_last_of("/");
    if (indexDot == std::string::npos || (indexSlash != std::string::npos && indexDot < indexSlash)) {
        return "";
    }
    std::string ext = path_.substr(indexDot);
    return ext;
}

std::string Path::toString() const {
    return path_;
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