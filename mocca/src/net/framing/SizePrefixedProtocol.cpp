#include "mocca/net/framing/SizePrefixedProtocol.h"

#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

template <typename IOStreamType>
ByteArray SizePrefixedProtocol<IOStreamType>::readFrameFromStreamImpl(std::chrono::milliseconds timeout) const {
    std::lock_guard<IOStreamType> lock(*ioStream_);
    auto sizeData = readExactly(*ioStream_, sizeof(uint32_t), timeout);
    if (sizeData.isEmpty()) {
        return ByteArray();
    }
    auto frameSize = sizeData.read<uint32_t>();
    auto frame = readExactly(*ioStream_, frameSize, timeout);
    return frame;
}

template <typename IOStreamType>
void SizePrefixedProtocol<IOStreamType>::writeFrameToStreamImpl(ByteArray frame, std::chrono::milliseconds timeout) const {
    // fixme: performance loss; implement prepend method for ByteArray
    ByteArray newFrame(frame.size() + sizeof(uint32_t));
    newFrame << frame.size();
    newFrame.append(frame);
    ioStream_->write(std::move(newFrame));
}

template class SizePrefixedProtocol<TCPConnection>;
template class SizePrefixedProtocol<MessageQueueStream>;