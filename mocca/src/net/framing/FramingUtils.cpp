/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/framing/FramingUtils.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

ReadStatus mocca::net::readUntil(IStreamConnection& stream, std::vector<uint8_t>& buffer, const std::string& delim, std::chrono::milliseconds timeout) {
    bool cont = true;
    uint8_t byte;
    while (cont) {
        auto bytesReceived = stream.receive(&byte, 1, timeout);
        if (bytesReceived == 0) {
            return ReadStatus::Incomplete;
        }
        auto offset = buffer.size() - std::min(static_cast<uint32_t>(delim.size()), buffer.size());
        buffer.push_back(byte);
        auto searchBegin = buffer.data() + offset;
        auto searchEnd = buffer.data() + buffer.size();
        auto searchIt = std::search(searchBegin, searchEnd, begin(delim), end(delim));
        if (searchIt != searchEnd) {
            cont = false;
        }
    }
    return ReadStatus::Complete;
}

ReadStatus mocca::net::readExactly(IStreamConnection& stream, std::vector<uint8_t>& buffer, uint32_t size, std::chrono::milliseconds timeout) {
    uint32_t bytesRead = 0;
    std::vector<uint8_t> chunk(size);
    while (bytesRead < size) {
        auto bytesReceived = stream.receive(chunk.data(), size - bytesRead, timeout);
        if (bytesReceived == 0) {
            return ReadStatus::Incomplete;
        }
        buffer.insert(end(buffer), begin(chunk), begin(chunk) + bytesReceived);
        bytesRead += bytesReceived;
        chunk.clear();
    }
    return ReadStatus::Complete;
}