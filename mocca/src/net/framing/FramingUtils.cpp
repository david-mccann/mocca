#include "mocca/net/framing/FramingUtils.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

ReadStatus mocca::net::readUntil(IStreamConnection& stream, ByteArray& buffer, const std::string& delim, std::chrono::milliseconds timeout,
                                 uint32_t chunkSize) {
    bool cont = true;
    while (cont) {
        auto chunk = stream.receive(chunkSize, timeout);
        if (chunk.isEmpty()) {
            return ReadStatus::Incomplete;
        }
        auto offset = buffer.size() - std::min(static_cast<uint32_t>(delim.size()), buffer.size());
        buffer.append(chunk);
        auto searchBegin = buffer.data() + offset;
        auto searchEnd = buffer.data() + buffer.size();
        auto searchIt = std::search(searchBegin, searchEnd, begin(delim), end(delim));
        if (searchIt != searchEnd) {
            buffer.setSize(std::distance(buffer.data(), searchIt) + delim.size());
            cont = false;
        }
    }
    return ReadStatus::Complete;
}

ReadStatus mocca::net::readExactly(IStreamConnection& stream, ByteArray& buffer, uint32_t size, std::chrono::milliseconds timeout) {
    uint32_t bytesRead = 0;
    while (bytesRead < size) {
        auto chunk = stream.receive(size - bytesRead, timeout);
        bytesRead += chunk.size();
        if (chunk.isEmpty()) {
            return ReadStatus::Incomplete;
        }
        buffer.append(chunk);
    }
    return ReadStatus::Complete;
}