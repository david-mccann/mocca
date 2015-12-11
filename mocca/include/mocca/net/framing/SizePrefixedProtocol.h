#pragma once

#include "mocca/net/framing/FramingProtocolBase.h"
#include "mocca/net/framing/FramingUtils.h"

namespace mocca {
namespace net {

template <typename IOStreamType> class SizePrefixedProtocol : public FramingProtocolBase<SizePrefixedProtocol<IOStreamType>, IOStreamType> {
public:
    friend class FramingProtocolBase<SizePrefixedProtocol<IOStreamType>, IOStreamType>;
    using FramingProtocolBase::FramingProtocolBase; // inherit constructor

private:
    ByteArray readFrameFromStreamImpl(std::chrono::milliseconds timeout) const;
    void writeFrameToStreamImpl(ByteArray frame, std::chrono::milliseconds timeout) const;
};
}
}
