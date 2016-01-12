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

namespace mocca {
namespace fs {
bool exists(const Path& path);
bool isDirectory(const Path& path);
void createDirectories(const Path& path);
std::vector<Path> directoryContents(const Path& path);
}
}