#pragma once

#include "mocca/net/IStreamConnection.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IStreamConnectionAcceptor {
public:
    virtual ~IStreamConnectionAcceptor() {}

    virtual std::unique_ptr<IStreamConnection> accept(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
};
}
}