#include "mocca/net/message/StreamFramingConnectionAcceptor.h"

#include "mocca/net/framing/SizePrefixedProtocol.h"
#include "mocca/net/message/StreamFramingConnection.h"
#include "mocca/net/stream/TCPConnectionAcceptor.h"

using namespace mocca::net;

template <typename FramingProtocolType, typename StreamAcceptorType>
std::unique_ptr<IMessageConnection>
StreamFramingConnectionAcceptor<FramingProtocolType, StreamAcceptorType>::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    FramingProtocolType::performHandshake(*stream, timeout);
    return std::unique_ptr<IMessageConnection>(
        new StreamFramingConnection<FramingProtocolType, typename StreamAcceptorType::Stream_Type>(std::move(stream)));
}


template class StreamFramingConnectionAcceptor<SizePrefixedProtocol<TCPConnection>, TCPConnectionAcceptor>;