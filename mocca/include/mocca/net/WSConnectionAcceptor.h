#pragma once

#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/IPhysicalConnectionAcceptor.h"

namespace mocca {
namespace net {

class WSConnectionAcceptor : public IProtocolConnectionAcceptor {
public:
    WSConnectionAcceptor(std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor);
    std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor_;
};
}
}