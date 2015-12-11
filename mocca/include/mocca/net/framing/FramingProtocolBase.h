#pragma once

#include "mocca/base/ByteArray.h"

#include <memory>
#include <chrono>

namespace mocca {
namespace net {

template <typename Derived, typename StreamType> class FramingProtocolBase {
public:
    static ByteArray readFrameFromStream(StreamType& stream, std::chrono::milliseconds timeout) {
        return Derived::readFrameFromStreamImpl(stream, timeout);
    }

    static void writeFrameToStream(StreamType& stream, ByteArray frame, std::chrono::milliseconds timeout) {
        Derived::writeFrameToStreamImpl(stream, std::move(frame), timeout);
    }

    static void performHandshake(StreamType& stream, std::chrono::milliseconds timeout) {
        Derived::performHandshakeImpl(stream, timeout);
    }
};
}
}
