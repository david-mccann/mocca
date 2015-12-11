#include "mocca/net/framing/FramingUtils.h"

#include "mocca/net/stream/MessageQueueStream.h"
#include "mocca/net/stream/TCPStream.h"

#include <algorithm>

using namespace mocca;
using namespace mocca::net;

template <typename StreamType>
ByteArray mocca::net::readUntil(StreamType& stream, const std::string& delim, std::chrono::milliseconds timeout, uint32_t chunkSize) {
    ByteArray result;
    bool cont = true;
    while (cont) {
        auto chunk = stream.read(chunkSize, timeout);
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

template <typename StreamType> ByteArray mocca::net::readExactly(StreamType& stream, uint32_t size, std::chrono::milliseconds timeout) {
    ByteArray result(size);
    while (result.size() < size) {
        auto chunk = stream.read(size, timeout);
        if (chunk.isEmpty()) {
            return ByteArray(); // fixme: if no data is returned, the queue should not be modified
        }
        result.append(chunk);
    }
    return result;
}

template ByteArray mocca::net::readUntil<TCPStream>(TCPStream& stream, const std::string& delim, std::chrono::milliseconds timeout,
                                                        uint32_t chunkSize);
template ByteArray mocca::net::readUntil<MessageQueueStream>(MessageQueueStream& stream, const std::string& delim,
                                                             std::chrono::milliseconds timeout, uint32_t chunkSize);

template ByteArray mocca::net::readExactly<TCPStream>(TCPStream& stream, uint32_t size, std::chrono::milliseconds timeout);
template ByteArray mocca::net::readExactly<MessageQueueStream>(MessageQueueStream& stream, uint32_t size,
                                                               std::chrono::milliseconds timeout);
