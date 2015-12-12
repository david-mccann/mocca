#pragma once

#include "mocca/net/IMessageConnection.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingConnection : public IMessageConnection {
public:
    FramingConnection(std::unique_ptr<IStreamConnection> stream, std::unique_ptr<FramingStrategy> framingStrategy);

    std::string identifier() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<IStreamConnection> stream_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
};
}
}
