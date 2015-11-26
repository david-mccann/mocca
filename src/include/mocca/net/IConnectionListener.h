#pragma once

#include <memory>
#include <chrono>

namespace mocca {
namespace net {

class AbstractConnection;

class IConnectionListener {
public:
    virtual ~IConnectionListener() {}

    virtual std::unique_ptr<AbstractConnection> getConnection(std::chrono::milliseconds timeout
        = std::chrono::milliseconds(500)) = 0;
};
}
}