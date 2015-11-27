#pragma once

#include "mocca/net/IPhysicalConnectionAcceptor.h"
#include "mocca/net/LoopbackConnection_tmp.h"

namespace mocca {
namespace net {

class LoopbackConnectionAcceptor_tmp : public IPhysicalConnectionAcceptor {
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection_tmp>>;

public:
    LoopbackConnectionAcceptor_tmp(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
    virtual std::unique_ptr<IPhysicalConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}