#pragma once

#include "mocca/net/IMessageConnection.h"
#include "mocca/net/Endpoint.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IMessageConnectionAcceptor {
public:
    virtual ~IMessageConnectionAcceptor() {}
    virtual std::string protocol() const = 0;
    virtual Endpoint connectionEndpoint() const = 0;
    virtual std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
};
}
}