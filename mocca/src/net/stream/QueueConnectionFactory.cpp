#include "mocca/net/stream/QueueConnectionFactory.h"

#include "mocca/net/stream/QueueConnection.h"
#include "mocca/net/stream/QueueConnectionAcceptor.h"
#include "mocca/net/NetworkError.h"

using namespace mocca::net;


std::unique_ptr<IStreamConnection> QueueConnectionFactory::connect(const std::string& queueName) {
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection acceptor bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<QueueConnection::MQ>();
    auto messageQueue2 = std::make_shared<QueueConnection::MQ>();

    auto signalQueue1 = std::make_shared<QueueConnection::SQ>();
    auto signalQueue2 = std::make_shared<QueueConnection::SQ>();

    auto serverConnection = std::unique_ptr<QueueConnection>(new QueueConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection =
        std::unique_ptr<IStreamConnection>(new QueueConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<IStreamConnectionAcceptor> QueueConnectionFactory::bind(const std::string& queueName) {
    auto queue = std::make_shared<typename QueueConnectionAcceptor::LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<IStreamConnectionAcceptor>(new QueueConnectionAcceptor(queue));
}