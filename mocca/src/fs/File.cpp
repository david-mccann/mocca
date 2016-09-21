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