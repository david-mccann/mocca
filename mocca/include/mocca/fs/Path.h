/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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
    operator std::string() const;

    Path parent() const;

private:
    std::string path_;
};

bool operator==(const Path& lhs, const Path& rhs);
Path operator+(const Path& lhs, const std::string& rhs);
}
}