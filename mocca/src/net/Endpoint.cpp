#include "mocca/net/Endpoint.h"

#include "mocca/base/Error.h"

using namespace mocca::net;

Endpoint::Endpoint(const std::string& protocol, const std::string& address)
    : protocol_(protocol)
    , address_(address) {}

bool Endpoint::equals(const Endpoint& other) const {
    return protocol_ == other.protocol_ && address_ == other.address_;
}

std::string Endpoint::toString() const {
    return protocol_ + ":" + address_;
}

std::string mocca::net::Endpoint::protocol() const {
    return protocol_;
}

std::string mocca::net::Endpoint::address() const {
    return address_;
}