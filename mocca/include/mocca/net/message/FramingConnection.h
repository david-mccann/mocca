#pragma once

#include "mocca/net/IMessageConnection.h"

namespace mocca {
namespace net {

template <typename ProtocolType, typename StreamType> class FramingConnection : public IMessageConnection {
public:
    FramingConnection(std::unique_ptr<StreamType> stream) : stream_(std::move(stream)) {}

    std::string identifier() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<StreamType> stream_;
};
}
}
