#include "mocca/net/stream/MessageQueueObjectFactory.h"

#include "mocca/net/Error.h"

using namespace mocca::net;

std::unique_ptr<MessageQueueStream> MessageQueueObjectFactory::createStreamImpl(const std::string & queueName)
{
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection acceptor bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<MessageQueueStream::LoopbackMessageQueue>();
    auto messageQueue2 = std::make_shared<MessageQueueStream::LoopbackMessageQueue>();

    auto signalQueue1 = std::make_shared<MessageQueueStream::LoopbackSignalQueue>();
    auto signalQueue2 = std::make_shared<MessageQueueStream::LoopbackSignalQueue>();

    auto serverConnection = std::unique_ptr<MessageQueueStream>(
        new MessageQueueStream(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection =
        std::unique_ptr<MessageQueueStream>(new MessageQueueStream(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<MessageQueueStreamAcceptor> MessageQueueObjectFactory::createAcceptorImpl(const std::string & queueName)
{
    auto queue = std::make_shared<LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<MessageQueueStreamAcceptor>(new MessageQueueStreamAcceptor(queue));
}