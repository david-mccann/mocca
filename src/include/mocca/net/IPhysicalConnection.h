#pragma once

#include <string>
#include <chrono>

#include "mocca/base/ByteArray.h"

namespace mocca {
    
namespace net {

class IPhysicalConnection {
public:
    virtual ~IPhysicalConnection() {}

    virtual std::string identifier() const = 0;
    virtual void send(ByteArray message) const = 0;
    virtual ByteArray receive(std::chrono::milliseconds timeout = std::chrono::milliseconds(500)) const = 0;
};
}
}