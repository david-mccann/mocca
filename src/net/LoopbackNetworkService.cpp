#include "mocca/base/ByteArray.h"
#include "mocca/base/MessageQueue.h"
#include "mocca/net/LoopbackNetworkService.h"
#include "mocca/net/LoopbackConnection.h"
#include "mocca/net/LoopbackConnectionListener.h"
#include "mocca/net/TCPNetworkAddress.h"
#include "mocca/net/Error.h"

namespace mocca {
namespace net {

std::unique_ptr<AbstractConnection> LoopbackNetworkService::connect(const std::string& queueName) {
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection listener bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<LoopbackConnection::LoopbackMessageQueue>();
    auto messageQueue2 = std::make_shared<LoopbackConnection::LoopbackMessageQueue>();

    auto signalQueue1 = std::make_shared<LoopbackConnection::LoopbackSignalQueue>();
    auto signalQueue2 = std::make_shared<LoopbackConnection::LoopbackSignalQueue>();

    auto serverConnection = std::unique_ptr<LoopbackConnection>(
        new LoopbackConnection(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection = std::unique_ptr<AbstractConnection>(
        new LoopbackConnection(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<IConnectionListener> LoopbackNetworkService::bind(const std::string& queueName) {
    auto queue = std::make_shared<LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<IConnectionListener>(new LoopbackConnectionListener(queue));
}
}
}