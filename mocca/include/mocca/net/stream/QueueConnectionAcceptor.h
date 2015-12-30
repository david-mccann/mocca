#pragma once

#include "mocca/net/IStreamConnectionAcceptor.h"
#include "mocca/net/stream/QueueConnectionSpawner.h"

namespace mocca {
namespace net {

class QueueConnectionAcceptor : public IStreamConnectionAcceptor {
public:
    QueueConnectionAcceptor(std::shared_ptr<QueueConnectionSpawner> spawner);
    std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout) override;

private:
    std::shared_ptr<QueueConnectionSpawner> spawner_;
};
}
}