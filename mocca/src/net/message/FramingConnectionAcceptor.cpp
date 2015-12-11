#include "mocca/net/message/FramingConnectionAcceptor.h"

#include "mocca/net/framing/SizePrefixedProtocol.h"
#include "mocca/net/message/FramingConnection.h"
#include "mocca/net/stream/TCPStreamAcceptor.h"
#include "mocca/net/stream/MessageQueueStreamAcceptor.h"

using namespace mocca::net;

template <typename ProtocolType, typename StreamAcceptorType>
std::unique_ptr<IMessageConnection>
FramingConnectionAcceptor<ProtocolType, StreamAcceptorType>::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    if (stream != nullptr) {
        ProtocolType::performHandshake(*stream, timeout);
        return std::unique_ptr<IMessageConnection>(
            new FramingConnection<ProtocolType, typename StreamAcceptorType::Stream_Type>(std::move(stream)));
    }
    return nullptr;
}


template class FramingConnectionAcceptor<SizePrefixedProtocol<TCPStream>, TCPStreamAcceptor>;
template class FramingConnectionAcceptor<SizePrefixedProtocol<MessageQueueStream>, MessageQueueStreamAcceptor>;