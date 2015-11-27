#pragma once

#include "mocca/net/IProtocolConnectionAcceptor.h"
#include "mocca/net/IPhysicalConnectionAcceptor.h"


namespace mocca {
namespace net {
class MoccaConnectionAcceptor : public IProtocolConnectionAcceptor {
public:
    MoccaConnectionAcceptor(std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor);
    std::unique_ptr<IProtocolConnection> getConnection(std::chrono::milliseconds timeout) override;

private:
    std::unique_ptr<IPhysicalConnectionAcceptor> physicalConnectionAcceptor_;
};
}
}