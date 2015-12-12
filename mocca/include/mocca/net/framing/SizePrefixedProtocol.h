#pragma once

#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class SizePrefixedProtocol : FramingStrategy, CloneableMixin<SizePrefixedProtocol> {
public:
    ByteArray readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) override;
    void writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) override;
};
}
}
