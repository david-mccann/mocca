#include "mocca/net/framing/FramingUtils.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

ByteArray mocca::net::readUntil(IStreamConnection& stream, const std::string& delim, std::chrono::milliseconds timeout,
                                uint32_t chunkSize) {
    ByteArray result;
    bool cont = true;
    while (cont) {
        auto chunk = stream.receive(chunkSize, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        auto offset = result.size() - std::min(static_cast<uint32_t>(delim.size()), result.size());
        result.append(chunk);
        auto searchBegin = result.data() + offset;
        auto searchEnd = result.data() + result.size();
        auto searchIt = std::search(searchBegin, searchEnd, begin(delim), end(delim));
        if (searchIt != searchEnd) {
            result.setSize(std::distance(result.data(), searchIt) + delim.size());
            cont = false;
        }
    }
    return result;
}

ByteArray mocca::net::readExactly(IStreamConnection& stream, uint32_t size, std::chrono::milliseconds timeout) {
    ByteArray result(size);
    while (result.size() < size) {
        auto chunk = stream.receive(size, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        result.append(chunk);
    }
    return result;
}