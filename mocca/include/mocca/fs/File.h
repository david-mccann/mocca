/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include <fstream>
#include <string>

namespace mocca {
namespace fs {

class File {
public:
    explicit File(const std::string& fileName);
    virtual ~File();

    std::string readAsString() const;

    static bool fileExists(const std::string& fileName);

private:
    std::string fileName_;
    std::ifstream stream_;
};
}
}