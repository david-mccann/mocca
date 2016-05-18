#include "mocca/net/rpc/RpcClient.h"

#include "mocca/base/Error.h"
#include "mocca/net/rpc/JsonKeys.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace mocca::net;

RpcClient::RpcClient(const mocca::net::Endpoint& endpoint)
    : connection_(mocca::net::ConnectionFactorySelector::connect(endpoint)) {}

void RpcClient::send(const std::string& method, const JsonCpp::Value& params) const {
    JsonCpp::Value request;
    request[mocca::net::methodKey()] = method;
    request[mocca::net::paramsKey()] = params;
    JsonCpp::FastWriter writer;
    std::string str = writer.write(request);
    mocca::net::Message message{ mocca::net::createMessagePart(str) };
    connection_->send(message);
}

RpcClient::ReturnType RpcClient::receive() const {
    auto message = connection_->receive();
    std::string json = mocca::net::readMessagePart(*message[0]);
    JsonCpp::Value root;
    JsonCpp::Reader reader;
    if (!reader.parse(json, root)) {
        throw Error("Invalid JSON returned from server: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    } else if (root[mocca::net::statusKey()].asString() == mocca::net::errorStatus()) {
        throw Error("Server returned error: " + root[mocca::net::errorKey()].asString(), __FILE__, __LINE__);
    }
    auto binary = std::vector<mocca::net::MessagePart>(begin(message) + 1, end(message));
    return std::make_pair(root[mocca::net::resultKey()], binary);
}

bool RpcClient::isConnected() const {
    return connection_->isConnected();
}

std::vector<MethodDescription> RpcClient::describe() const {
    send(mocca::net::describe(), JsonCpp::Value());
    std::vector<MethodDescription> descriptions;
    auto result = receive().first;
    for (auto it = result.begin(); it != result.end(); ++it) {
        descriptions.push_back(MethodDescription::fromJson(*it));
    }
    return descriptions;
}