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