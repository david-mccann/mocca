#include "mocca/base/MessageQueue.h"
#include "mocca/net/Error.h"
#include "mocca/net/LoopbackConnection_tmp.h"
#include "mocca/net/LoopbackConnectionAcceptor_tmp.h"
#include "mocca/net/LoopbackNetworkService_tmp.h"

namespace mocca {
namespace net {

std::unique_ptr<IPhysicalConnection> LoopbackNetworkService_tmp::connect(const std::string& queueName) {
    if (!spawnedConnections_.count(queueName)) {
        throw NetworkError("No connection acceptor bound to queue " + queueName, __FILE__, __LINE__);
    }

    auto messageQueue1 = std::make_shared<LoopbackConnection_tmp::LoopbackMessageQueue>();
    auto messageQueue2 = std::make_shared<LoopbackConnection_tmp::LoopbackMessageQueue>();

    auto signalQueue1 = std::make_shared<LoopbackConnection_tmp::LoopbackSignalQueue>();
    auto signalQueue2 = std::make_shared<LoopbackConnection_tmp::LoopbackSignalQueue>();

    auto serverConnection =
        std::unique_ptr<LoopbackConnection_tmp>(new LoopbackConnection_tmp(messageQueue1, messageQueue2, signalQueue1, signalQueue2));
    auto clientConnection =
        std::unique_ptr<IPhysicalConnection>(new LoopbackConnection_tmp(messageQueue2, messageQueue1, signalQueue2, signalQueue1));

    spawnedConnections_[queueName]->enqueue(std::move(serverConnection));

    return clientConnection;
}

std::unique_ptr<IPhysicalConnectionAcceptor> LoopbackNetworkService_tmp::bind(const std::string& queueName) {
    auto queue = std::make_shared<LoopbackConnectionQueue>();
    spawnedConnections_[queueName] = queue;
    return std::unique_ptr<IPhysicalConnectionAcceptor>(new LoopbackConnectionAcceptor_tmp(queue));
}
}
}