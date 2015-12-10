#include <sstream>

#include "mocca/base/Error.h"
#include "mocca/fs/File.h"

namespace mocca {
namespace fs {

File::File(const std::string& fileName)
    : fileName_(fileName)
    , stream_(fileName) {
    if (!stream_) {
        throw Error("Error opening file: " + fileName_, __FILE__, __LINE__);
    }
}

File::~File() {
    stream_.close();
}

std::string File::readAsString() const {
    std::stringstream output;
    if (stream_) {
        output << stream_.rdbuf();
    }
    return output.str();
}

bool File::fileExists(const std::string& fileName) {
    std::ifstream file(fileName);
    bool result = file.good();
    file.close();
    return result;
}
}
}