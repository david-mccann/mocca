/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

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