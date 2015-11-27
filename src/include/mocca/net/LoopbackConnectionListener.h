#pragma once

#include "mocca/net/IPhysicalConnectionAcceptor.h"
#include "mocca/net/LoopbackConnection.h"

namespace mocca {
namespace net {

class LoopbackNetworkService;

class LoopbackConnectionListener : public IPhysicalConnectionAcceptor {
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;
public:
    LoopbackConnectionListener(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
    virtual std::unique_ptr<IPhysicalConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}