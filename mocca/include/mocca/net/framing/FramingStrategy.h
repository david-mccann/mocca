#pragma once

#include "mocca/net/IStreamConnection.h"

#include <memory>

namespace mocca {
namespace net {

class FramingStrategy  {
public:
    virtual ~FramingStrategy() {}
    virtual std::unique_ptr<FramingStrategy> clone() const = 0;
    virtual ByteArray readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
    virtual void writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
    virtual void performHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {}
};
}
}
