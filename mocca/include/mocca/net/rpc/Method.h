#pragma once

#include "mocca/net/Message.h"
#include "mocca/net/rpc/MethodDescription.h"

#include "jsoncpp/json.h"

#include <functional>

namespace mocca {
namespace net {

class Method {
public:
    using ReturnType = std::pair<JsonCpp::Value, std::vector<mocca::net::MessagePart>>;
    using CallbackType = std::function<ReturnType(const JsonCpp::Value&)>;
    Method(const MethodDescription& description, const CallbackType callback);

    std::string name() const;
    MethodDescription description() const;

    ReturnType operator()(const JsonCpp::Value& params) const;

private:
    MethodDescription description_;
    CallbackType callback_;
};
}
}