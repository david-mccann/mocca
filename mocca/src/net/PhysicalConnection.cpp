#include "mocca/net/PhysicalConnection.h"

#include "mocca/net/TCPConnection.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

template <typename PhysicalConnectionType>
ByteArray mocca::net::receiveUntil(PhysicalConnectionType& connection, const std::string& delim, std::chrono::milliseconds timeout,
                                   uint32_t chunkSize) {
    ByteArray result;
    bool cont = true;
    while (cont) {
        auto chunk = connection.receive(chunkSize, timeout);
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

template <typename PhysicalConnectionType>
ByteArray mocca::net::receiveExactly(PhysicalConnectionType& connection, uint32_t size, std::chrono::milliseconds timeout) {
    ByteArray result(size);
    while (result.size() < size) {
        auto chunk = connection.receive(size, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        result.append(chunk);
    }
    return result;
}

template ByteArray mocca::net::receiveUntil<TCPConnection>(TCPConnection& connection, const std::string& delim,
                                                           std::chrono::milliseconds timeout, uint32_t chunkSize);
template ByteArray mocca::net::receiveUntil<LoopbackPhysicalConnection>(LoopbackPhysicalConnection& connection, const std::string& delim,
                                                                        std::chrono::milliseconds timeout, uint32_t chunkSize);

template ByteArray mocca::net::receiveExactly<TCPConnection>(TCPConnection& connection, uint32_t size, std::chrono::milliseconds timeout);
template ByteArray mocca::net::receiveExactly<LoopbackPhysicalConnection>(LoopbackPhysicalConnection& connection, uint32_t size,
                                                                          std::chrono::milliseconds timeout);
