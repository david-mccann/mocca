#pragma once

#include "mocca/base/ByteArray.h"

#include <memory>
#include <chrono>

namespace mocca {
namespace net {

template <typename Derived, typename IOStreamType> class FramingProtocolBase {
public:
    FramingProtocolBase(std::unique_ptr<IOStreamType> ioStream)
        : ioStream_(std::move(ioStream)) {}

    ByteArray readFrameFromStream(std::chrono::milliseconds timeout) const {
        return static_cast<const Derived*>(this)->readFrameFromStreamImpl(timeout);
    }
    void writeFrameToStream(ByteArray frame, std::chrono::milliseconds timeout) const {
        static_cast<const Derived*>(this)->writeFrameToStreamImpl(std::move(frame), timeout);
    }

protected:
    std::unique_ptr<IOStreamType> ioStream_;
};
}
}
