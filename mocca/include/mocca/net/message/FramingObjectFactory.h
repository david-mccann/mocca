#pragma once

#include "mocca/net/message/FramingConnection.h"
#include "mocca/net/message/FramingConnectionAcceptor.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/stream/TCPObjectFactory.h"
#include "mocca/net/stream/MessageQueueObjectFactory.h"
#include "mocca/net/framing/SizePrefixedProtocol.h"

namespace mocca {
namespace net {

template <typename ProtocolType, typename StreamObjectFactoryType> class FramingObjectFactory {
public:
    using Stream_Object_Factory_Type = StreamObjectFactoryType;

    std::unique_ptr<IMessageConnection> connect(const std::string& args);
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& args);

private:
    StreamObjectFactoryType streamObjectFactory_;
};

using Prefixed_TCP_Factory = FramingObjectFactory<SizePrefixedProtocol<TCPStream>, TCPObjectFactory>;
using Prefixed_MessageQueue_Factory = FramingObjectFactory<SizePrefixedProtocol<MessageQueueStream>, MessageQueueObjectFactory>;


}
}
