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
    Message readMessageFromStream(IStreamConnection& connection) override;
    void writeMessageToStream(IStreamConnection& connection, Message frame) override;
};
}
}
