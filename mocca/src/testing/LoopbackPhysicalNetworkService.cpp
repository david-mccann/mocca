#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"
#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"
#include "mocca/testing/LoopbackPhysicalNetworkService.h"

namespace mocca {
namespace net {

std::string mocca::net::LoopbackPhysicalNetworkService::protocol() {
    return "loopback";
}

std::unique_ptr<LoopbackPhysicalConnection> LoopbackPhysicalNetworkService::connect(const std::string& queueName) {
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection acceptor bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<LoopbackPhysicalConnection::LoopbackMessageQueue>();
    auto messageQueue2 = std::make_shared<LoopbackPhysicalConnection::LoopbackMessageQueue>();

    auto signalQueue1 = std::make_shared<LoopbackPhysicalConnection::LoopbackSignalQueue>();
    auto signalQueue2 = std::make_shared<LoopbackPhysicalConnection::LoopbackSignalQueue>();

    auto serverConnection = std::unique_ptr<LoopbackPhysicalConnection>(
        new LoopbackPhysicalConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection = std::unique_ptr<LoopbackPhysicalConnection>(
        new LoopbackPhysicalConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<LoopbackPhysicalConnectionAcceptor> LoopbackPhysicalNetworkService::bind(const std::string& queueName) {
    auto queue = std::make_shared<LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<LoopbackPhysicalConnectionAcceptor>(new LoopbackPhysicalConnectionAcceptor(queue));
}
}
}