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

#pragma once

#include "mocca/net/IStreamConnection.h"

#include <chrono>

namespace mocca {
namespace net {

void readUntil(IStreamConnection& stream, std::vector<uint8_t>& buffer, const std::string& delim);
void readExactly(IStreamConnection& stream, std::vector<uint8_t>& buffer, uint32_t size);
void sendAll(IStreamConnection& stream, const uint8_t* buffer, uint32_t size);

template <typename T> T readValue(IStreamConnection& stream) {
    std::vector<uint8_t> buffer;
    readExactly(stream, buffer, sizeof(T));
    return *reinterpret_cast<T*>(buffer.data());
}

template <typename T> void sendValue(IStreamConnection& stream, T value) {
    sendAll(stream, reinterpret_cast<uint8_t*>(&value), sizeof(T));
}
}
}