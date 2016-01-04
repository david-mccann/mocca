#pragma once

#include "mocca/base/Error.h"
#include "mocca/net/ConnectionID.h"

namespace mocca {
namespace net {

class NetworkError : public Error {
public:
    NetworkError(const std::string& msg, const std::string& file, int line)
        : Error(msg, file, line) {}
};

class ConnectionClosedError : public NetworkError {
public:
    ConnectionClosedError(const std::string& msg, const ConnectionID& connectionID, const std::string& file, int line)
        : NetworkError(msg, file, line)
        , connectionID_(connectionID) {}
    const ConnectionID& connectionID() const { return connectionID_; }

private:
    ConnectionID connectionID_;
};

class ConnectFailedError : public NetworkError {
public:
    ConnectFailedError(const std::string& msg, const std::string& file, int line)
        : NetworkError(msg, file, line) {}
};
}
}