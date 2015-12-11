#pragma once

#include "mocca/net/message/IMessageConnection.h"

#include <chrono>
#include <memory>

namespace mocca {
namespace net {

class IMessageConnectionAcceptor {
public:
    virtual ~IMessageConnectionAcceptor() {}

    virtual std::unique_ptr<IMessageConnection> getConnection(std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
};
}
}