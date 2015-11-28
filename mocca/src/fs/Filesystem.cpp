#include "mocca/fs/Filesystem.h"

#include "mocca/base/Error.h"
#include "mocca/fs/Path.h"

#include <list>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
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