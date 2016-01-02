#pragma once

#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/message/LoopbackConnectionSpawner.h"

namespace mocca {
namespace net {

class LoopbackConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    LoopbackConnectionAcceptor(std::shared_ptr<LoopbackConnectionSpawner> spawner);
    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<LoopbackConnectionSpawner> spawner_;
};
}
}