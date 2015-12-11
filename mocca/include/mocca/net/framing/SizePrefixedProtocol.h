#pragma once

#include "mocca/net/framing/FramingProtocolBase.h"
#include "mocca/net/framing/FramingUtils.h"

namespace mocca {
namespace net {

template <typename StreamType> class SizePrefixedProtocol : public FramingProtocolBase<SizePrefixedProtocol<StreamType>, StreamType> {
public:
    friend class FramingProtocolBase<SizePrefixedProtocol<StreamType>, StreamType>;

private:
    static ByteArray readFrameFromStreamImpl(StreamType& stream, std::chrono::milliseconds timeout);
    static void writeFrameToStreamImpl(StreamType& stream, ByteArray frame, std::chrono::milliseconds timeout);
    static void performHandshakeImpl(StreamType& stream, std::chrono::milliseconds timeout);
};
}
}
