#include "mocca/net/Endpoint.h"

#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"

using namespace mocca::net;

Endpoint::Endpoint(const std::string& protocol, const std::string& address)
    : protocol_(protocol)
    , address_(address) {}

Endpoint::Endpoint(const std::string& str) {
    auto index = str.find_first_of(':');
    if (index == std::string::npos) {
        throw Error(mocca::formatString("Cannot initialize endpoint from string '%%'", str), __FILE__, __LINE__);
    }
    protocol_ = str.substr(0, index);
    address_ = str.substr(index + 1, std::string::npos);
}

bool Endpoint::equals(const Endpoint& other) const {
    return protocol_ == other.protocol_ && address_ == other.address_;
}

std::string Endpoint::toString() const {
    return protocol_ + ":" + address_;
}

std::string Endpoint::protocol() const {
    return protocol_;
}

std::string Endpoint::address() const {
    return address_;
}

namespace mocca {
namespace net {
bool operator==(const Endpoint& lhs, const Endpoint& rhs) {
    return lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const Endpoint& obj) {
    return os << obj.toString();
}
}
}
