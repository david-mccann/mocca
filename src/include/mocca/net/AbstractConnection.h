#pragma once

#include <string>
#include <chrono>

#include "mocca/base/ByteArray.h"

namespace mocca {
    
namespace net {

class AbstractConnection {
public:
    AbstractConnection();
    virtual ~AbstractConnection() {}

    std::string identifier() const;
    void setIdentifier(const std::string& identifier);

    virtual void send(ByteArray message) const = 0;
    virtual ByteArray receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) const = 0;

private:
    std::string createIdentifier() const;

    std::string identifier_;
};
}
}