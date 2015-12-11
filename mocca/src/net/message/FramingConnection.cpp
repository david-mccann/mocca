#include "mocca/net/message/FramingConnection.h"

#include "mocca/net/framing//SizePrefixedProtocol.h"
#include "mocca/net/stream/MessageQueueStream.h"
#include "mocca/net/stream/TCPStream.h"

using namespace mocca;
using namespace mocca::net;

template <typename ProtocolType, typename StreamType> inline std::string FramingConnection<ProtocolType, StreamType>::identifier() const {
    return stream_->identifier();
}

template <typename ProtocolType, typename StreamType>
void FramingConnection<ProtocolType, StreamType>::send(ByteArray message, std::chrono::milliseconds timeout) const {
    ProtocolType::writeFrameToStream(*stream_, std::move(message), timeout);
}

template <typename ProtocolType, typename StreamType>
ByteArray FramingConnection<ProtocolType, StreamType>::receive(std::chrono::milliseconds timeout) const {
    return ProtocolType::readFrameFromStream(*stream_, timeout);
}

template class FramingConnection<SizePrefixedProtocol<TCPStream>, TCPStream>;
template class FramingConnection<SizePrefixedProtocol<MessageQueueStream>, MessageQueueStream>;
