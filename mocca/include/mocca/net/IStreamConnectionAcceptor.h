#pragma once

#include "mocca/net/IStreamConnection.h"
#include "mocca/net/Endpoint.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IStreamConnectionAcceptor {
public:
    virtual ~IStreamConnectionAcceptor() {}

    virtual std::string protocol() const = 0;
    virtual Endpoint connectionEndpoint() const = 0;
    virtual std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
};
}
}