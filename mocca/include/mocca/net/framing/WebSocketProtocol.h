#pragma once

#include "mocca/net/framing/FramingStrategy.h"

#include <unordered_map>

namespace mocca {
namespace net {
class WebSocketProtocol : public FramingStrategy {
public:
    struct ConnectionInfo {
        std::string method;
        std::string path;
        std::string httpVersion;
        std::unordered_map<std::string, std::string> header;
    };

    std::unique_ptr<FramingStrategy> clone() const override;
    ByteArray readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) override;
    void writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) override;
    void performHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout);

private:
    void receiveHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout);
    void sendHandshakeResponse(IStreamConnection& connection, std::chrono::milliseconds timeout);

private:
    ConnectionInfo connectionInfo_;
};
}
}
