#pragma once

#include "mocca/net/stream/MessageQueueStream.h"
#include "mocca/net/stream/MessageQueueStreamAcceptor.h"
#include "mocca/net/stream/StreamObjectFactory.h"

#include <map>

namespace mocca {
namespace net {
class MessageQueueObjectFactory : public StreamObjectFactory<MessageQueueObjectFactory, MessageQueueStream, MessageQueueStreamAcceptor> {
    friend class StreamObjectFactory<MessageQueueObjectFactory, MessageQueueStream, MessageQueueStreamAcceptor>;

private:
    std::unique_ptr<MessageQueueStream> createStreamImpl(const std::string& queueName);
    std::unique_ptr<MessageQueueStreamAcceptor> createAcceptorImpl(const std::string& queueName);

    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<MessageQueueStream>>;
    std::map<std::string, std::shared_ptr<LoopbackConnectionQueue>> spawnedConnections_;
};
}
}
