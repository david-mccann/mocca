#pragma once

#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class SizePrefixedProtocol : public FramingStrategy {
public:
    std::unique_ptr<FramingStrategy> clone() const;
    std::string name() const override;
    ByteArray readFrameFromStream(IStreamConnection& connection, std::chrono::milliseconds timeout) override;
    void writeFrameToStream(IStreamConnection& connection, ByteArray frame, std::chrono::milliseconds timeout) override;
};
}
}
