#pragma once

#include "mocca/net/IPhysicalConnectionAcceptor.h"
#include "mocca/testing/LoopbackPhysicalConnection.h"

namespace mocca {
namespace net {

class LoopbackPhysicalConnectionAcceptor : public IPhysicalConnectionAcceptor {
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackPhysicalConnection>>;

public:
    LoopbackPhysicalConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
    virtual std::unique_ptr<IPhysicalConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}