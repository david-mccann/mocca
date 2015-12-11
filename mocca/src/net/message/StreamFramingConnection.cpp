#include "mocca/net/message/StreamFramingConnection.h"

#include "mocca/net/framing//SizePrefixedProtocol.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

template <typename FramingProtocolType, typename StreamType>
inline std::string StreamFramingConnection<FramingProtocolType, StreamType>::identifier() const
{
    return std::string(); // fixme: todo
}

template <typename FramingProtocolType, typename StreamType>
void StreamFramingConnection<FramingProtocolType, StreamType>::send(ByteArray message, std::chrono::milliseconds timeout) const
{
    FramingProtocolType::writeFrameToStream(*stream_, std::move(message), timeout);
}

template <typename FramingProtocolType, typename StreamType>
ByteArray StreamFramingConnection<FramingProtocolType, StreamType>::receive(std::chrono::milliseconds timeout) const
{
    return FramingProtocolType::readFrameFromStream(*stream_, timeout);
}

template class StreamFramingConnection<SizePrefixedProtocol<TCPConnection>, TCPConnection>;
template class StreamFramingConnection<SizePrefixedProtocol<MessageQueueStream>, MessageQueueStream>;
