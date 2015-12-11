#include "mocca/net/framing/SizePrefixedProtocol.h"

#include "mocca/net/stream/TCPStream.h"
#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

template <typename StreamType>
ByteArray SizePrefixedProtocol<StreamType>::readFrameFromStreamImpl(StreamType& stream, std::chrono::milliseconds timeout) {
    std::lock_guard<StreamType> lock(stream);
    auto sizeData = readExactly(stream, sizeof(uint32_t), timeout);
    if (sizeData.isEmpty()) {
        return ByteArray();
    }
    auto frameSize = sizeData.read<uint32_t>();
    auto frame = readExactly(stream, frameSize, timeout);
    return frame;
}

template <typename StreamType>
void SizePrefixedProtocol<StreamType>::writeFrameToStreamImpl(StreamType& stream, ByteArray frame, std::chrono::milliseconds timeout) {
    // fixme: performance loss; implement prepend method for ByteArray
    ByteArray newFrame(frame.size() + sizeof(uint32_t));
    newFrame << frame.size();
    newFrame.append(frame);
    stream.write(std::move(newFrame));
}

template<typename StreamType>
void SizePrefixedProtocol<StreamType>::performHandshakeImpl(StreamType & stream, std::chrono::milliseconds timeout)
{
    // nothing to do
}

template class SizePrefixedProtocol<TCPStream>;
template class SizePrefixedProtocol<MessageQueueStream>;