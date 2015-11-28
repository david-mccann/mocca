#include "mocca/net/PhysicalConnection.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

ByteArray mocca::net::receiveUntil(IPhysicalConnection& connection, const std::string& delim, std::chrono::milliseconds timeout,
                                   uint32_t chunkSize) {
    ByteArray result;
    bool cont = true;
    while (cont) {
        auto chunk = connection.receive(chunkSize, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        auto offset = result.size() - std::min(delim.size(), result.size());
        result.append(chunk.data(), chunk.size());
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

ByteArray mocca::net::receiveExactly(IPhysicalConnection& connection, uint32_t size, std::chrono::milliseconds timeout) {
    ByteArray result(size);
    while (result.size() < size) {
        auto chunk = connection.receive(size, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        result.append(chunk.data(), chunk.size());
    }
    return result;
}