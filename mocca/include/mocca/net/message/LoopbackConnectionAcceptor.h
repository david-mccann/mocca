#pragma once

#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/message/LoopbackConnection.h"

namespace mocca {
namespace net {

class LoopbackConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    using LoopbackConnectionQueue = MessageQueue<std::unique_ptr<LoopbackConnection>>;

    LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionQueue> connectionQueue);

    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionQueue> connectionQueue_;
};
}
}