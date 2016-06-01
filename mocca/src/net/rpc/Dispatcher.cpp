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

#include "mocca/net/rpc/Dispatcher.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/Error.h"
#include "mocca/base/Memory.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/rpc/JsonKeys.h"

using namespace mocca::net;

Dispatcher::Dispatcher(const std::vector<mocca::net::Endpoint>& endpoints) {
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors;
    for (const auto& ep : endpoints) {
        acceptors.push_back(mocca::net::ConnectionFactorySelector::bind(ep));
    }
    aggregator_ = mocca::make_unique<mocca::net::ConnectionAggregator>(std::move(acceptors));
    registerReflection();
}

Dispatcher::~Dispatcher() {
    join();
}

void Dispatcher::sendReply(const JsonCpp::Value root, const std::vector<mocca::net::MessagePart>& binary,
                           std::shared_ptr<const mocca::net::ConnectionID> connectionID) {
    JsonCpp::FastWriter writer;
    std::string jsonStr = writer.write(root);
    mocca::net::Message message;
    message.push_back(mocca::net::createMessagePart(jsonStr));
    message.insert(end(message), begin(binary), end(binary));
    mocca::net::MessageEnvelope envelope(std::move(message), connectionID);
    aggregator_->send(std::move(envelope));
}

void Dispatcher::run() {
    const auto timeout(std::chrono::milliseconds(100));

    while (!isInterrupted()) {
        auto envelopeNullable = aggregator_->receive(timeout);
        if (!envelopeNullable.isNull()) {
            auto envelope = envelopeNullable.release();
            auto message = std::move(envelope.message);
            try {
                JsonCpp::Value root = parseMessage(message);

                // sanity checks on request
                if (!root.isMember(mocca::net::methodKey())) {
                    throw Error("Malformatted request: Required field 'method' or 'params' is missing", __FILE__, __LINE__);
                }

                // find matching method for request
                std::string methodName = root[mocca::net::methodKey()].asString();
                auto findIt = mocca::findMemberEqual(begin(methods_), end(methods_), &Method::name, methodName);
                if (findIt == end(methods_)) {
                    throw Error("Unknown method '" + methodName + "'", __FILE__, __LINE__);
                }

                // call method
                const auto& method = *findIt;
                auto result = method(root[mocca::net::paramsKey()]);
                const auto& json = result.first;
                const auto& binary = result.second;

                // send reply if method returned something
                JsonCpp::Value reply;
                reply[mocca::net::resultKey()] = json;
                reply[mocca::net::statusKey()] = mocca::net::successStatus();
                sendReply(reply, binary, envelope.connectionID);
            } catch (const std::runtime_error& err) {
                LERROR(err.what());
                // in case of an error, send error message back to client
                JsonCpp::Value reply;
                reply[mocca::net::statusKey()] = mocca::net::errorStatus();
                reply[mocca::net::errorKey()] = err.what();
                sendReply(reply, {}, envelope.connectionID);
            }
        }
    }
}

JsonCpp::Value Dispatcher::parseMessage(const mocca::net::Message& message) {
    JsonCpp::Reader reader;
    JsonCpp::Value root;
    std::string json(reinterpret_cast<const char*>(message[0]->data()), message[0]->size());
    LDEBUG("Request: " << json);
    if (!reader.parse(json, root)) {
        throw Error("JSON parse error: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
    return root;
}

void Dispatcher::registerMethod(Method method) {
    methods_.push_back(std::move(method));
}

void Dispatcher::registerReflection() {
    MethodDescription description(mocca::net::describe(), {});
    Method method(description, [this](const JsonCpp::Value&) {
        JsonCpp::Value result;
        int count = 0;
        for (const auto& m : methods_) {
            result[count++] = MethodDescription::toJson(m.description());
        }
        return std::make_pair(result, std::vector<mocca::net::MessagePart>());
    });
    registerMethod(std::move(method));
}