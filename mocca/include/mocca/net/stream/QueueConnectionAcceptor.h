#pragma once

#include "mocca/net/IStreamConnectionAcceptor.h"
#include "mocca/net/stream/QueueConnection.h"

namespace mocca {
namespace net {

class QueueConnectionAcceptor : public IStreamConnectionAcceptor {
public:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<QueueConnection>>;

    QueueConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);
    
    std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}