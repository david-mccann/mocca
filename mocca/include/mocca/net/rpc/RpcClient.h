#pragma once

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"
#include "mocca/net/rpc/MethodDescription.h"

#include "jsoncpp/json.h"

#include <chrono>

namespace mocca {
namespace net {
class RpcClient {
public:
    using ReturnType = std::pair<JsonCpp::Value, std::vector<mocca::net::MessagePart>>;

    RpcClient(const mocca::net::Endpoint& endpoint);

    std::vector<MethodDescription> describe() const;
    void send(const std::string& method, const JsonCpp::Value& params) const;
    ReturnType receive() const;
    bool isConnected() const;

private:
    std::unique_ptr<mocca::net::IMessageConnection> connection_;
};
}
}
