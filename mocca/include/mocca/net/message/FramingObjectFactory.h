#pragma once

#include "mocca/net/message/FramingConnection.h"
#include "mocca/net/message/FramingConnectionAcceptor.h"
#include "mocca/net/message/IMessageConnection.h"
#include "mocca/net/message/IMessageConnectionAcceptor.h"
#include "mocca/net/stream/TCPObjectFactory.h"
#include "mocca/net/stream/MessageQueueObjectFactory.h"
#include "mocca/net/framing/SizePrefixedProtocol.h"

namespace mocca {
namespace net {

template <typename ProtocolType, typename StreamObjectFactoryType> class FramingObjectFactory {
public:
    using Stream_Object_Factory_Type = StreamObjectFactoryType;

    std::unique_ptr<IMessageConnection> connect(const std::string& args) {
        return std::unique_ptr<IMessageConnection>(new FramingConnection<ProtocolType, typename StreamObjectFactoryType::Stream_Type>(
            streamObjectFactory_.createStream(args)));
    }

    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& args) {
        return std::unique_ptr<IMessageConnectionAcceptor>(
            new FramingConnectionAcceptor<ProtocolType, typename StreamObjectFactoryType::Acceptor_Type>(
                streamObjectFactory_.createAcceptor(args)));
    }

private:
    StreamObjectFactoryType streamObjectFactory_;
};

using Prefixed_TCP_Factory = FramingObjectFactory<SizePrefixedProtocol<TCPStream>, TCPObjectFactory>;
using Prefixed_MessageQueue_Factory = FramingObjectFactory<SizePrefixedProtocol<MessageQueueStream>, MessageQueueObjectFactory>;


}
}
