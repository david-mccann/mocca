#pragma once

#include <string>

namespace mocca {
namespace fs {

class Path {
public:
    Path() = default;
    explicit Path(const std::string& path);

    std::string directory() const;
    std::string filename() const;
    std::string extension() const;

    std::string toString() const;

    Path parent() const;

private:
    std::string path_;
};

bool operator==(const Path& lhs, const Path& rhs);
Path operator+(const Path& lhs, const std::string& rhs);
}
}