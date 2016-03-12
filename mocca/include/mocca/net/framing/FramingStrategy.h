/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/IStreamConnection.h"

#include <memory>

namespace mocca {
namespace net {

class FramingStrategy {
public:
    virtual ~FramingStrategy() {}
    virtual std::unique_ptr<FramingStrategy> clone() const = 0;
    virtual std::string name() const = 0;
    virtual ByteArray readFrameFromStream(IStreamConnection& connection,
                                          std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) = 0;
    virtual void writeFrameToStream(IStreamConnection& connection, ByteArray frame) = 0;
    virtual void performHandshake(IStreamConnection& connection, std::chrono::milliseconds timeout = std::chrono::milliseconds(100)) {}
};
}
}
