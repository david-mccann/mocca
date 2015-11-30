#include "mocca/net/Endpoint.h"

using namespace mocca::net;

Endpoint::Endpoint(const std::string& protocol, const std::string& transport, const std::string& connectionString)
    : protocol_(protocol)
    , transport_(transport)
    , connectionString_(connectionString) {}

bool Endpoint::equals(const Endpoint& other) const {
    return protocol_ == other.protocol_ && transport_ == other.transport_ && connectionString_ == other.connectionString_;
}

std::string Endpoint::toString() const {
    return protocol_ + ":" + transport_ + ":" + connectionString_;
}

std::string mocca::net::Endpoint::protocol() const {
    return protocol_;
}

std::string Endpoint::transport() const {
    return transport_;
}

std::string Endpoint::connectionString() const {
    return connectionString_;
}
