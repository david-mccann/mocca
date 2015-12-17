#include "mocca/net/message/LoopbackConnectionFactory.h"

#include "mocca/net/message/LoopbackConnection.h"
#include "mocca/net/message/LoopbackConnectionAcceptor.h"

#include "mocca/net/NetworkError.h"

using namespace mocca::net;

std::unique_ptr<IMessageConnection> LoopbackConnectionFactory::connect(const std::string& queueName) {
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection acceptor bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<LoopbackConnection::MQ>();
    auto messageQueue2 = std::make_shared<LoopbackConnection::MQ>();

    auto signalQueue1 = std::make_shared<LoopbackConnection::SQ>();
    auto signalQueue2 = std::make_shared<LoopbackConnection::SQ>();

    auto serverConnection = std::unique_ptr<LoopbackConnection>(new LoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection =
        std::unique_ptr<IMessageConnection>(new LoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<IMessageConnectionAcceptor> LoopbackConnectionFactory::bind(const std::string& queueName) {
    auto queue = std::make_shared<typename LoopbackConnectionAcceptor::LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<IMessageConnectionAcceptor>(new LoopbackConnectionAcceptor(queue));
}