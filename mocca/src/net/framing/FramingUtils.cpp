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

#include "mocca/net/framing/FramingUtils.h"
#include "mocca/base/Thread.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

void mocca::net::readUntil(IStreamConnection& stream, std::vector<uint8_t>& buffer, const std::string& delim) {
    bool cont = true;
    uint8_t byte;
    while (cont) {
        if (Runnable::isCurrentInterrupted()) {
            throw ThreadInterrupt(__FILE__, __LINE__);
        }
        auto bytesReceived = stream.receive(&byte, 1);
        auto offset = buffer.size() - std::min(static_cast<uint32_t>(delim.size()), static_cast<uint32_t>(buffer.size()));
        buffer.push_back(byte);
        auto searchBegin = buffer.data() + offset;
        auto searchEnd = buffer.data() + buffer.size();
        auto searchIt = std::search(searchBegin, searchEnd, begin(delim), end(delim));
        if (searchIt != searchEnd) {
            cont = false;
        }
    }
}

void mocca::net::readExactly(IStreamConnection& stream, std::vector<uint8_t>& buffer, uint32_t size) {
    uint32_t bytesTotal = 0;
    std::vector<uint8_t> chunk(size);
    while (bytesTotal < size) {
        if (Runnable::isCurrentInterrupted()) {
            throw ThreadInterrupt(__FILE__, __LINE__);
        }
        auto bytesReceived = stream.receive(chunk.data(), size - bytesTotal);
        buffer.insert(end(buffer), begin(chunk), begin(chunk) + bytesReceived);
        bytesTotal += bytesReceived;
        chunk.clear();
        chunk.resize(size);
    }
}

void mocca::net::sendAll(IStreamConnection& stream, const uint8_t* buffer, uint32_t size) {
    uint32_t bytesTotal = 0;
    while (bytesTotal < size) {
        if (Runnable::isCurrentInterrupted()) {
            throw ThreadInterrupt(__FILE__, __LINE__);
        }
        auto bytesSent = stream.send(buffer + bytesTotal, size - bytesTotal);
        bytesTotal += bytesSent;
    }
}