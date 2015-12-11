#pragma once

#include "mocca/net/message/IMessageConnection.h"

namespace mocca {
namespace net {

template <typename FramingProtocolType, typename StreamType> class StreamFramingConnection : public IMessageConnection {
public:
    StreamFramingConnection(std::unique_ptr<StreamType> stream) : stream_(std::move(stream)) {}

    std::string identifier() const override;
    void send(ByteArray message, std::chrono::milliseconds timeout) const override;
    ByteArray receive(std::chrono::milliseconds timeout) const override;

private:
    std::unique_ptr<StreamType> stream_;
};
}
}
