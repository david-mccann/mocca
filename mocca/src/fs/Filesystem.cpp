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

#include "mocca/fs/Filesystem.h"

#include "mocca/base/Error.h"
#include "mocca/fs/Path.h"
#include "mocca/base/Memory.h"

#include <fstream>
#include <list>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace mocca::fs;

bool mocca::fs::exists(const Path& path) {
#ifdef _WIN32
    DWORD dwAttrib = GetFileAttributesA(path.toString().c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
    struct stat buf;
    return (stat(path.toString().c_str(), &buf) == 0);
#endif
}

void mocca::fs::removeFile(const Path& path) {
#ifdef WIN32
    DeleteFileA(path.toString().c_str());
#else
    unlink(path.toString().c_str());
#endif
}

bool mocca::fs::isDirectory(const Path& path) {
#ifdef _WIN32
    DWORD dwAttrib = GetFileAttributesA(path.toString().c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat buf;
    return (stat(path.toString().c_str(), &buf) == 0 && S_ISDIR(buf.st_mode));
#endif
}

void mocca::fs::createDirectories(const Path& path) {
    std::list<Path> paths;
    for (auto it = path; !(it == Path()) && !exists(it); it = it.parent()) {
        paths.push_front(it);
    }
    for (auto it : paths) {
#ifdef _WIN32
        CreateDirectoryA(it.toString().c_str(), NULL);
#else
        mkdir(it.toString().c_str(), 0755);
#endif
    }
}

std::vector<Path> mocca::fs::directoryContents(const Path& path) {
    if (!isDirectory(path)) {
        throw Error(path.toString() + " is not a directory", __FILE__, __LINE__);
    }

    std::vector<Path> result;
#ifdef _WIN32
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;
    hFind = FindFirstFileA((path + "*.*").toString().c_str(), &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (std::strcmp(FindFileData.cFileName, ".") && std::strcmp(FindFileData.cFileName, "..")) {
                result.push_back(path + FindFileData.cFileName);
            }
        } while (FindNextFileA(hFind, &FindFileData));
    }
    FindClose(hFind);
#else
    DIR* dirData = opendir(path.toString().c_str());
    if (dirData != NULL) {
        struct dirent* finfo;
        while ((finfo = readdir(dirData)) != NULL) {
            std::string dirIt = finfo->d_name;
            if (dirIt != "." && dirIt != "..") {
                result.push_back(path + dirIt);
            }
        }
        closedir(dirData);
    }
#endif
    return result;
}

std::string mocca::fs::readTextFile(const Path& path) {
    std::ifstream file(path);
    std::stringstream stream;
    std::string line;
    while (std::getline(file, line)) {
        stream << line << std::endl;
    }
    file.close();
    return stream.str();
}

std::unique_ptr<std::vector<uint8_t>> mocca::fs::readBinaryFile(const Path& path) {
    std::ifstream file(path.toString().data(), std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        int size = static_cast<int>(file.tellg());
        auto buffer = mocca::make_unique<std::vector<uint8_t>>(size);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(buffer->data()), size);
        file.close();
        return buffer;
    }
    return nullptr;
}

mocca::fs::Path mocca::fs::tempPath() {
#if WIN32
    TCHAR buffer[MAX_PATH + 1];
    DWORD size = GetTempPath(MAX_PATH + 1, buffer);
    return Path(std::string(buffer, size));
#else
    return Path("/tmp");
#endif
}
