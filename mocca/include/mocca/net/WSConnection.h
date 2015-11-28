#pragma once

#include "mocca/net/IPhysicalConnection.h"
#include "mocca/net/IProtocolConnection.h"

namespace mocca {
namespace net {

class WSConnection : public IProtocolConnection {
public:
    WSConnection(std::unique_ptr<IPhysicalConnection> physicalConnection);

    std::string identifier() const override;
    void send(ByteArray message) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<IPhysicalConnection> physicalConnection_;
};
}
}