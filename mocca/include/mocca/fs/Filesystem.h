/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/fs/Path.h"

#include <vector>
#include <memory>

namespace mocca {
namespace fs {
bool exists(const Path& path);
void removeFile(const Path& path);
bool isDirectory(const Path& path);
void createDirectories(const Path& path);
std::vector<Path> directoryContents(const Path& path);
std::string readTextFile(const Path& path);
std::unique_ptr<std::vector<uint8_t>> readBinaryFile(const Path& path);
}
}