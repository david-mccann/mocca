#pragma once

#include "mocca/net/message/IMessageConnection.h"

namespace mocca {
namespace net {

template <typename FramingProtocolType> class StreamFramingConnection : public IMessageConnection {
public:
    std::string identifier() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    FramingProtocolType framingProtocol_;
};
}
}
