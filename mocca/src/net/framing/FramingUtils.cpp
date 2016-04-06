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

ReadStatus mocca::net::readUntil(IStreamConnection& stream, ByteArray& buffer, const std::string& delim, std::chrono::milliseconds timeout,
                                 uint32_t chunkSize) {
    bool cont = true;
    std::vector<uint8_t> chunk(chunkSize);
    while (cont) {
        auto bytesReceived = stream.receive(chunk.data(), chunkSize, timeout);
        if (bytesReceived == 0) {
            return ReadStatus::Incomplete;
        }
        auto offset = buffer.size() - std::min(static_cast<uint32_t>(delim.size()), buffer.size());
        buffer.append(chunk.data(), bytesReceived);
        auto searchBegin = buffer.data() + offset;
        auto searchEnd = buffer.data() + buffer.size();
        auto searchIt = std::search(searchBegin, searchEnd, begin(delim), end(delim));
        if (searchIt != searchEnd) {
            buffer.setSize(static_cast<uint32_t>(std::distance(buffer.data(), searchIt) + delim.size()));
            cont = false;
        }
        chunk.clear();
    }
    return ReadStatus::Complete;
}

ReadStatus mocca::net::readExactly(IStreamConnection& stream, ByteArray& buffer, uint32_t size, std::chrono::milliseconds timeout) {
    uint32_t bytesRead = 0;
    std::vector<uint8_t> chunk(size);
    while (bytesRead < size) {
        auto bytesReceived = stream.receive(chunk.data(), size - bytesRead, timeout);
        if (bytesReceived == 0) {
            return ReadStatus::Incomplete;
        }
        buffer.append(chunk.data(), bytesReceived);
        bytesRead += bytesReceived;
        chunk.clear();
    }
    return ReadStatus::Complete;
}