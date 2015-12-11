#include "mocca/net/message/StreamFramingConnection.h"

#include "mocca/net/framing//SizePrefixedProtocol.h"
#include "mocca/net/stream/TCPConnection.h"
#include "mocca/net/stream/MessageQueueStream.h"

using namespace mocca;
using namespace mocca::net;

template<typename FramingProtocolType>
inline std::string StreamFramingConnection<FramingProtocolType>::identifier() const
{
    return std::string(); // fixme: todo
}

template<typename FramingProtocolType>
void StreamFramingConnection<FramingProtocolType>::send(ByteArray message, std::chrono::milliseconds timeout) const
{
    framingProtocol_.writeFrameToStream(std::move(message), timeout);
}

template<typename FramingProtocolType>
ByteArray StreamFramingConnection<FramingProtocolType>::receive(std::chrono::milliseconds timeout) const
{
    return framingProtocol_.readFrameFromStream(timeout);
}

template class StreamFramingConnection<SizePrefixedProtocol<TCPConnection>>;
template class StreamFramingConnection<SizePrefixedProtocol<MessageQueueStream>>;
