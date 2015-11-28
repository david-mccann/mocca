#pragma once

#include "mocca/net/IProtocolConnection.h"
#include "mocca/net/IPhysicalConnection.h"

namespace mocca {
namespace net {
class MoccaConnection : public IProtocolConnection {
public:
    MoccaConnection(std::unique_ptr<IPhysicalConnection> physicalConnection);

    std::string identifier() const override;
    void send(ByteArray message) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<IPhysicalConnection> physicalConnection_;
};
}
}
