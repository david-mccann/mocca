/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#pragma once

#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class SizePrefixedProtocol : public FramingStrategy {
public:
    std::unique_ptr<FramingStrategy> clone() const override;
    std::string name() const override;
    ByteArray readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) override;
    void writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) override;
};
}
}
