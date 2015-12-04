#pragma once

#include <memory>
#include <chrono>

namespace mocca {
namespace net {

class IPhysicalConnection;

class IPhysicalConnectionAcceptor {
public:
    virtual ~IPhysicalConnectionAcceptor() {}

    virtual std::unique_ptr<IPhysicalConnection> getConnection(std::chrono::milliseconds timeout
        = std::chrono::milliseconds(200)) = 0;
};
}
}