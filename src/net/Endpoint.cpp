#include "mocca/net/Endpoint.h"

using namespace mocca::net;

Endpoint::Endpoint(const std::string& transport, const std::string& connectionString)
    : transport_(transport)
    , connectionString_(connectionString) {}

bool Endpoint::equals(const Endpoint& other) const {
    return transport_ == other.transport_ && connectionString_ == other.connectionString_;
}

std::string Endpoint::toString() const {
    return transport_ + ":" + connectionString_;
}

std::string Endpoint::transport() const {
    return transport_;
}

std::string Endpoint::connectionString() const {
    return connectionString_;
}
