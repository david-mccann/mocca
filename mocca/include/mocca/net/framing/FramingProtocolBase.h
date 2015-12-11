#pragma once

#include "mocca/base/ByteArray.h"

#include <memory>

namespace mocca {
namespace net {

template <typename Derived, typename IOStreamType> class FramingProtocolBase {
public:
    FramingProtocolBase(std::unique_ptr<IOStreamType> ioStream)
        : ioStream_(std::move(ioStream)) {}

    ByteArray readFrameFromStream() { return static_cast<Derived*>(this)->readFrameFromStreamImpl(); }

    void writeFrameToStream(ByteArray frame) { static_cast<Derived*>(this)->writeFrameToStreamImpl(std::move(frame)); }

protected:
    std::unique_ptr<IOStreamType> ioStream_;
};
}
}
